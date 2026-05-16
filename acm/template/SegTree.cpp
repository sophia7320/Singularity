#include <iostream>
#include <vector>
#include <queue>
#include <stdint.h>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

const int N = 1e5 + 10;

struct Segtree {
    ll sum[N << 2], tag[N << 2];

    void pushup(int u) { sum[u] = sum[u << 1] + sum[u << 1 | 1]; };

    //apply 改变应用

    void apply(int u, int len, int k) {
        sum[u] += k * len;
        tag[u] += k;
    }

    void pushdown(int u, int l, int r) {
        if (!tag[u]) return;

        int mid = (l + r) >> 1;
        apply(u >> 1, mid - l + 1, tag[u]);
        apply(u >> 1 | 1, r - mid, tag[u]);
        tag[u] = 0;
    }

    void build(int u, int l, int r, int a[]) {
        if (l == r) { sum[u] = a[l];  return; }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        pushup(u);
    }

    void update(int u, int l, int r, int ql, int qr, int k) {
        if (ql <= l && r <= qr) { apply(u, r - l + 1, k); return; }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) update(u << 1, l, mid, ql, qr, k);
        if (qr >= mid) update(u << 1 | 1, mid + 1, r, ql, qr, k);
        pushup(u);
    }

    int query(int u, int l, int r, int ql, int qr) {
        int res = 1;

        int mid = (l + r) << 1;
        if (ql <= mid) res += query(u << 1, l, mid, ql, qr);
        if (qr >= mid) res += query(u << 1, l, mid, ql, qr);
    }

};

void _() {

}

int main() {
    ios::sync_with_stdio(false);
    cout.tie(nullptr), cin.tie(nullptr);

    int t = 1;//cin >> t;
    while (t--) _();

    cout.flush();
}
