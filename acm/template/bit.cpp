#include <iostream>
#include <cassert>
#include <cstdlib>
using namespace std;
using ll = long long;

const int N = 1e5 + 10;

struct BIT {
    ll t[N]; int n;

    void init(int n_) { n = n_; fill(t, t + n + 2, 0); }

    int lowbit(int x) { return x & -x; }

    void add(int x, ll k) {
        for (; x <= n; x += lowbit(x)) t[x] += k;
    }

    ll sum(int x) {
        ll res = 0;
        for (; x; x -= lowbit(x)) res += t[x];
        return res;
    }

    ll range(int l, int r) {
        if (l > r) return 0;
        return sum(r) - sum(l - 1);
    }
};

int main() {
    BIT bit;
    const int n = 1000;
    bit.init(n);

    // 边界: 初始全 0
    for (int i = 1; i <= n; i++)
        assert(bit.sum(i) == 0);

    // 单点加 + 前缀和
    bit.add(1, 10);
    assert(bit.sum(1) == 10);
    assert(bit.sum(2) == 10);
    assert(bit.sum(n) == 10);

    bit.add(500, 20);
    assert(bit.sum(499) == 10);
    assert(bit.sum(500) == 30);
    assert(bit.sum(n) == 30);

    // 负数
    bit.add(1, -5);
    assert(bit.sum(1) == 5);

    // 区间查询
    assert(bit.range(1, 1) == 5);
    assert(bit.range(1, 500) == 25);
    assert(bit.range(500, 500) == 20);
    assert(bit.range(501, 1000) == 0);

    // 边界: l > r
    assert(bit.range(5, 3) == 0);

    // 边界: sum(0)
    // sum(0) 不会进循环，应返回 0
    assert(bit.sum(0) == 0);

    // 大量操作后验证
    BIT bit2;
    bit2.init(100);
    int a[101] = {};
    for (int i = 1; i <= 100; i++) {
        int val = (i * 137) % 100 - 50; // -50 ~ 49
        a[i] += val;
        bit2.add(i, val);
    }
    for (int i = 1; i <= 100; i++) {
        ll expect = 0;
        for (int j = 1; j <= i; j++) expect += a[j];
        assert(bit2.sum(i) == expect);
    }

    // 区间和大量测试
    for (int l = 1; l <= 100; l++) {
        ll prefix = 0;
        for (int r = l; r <= 100; r++) {
            prefix += a[r];
            assert(bit2.range(l, r) == prefix);
        }
    }

    // === 随机压力测试 ===
    {
        const int n = 1000;
        BIT b3; b3.init(n);
        ll bf[1001] = {};
        for (int op = 0; op < 5000; op++) {
            int t = rand() % 3;
            if (t == 0) {
                // 单点加
                int x = rand() % n + 1;
                ll k = rand() % 2001 - 1000;
                b3.add(x, k);
                bf[x] += k;
            } else if (t == 1) {
                // 前缀和
                int x = rand() % n + 1;
                ll expect = 0;
                for (int i = 1; i <= x; i++) expect += bf[i];
                assert(b3.sum(x) == expect);
            } else {
                // 区间和
                int l = rand() % n + 1;
                int r = rand() % n + 1;
                if (l > r) swap(l, r);
                ll expect = 0;
                for (int i = l; i <= r; i++) expect += bf[i];
                assert(b3.range(l, r) == expect);
            }
        }
    }

    cout << "BIT: ALL TESTS PASSED" << endl;
    return 0;
}
