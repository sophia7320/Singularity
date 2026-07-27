#include <iostream>
#include <cassert>
#include <cstdlib>
using namespace std;
using ll = long long;

const int N = 1e5 + 10;
ll a[N];  // 原始数组 (下标从 1 开始)

struct SegTree {
    ll sum[N << 2], tag[N << 2];

    void pushup(int u) { sum[u] = sum[u << 1] + sum[u << 1 | 1]; }

    void apply(int u, int len, ll k) {
        sum[u] += k * len;
        tag[u] += k;
    }

    void pushdown(int u, int l, int r) {
        if (!tag[u]) return;
        int mid = (l + r) >> 1;
        apply(u << 1, mid - l + 1, tag[u]);
        apply(u << 1 | 1, r - mid, tag[u]);
        tag[u] = 0;
    }

    void build(int u, int l, int r) {
        tag[u] = 0;  // 清除上一次 build/update 残留的 tag
        if (l == r) { sum[u] = a[l]; return; }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid);
        build(u << 1 | 1, mid + 1, r);
        pushup(u);
    }

    void update(int u, int l, int r, int ql, int qr, ll k) {
        if (ql <= l && r <= qr) { apply(u, r - l + 1, k); return; }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) update(u << 1, l, mid, ql, qr, k);
        if (qr > mid)  update(u << 1 | 1, mid + 1, r, ql, qr, k);
        pushup(u);
    }

    ll query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return sum[u];
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        ll res = 0;
        if (ql <= mid) res += query(u << 1, l, mid, ql, qr);
        if (qr > mid)  res += query(u << 1 | 1, mid + 1, r, ql, qr);
        return res;
    }
} seg;

// 蛮力验证
ll brute[N];

int main() {
    // 测试 1: 单元素
    {
        const int n = 1;
        a[1] = 5; brute[1] = 5;
        seg.build(1, 1, n);
        assert(seg.query(1, 1, n, 1, 1) == 5);
        seg.update(1, 1, n, 1, 1, 3);
        brute[1] += 3;
        assert(seg.query(1, 1, n, 1, 1) == brute[1]);
    }

    // 测试 2: 小规模通用测试
    const int n = 200;
    for (int i = 1; i <= n; i++) {
        a[i] = (i * 37 + 123) % 100;
        brute[i] = a[i];
    }
    seg.build(1, 1, n);

    // 全区间查询
    {
        ll sum = 0;
        for (int i = 1; i <= n; i++) sum += brute[i];
        assert(seg.query(1, 1, n, 1, n) == sum);
    }

    // 子区间查询
    for (int l = 1; l <= n; l++) {
        ll sum = 0;
        for (int r = l; r <= n; r++) {
            sum += brute[r];
            assert(seg.query(1, 1, n, l, r) == sum);
        }
    }

    // 区间更新 + 查询
    {
        int l = 10, r = 50;
        ll k = 7;
        seg.update(1, 1, n, l, r, k);
        for (int i = l; i <= r; i++) brute[i] += k;

        // 验证全部区间
        for (int ql = 1; ql <= n; ql++) {
            ll sum = 0;
            for (int qr = ql; qr <= n; qr++) {
                sum += brute[qr];
                assert(seg.query(1, 1, n, ql, qr) == sum);
            }
        }
    }

    // 重叠更新
    {
        seg.update(1, 1, n, 1, 100, 10);
        for (int i = 1; i <= 100; i++) brute[i] += 10;
        seg.update(1, 1, n, 50, 150, 5);
        for (int i = 50; i <= 150; i++) brute[i] += 5;

        for (int ql = 1; ql <= n; ql++) {
            ll sum = 0;
            for (int qr = ql; qr <= n; qr++) {
                sum += brute[qr];
                assert(seg.query(1, 1, n, ql, qr) == sum);
            }
        }
    }

    // 全区间更新
    {
        seg.update(1, 1, n, 1, n, 100);
        for (int i = 1; i <= n; i++) brute[i] += 100;

        ll sum = 0;
        for (int i = 1; i <= n; i++) sum += brute[i];
        assert(seg.query(1, 1, n, 1, n) == sum);
    }

    // 负数更新
    {
        seg.update(1, 1, n, 30, 70, -15);
        for (int i = 30; i <= 70; i++) brute[i] -= 15;

        for (int i = 1; i <= n; i++) {
            assert(seg.query(1, 1, n, i, i) == brute[i]);
        }
    }

    // === 随机压力测试 (3000 次随机操作) ===
    {
        const int n = 200;
        ll bf[201] = {};
        // 完全重新建树，避免残留
        for (int i = 1; i <= n; i++) {
            a[i] = (i * 12345LL + 67890) % 10000 - 5000;
            bf[i] = a[i];
        }
        seg.build(1, 1, n);

        for (int op = 0; op < 3000; op++) {
            int l = rand() % n + 1;
            int r = rand() % n + 1;
            if (l > r) swap(l, r);
            if (rand() & 1) {
                // 区间加
                ll k = rand() % 2001 - 1000; // -1000 ~ 1000
                seg.update(1, 1, n, l, r, k);
                for (int i = l; i <= r; i++) bf[i] += k;
            } else {
                // 查询区间和
                ll expect = 0;
                for (int i = l; i <= r; i++) expect += bf[i];
                ll got = seg.query(1, 1, n, l, r);
                if (expect != got) {
                    cerr << "MISMATCH at op " << op << ": query [" << l << ", " << r
                         << "] expect=" << expect << " got=" << got << endl;
                    abort();
                }
            }
        }
    }

    // === 边界: 多次 build + update 不同 n ===
    for (int n : {1, 2, 3, 4, 5, 10, 50, 100}) {
        for (int i = 1; i <= n; i++) {
            a[i] = i;
            brute[i] = i;
        }
        seg.build(1, 1, n);
        // 全部单点验证
        for (int i = 1; i <= n; i++)
            assert(seg.query(1, 1, n, i, i) == i);
        // 更新 [1, n]
        seg.update(1, 1, n, 1, n, 10);
        for (int i = 1; i <= n; i++) brute[i] += 10;
        for (int i = 1; i <= n; i++)
            assert(seg.query(1, 1, n, i, i) == brute[i]);
    }

    cout << "SegTree: ALL TESTS PASSED" << endl;
    return 0;
}
