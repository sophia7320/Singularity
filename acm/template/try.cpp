#include <iostream>
#include <vector>
#include <queue>
#include <stdint.h>

using namespace std;
using ll = long long;
using vll = vector<ll>;
using vint = vector<int>;

const int N = 1e5 + 10;

int st[N][20];

inline int read() {
    int x = 0, f = 1;char ch = getchar();
    while (ch < '0' || ch>'9') { if (ch == '-') f = -1;ch = getchar(); }
    while (ch >= '0' && ch <= '9') { x = x * 10 + ch - 48;ch = getchar(); }
    return x * f;
}


void build(int n) {
    for (int j = 1; j < 20; j++)
        for (int i = 1; i + (1 << j) - 1 <= n; i++)
            st[i][j] = max(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
}

int query(int l, int r) {
    int k = __lg(r - l + 1);
    return max(st[l][k], st[r - (1 << k) + 1][k]);
}

void _() {
    int n, q;n = read(), q = read();
    for (int i = 1;i <= n;i++)
        cin >> st[i][0];
    build(n);

    while (q--) {
        int i, j;
        i = read(), j = read();
        cout << query(i, j) << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cout.tie(nullptr), cin.tie(nullptr);

    int t = 1;//cin >> t;
    while (t--) _();

    cout.flush();
}
