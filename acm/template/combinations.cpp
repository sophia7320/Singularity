#include <iostream>
#include <cassert>
using namespace std;
using ll = long long;

const int MOD = 1e9 + 7;
const int N = 1e5 + 10;

ll qpow(ll a, ll b, ll p) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % p;
        a = a * a % p;
        b >>= 1;
    }
    return res;
}

ll fact[N], inv[N];

void prework(int n) {
    fact[0] = fact[1] = 1;
    for (int i = 2; i <= n; i++)
        fact[i] = fact[i - 1] * i % MOD;
    inv[n] = qpow(fact[n], MOD - 2, MOD);
    for (int i = n - 1; i >= 0; i--)
        inv[i] = inv[i + 1] * (i + 1) % MOD;
}

ll C(int n, int k) {
    if (k > n || k < 0) return 0;
    return fact[n] * inv[k] % MOD * inv[n - k] % MOD;
}

int main() {
    const int maxn = 100000;
    prework(maxn);

    // 边界: k=0
    assert(C(0, 0) == 1);
    assert(C(5, 0) == 1);

    // 边界: k=n
    assert(C(5, 5) == 1);
    assert(C(100, 100) == 1);

    // 边界: k>n
    assert(C(3, 5) == 0);
    assert(C(0, 1) == 0);

    // 边界: k<0
    assert(C(5, -1) == 0);

    // 常规
    assert(C(5, 2) == 10);
    assert(C(10, 3) == 120);
    assert(C(10, 7) == C(10, 3)); // 对称性

    // 模运算验证: C(n,1) == n
    for (int n : {1, 2, 7, 100, 1000, 10000}) {
        assert(C(n, 1) == n);
    }

    // C(n,2) == n*(n-1)/2
    for (int n : {2, 5, 100, 200}) {
        assert(C(n, 2) == (ll)n * (n - 1) / 2 % MOD);
    }

    // 大数测试
    assert(C(100000, 50000) > 0);

    // 对称性: C(n,k) == C(n,n-k)
    for (int n : {5, 10, 20, 50, 100, 200}) {
        for (int k = 0; k <= n; k++)
            assert(C(n, k) == C(n, n - k));
    }

    // 阶乘自身验证: fact[i] * inv[i] % MOD == 1
    for (int i = 0; i <= 1000; i++)
        assert(fact[i] * inv[i] % MOD == 1);

    // 递归关系: C(n,k) = C(n-1,k-1) + C(n-1,k)
    for (int n = 2; n <= 100; n++) {
        for (int k = 1; k < n; k++) {
            assert((C(n - 1, k - 1) + C(n - 1, k)) % MOD == C(n, k));
        }
    }

    // n 较小但 k 大, 仍正确
    assert(C(0, 0) == 1);
    assert(C(3, 0) == 1);
    assert(C(3, 3) == 1);

    cout << "combinations: ALL TESTS PASSED" << endl;
    return 0;
}
