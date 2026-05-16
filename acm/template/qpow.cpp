#include <iostream>
#include <cassert>
#include <cstdlib>
using namespace std;
using ll = long long;

const ll MOD = 1e9 + 7;

ll qpow(ll a, ll b, ll p) {
    if (p == 1) return 0;
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % p;
        a = a * a % p;
        b >>= 1;
    }
    return res;
}

ll inv(ll a, ll p) {
    return qpow(a, p - 2, p);  // p 必须是质数
}

int main() {
    // === qpow 测试 ===
    // 边界: b=0
    assert(qpow(5, 0, MOD) == 1);
    assert(qpow(1, 0, MOD) == 1);
    assert(qpow(0, 0, MOD) == 1);

    // 边界: b=1
    assert(qpow(7, 1, MOD) == 7 % MOD);
    assert(qpow(0, 1, MOD) == 0);

    // 常规
    assert(qpow(2, 10, MOD) == 1024 % MOD);
    assert(qpow(2, 30, MOD) == qpow(2, 30, MOD)); // sanity

    // a%p==0, b>0
    assert(qpow(MOD, 5, MOD) == 0);
    assert(qpow(0, 100, MOD) == 0);

    // 大指数
    assert(qpow(2, 60, MOD) != 0);

    // mod=2 (小质数)
    assert(qpow(3, 5, 2) == 1);

    // === 逆元测试 (费马小定理) ===
    // p 为质数: a * inv(a) % p == 1
    const int primes[] = {2, 3, 5, 7, 11, 13, (int)MOD, 998244353};
    for (int p : primes) {
        for (ll a = 1; a <= min((ll)p - 1, 20LL); a++) {
            ll i = inv(a, p);
            assert(a * i % p == 1);
        }
        // 更大的随机测试
        for (ll a : {37LL, 123LL, 1000000LL}) {
            if (a % p != 0) {
                ll i = inv(a % p, p);
                assert((a % p) * i % p == 1);
            }
        }
    }

    // === 随机压力测试 ===
    srand(42);
    const int MOD2 = 998244353;
    for (int t = 0; t < 1000; t++) {
        ll a = rand() % (MOD2 - 1) + 1; // 1 ~ MOD2-1
        ll b = rand() % 2000;
        // 验证 (a^b mod p) 的一致性
        ll r1 = qpow(a, b, MOD2);
        // 验证逆元性质: a^b * a^(p-1-b) % p == a^(p-1) % p == 1
        if (b < MOD2 - 1) {
            ll r2 = qpow(a, (ll)MOD2 - 1 - b, MOD2);
            assert(r1 * r2 % MOD2 == 1);
        }
    }

    // mod=1 特殊边界
    assert(qpow(5, 0, 1) == 0);
    assert(qpow(5, 1, 1) == 0);
    assert(qpow(0, 100, 1) == 0);

    // 大指数: 2^30 == 1073741824
    assert(qpow(2, 30, 10000000000LL) == 1073741824LL);

    cout << "qpow / inv: ALL TESTS PASSED" << endl;
    return 0;
}
