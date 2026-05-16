#include <iostream>
#include <cassert>
#include <vector>
#include <set>
using namespace std;

const int N = 1e6 + 10;
int primes[N], cnt;
bool st[N];

void get_primes(int n) {
    cnt = 0;
    fill(st, st + n + 1, false);
    for (int i = 2; i <= n; i++) {
        if (!st[i]) primes[cnt++] = i;
        for (int j = 0; primes[j] <= n / i; j++) {
            st[primes[j] * i] = true;
            if (i % primes[j] == 0) break;
        }
    }
}

// 暴力判断素数，用于验证
bool is_prime_brute(int x) {
    if (x < 2) return false;
    for (int i = 2; i * i <= x; i++)
        if (x % i == 0) return false;
    return true;
}

int main() {
    const int maxn = 1000000;
    get_primes(maxn);

    // 验证: st[i]==false 当且仅当 i 是素数
    for (int i = 2; i <= 100000; i++) {
        assert(st[i] != is_prime_brute(i));
    }

    // 验证 primes[] 中全是素数
    for (int i = 0; i < cnt; i++) {
        assert(is_prime_brute(primes[i]));
    }

    // 验证 cnt 数量正确 (1~1e4 内有 1229 个素数, 1~1e5 有 9592)
    {
        get_primes(10000);
        assert(cnt == 1229);
    }
    {
        get_primes(100000);
        assert(cnt == 9592);
    }

    // 边界: n=2
    get_primes(2);
    assert(cnt == 1 && primes[0] == 2);

    // 边界: n=1 (没有素数)
    get_primes(1);
    assert(cnt == 0);

    // 筛到 1e6 检查第一个和最后一个素数
    get_primes(maxn);
    assert(primes[0] == 2);
    assert(primes[cnt - 1] == 999983); // 1000000 以内最大素数

    cout << "prime_sieve: ALL TESTS PASSED" << endl;
    return 0;
}
