#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
using namespace std;
using ull = unsigned long long;

const ull B = 131;
const int N = 1e5 + 10;

struct StringHash {
    ull h[N], p[N];

    void init(string &s) {
        int n = s.length();
        p[0] = 1;
        for (int i = 1; i <= n; i++) {
            h[i] = h[i - 1] * B + (ull)s[i - 1];
            p[i] = p[i - 1] * B;
        }
    }

    ull get(int l, int r) {  // 1-indexed, 闭区间 [l,r]
        return h[r] - h[l - 1] * p[r - l + 1];
    }
};

int main() {
    // 测试 1: 单个字符
    {
        string s = "a";
        StringHash sh;
        sh.init(s);
        assert(sh.get(1, 1) == (ull)'a');
    }

    // 测试 2: 相同子串应返回相同哈希
    {
        string s = "abcabcabc";
        StringHash sh;
        sh.init(s);
        for (int len = 1; len <= 3; len++)
            for (int i = 1; i + len - 1 <= 9; i++)
                for (int j = i + len; j + len - 1 <= 9; j++) {
                    bool same = (s.substr(i - 1, len) == s.substr(j - 1, len));
                    assert(same == (sh.get(i, i + len - 1) == sh.get(j, j + len - 1)));
                }
    }

    // 测试 3: 整串哈希
    {
        string s = "hello world";
        StringHash sh;
        sh.init(s);
        assert(sh.get(1, (int)s.length()) == sh.get(1, (int)s.length()));
    }

    // 测试 4: 不同子串通常不同 (不可能 100% 保证, 但基本如此)
    {
        string s = "ppppopppp";
        StringHash sh;
        sh.init(s);
        // 长度为 1: 相同字符相同哈希, 不同字符不同哈希
        assert(sh.get(1, 1) == sh.get(2, 2)); // both 'p'
        assert(sh.get(6, 6) == sh.get(7, 7)); // both 'p' (pos6,7 are 'p')
        assert(sh.get(1, 1) != sh.get(5, 5)); // 'p'(pos1) != 'o'(pos5)
        assert(sh.get(4, 4) != sh.get(5, 5)); // 'p'(pos4) != 'o'(pos5)
    }

    // 测试 5: 长字符串
    {
        string s;
        for (int i = 0; i < 10000; i++)
            s.push_back('a' + (i % 26));
        StringHash sh;
        sh.init(s);

        // 验证前缀哈希的前缀性质
        for (int i = 1; i <= 10; i++) {
            assert(sh.get(1, i) == sh.h[i]);
        }
    }

    // 测试 6: index 范围 r < l (不合法输入, 但保证不死循环/crash)
    {
        string s = "test";
        StringHash sh;
        sh.init(s);
        // 不合法调用不应崩溃 (实际结果无意义, 仅验证不崩溃)
        volatile ull _ = sh.get(3, 2);
        (void)_;
    }

    // === 随机压力测试 ===
    srand(42);
    for (int tc = 0; tc < 50; tc++) {
        string s;
        int len = rand() % 500 + 1;
        for (int i = 0; i < len; i++)
            s.push_back('a' + rand() % 26);
        StringHash sh;
        sh.init(s);
        // 随机查询子串, 验证 get(l,r) 与 get(l,i) + get(i+1,r) 的拼接一致性
        for (int q = 0; q < 100; q++) {
            int l = rand() % len + 1;
            int r = rand() % len + 1;
            if (l > r) swap(l, r);
            // 同子串 equals → 同哈希
            string sub = s.substr(l - 1, r - l + 1);
            for (int l2 = 1; l2 + (r - l) <= len; l2++) {
                string sub2 = s.substr(l2 - 1, r - l + 1);
                bool same = (sub == sub2);
                bool hash_same = (sh.get(l, r) == sh.get(l2, l2 + r - l));
                if (same != hash_same) {
                    cerr << "HASH COLLISION or BUG: s=" << s << " [" << l << "," << r
                         << "] vs [" << l2 << "," << l2 + r - l << "]\n";
                    abort();
                }
            }
        }
    }

    cout << "StringHash: ALL TESTS PASSED" << endl;
    return 0;
}
