#include <iostream>
#include <cassert>
#include <cstdlib>
using namespace std;
using ll = long long;

ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
ll lcm(ll a, ll b) { return a / gcd(a, b) * b; }

// ax + by = gcd(a, b), 求一组 x, y
ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (!b) { x = 1; y = 0; return a; }
    ll g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}

// exgcd 求 a 的逆元 (模 p, 需 gcd(a,p)=1)
ll exinv(ll a, ll p) {
    ll x, y;
    exgcd(a, p, x, y);
    return (x % p + p) % p;
}

int main() {
    // === gcd 测试 ===
    // 边界: gcd(0, x)
    assert(gcd(0, 5) == 5);
    assert(gcd(0, 0) == 0);
    assert(gcd(5, 0) == 5);

    // 常规
    assert(gcd(12, 8) == 4);
    assert(gcd(100, 25) == 25);
    assert(gcd(17, 13) == 1);  // 互质
    assert(gcd(1, 99999) == 1);

    // lcm
    assert(lcm(12, 8) == 24);
    assert(lcm(5, 7) == 35);
    assert(lcm(0, 5) == 0);    // 注意: lcm(0, a)=0
    assert(lcm(1, 1) == 1);

    // === exgcd 测试 ===
    for (ll a : {3LL, 5LL, 7LL, 12LL, 100LL, 1000000LL}) {
        for (ll b : {2LL, 5LL, 8LL, 13LL, 99LL, 123456LL}) {
            if (a == 0 && b == 0) continue;
            ll x, y;
            ll g = exgcd(a, b, x, y);
            assert(g == gcd(a, b));
            assert(a * x + b * y == g);
        }
    }

    // 边界: a=0
    {
        ll x, y;
        ll g = exgcd(0, 5, x, y);
        assert(g == 5);
        assert(0 * x + 5 * y == 5);
    }

    // 边界: b=0
    {
        ll x, y;
        ll g = exgcd(7, 0, x, y);
        assert(g == 7);
        assert(x == 1);
        assert(y == 0);
    }

    // === exinv 测试 ===
    // a * exinv(a, p) % p == 1
    int ps[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    for (int p : ps) {
        for (int a = 1; a < p; a++) {
            ll i = exinv(a, p);
            assert(i >= 0 && i < (ll)p);
            assert((ll)a * i % p == 1);
        }
    }

    // 非质数模但 gcd(a,p)=1
    assert(3 * (ll)exinv(3, 10) % 10 == 1);
    assert(7 * (ll)exinv(7, 100) % 100 == 1);

    // === 随机测试 (500 对) ===
    srand(42);
    for (int t = 0; t < 500; t++) {
        ll a = rand() % 100000 + 1;
        ll b = rand() % 100000 + 1;
        ll g = gcd(a, b);
        assert(a % g == 0 && b % g == 0);
        // 验证 exgcd
        ll x, y;
        ll eg = exgcd(a, b, x, y);
        assert(eg == g);
        assert(a * x + b * y == g);
    }

    // === 更大数值测试 ===
    {
        ll a = 1234567890123LL, b = 9876543210987LL;
        ll x, y;
        ll g = exgcd(a, b, x, y);
        assert(g == gcd(a, b));
        assert(a * x + b * y == g);
    }

    cout << "gcd / exgcd / exinv: ALL TESTS PASSED" << endl;
    return 0;
}
