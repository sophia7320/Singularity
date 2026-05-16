#include <iostream>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

// nxt[i]: P[0..i-1] 的最长相等真前后缀长度
void get_next(string &P, vector<int> &nxt) {
    int m = P.length();
    nxt.resize(m + 1);
    nxt[0] = -1;
    for (int i = 0, j = -1; i < m; ) {
        if (j == -1 || P[i] == P[j]) nxt[++i] = ++j;
        else j = nxt[j];
    }
}

vector<int> kmp(string &T, string &P) {
    vector<int> nxt, res;
    get_next(P, nxt);
    int n = T.length(), m = P.length();
    for (int i = 0, j = 0; i < n; ) {
        if (j == -1 || T[i] == P[j]) i++, j++;
        else j = nxt[j];
        if (j == m) {
            res.push_back(i - m);  // 0-indexed 起始位置
            j = nxt[j];
        }
    }
    return res;
}

// 暴力匹配，用于验证
vector<int> kmp_brute(string &T, string &P) {
    vector<int> res;
    int n = T.length(), m = P.length();
    for (int i = 0; i + m <= n; i++) {
        if (T.substr(i, m) == P) res.push_back(i);
    }
    return res;
}

int main() {
    // 测试 1: 基本匹配
    {
        string T = "abababab", P = "aba";
        auto res = kmp(T, P);
        auto brt = kmp_brute(T, P);
        assert(res == brt);
        assert(res.size() == 3);
    }

    // 测试 2: P 不在 T 中
    {
        string T = "abcdef", P = "xyz";
        auto res = kmp(T, P);
        assert(res.empty());
    }

    // 测试 3: P == T
    {
        string T = "hello", P = "hello";
        auto res = kmp(T, P);
        assert(res.size() == 1 && res[0] == 0);
    }

    // 测试 4: P 长度 > T 长度
    {
        string T = "a", P = "abcdef";
        auto res = kmp(T, P);
        assert(res.empty());
    }

    // 测试 5: P 为空 (空模式串行为视实现而定)
    {
        string T = "abc", P = "";
        auto res = kmp(T, P);
        // 空模式串: 此实现会匹配 T.length() 个位置 (每个字符间)
        assert((int)res.size() == (int)T.length());
    }

    // 测试 6: T 为空, P 非空
    {
        string T = "", P = "a";
        auto res = kmp(T, P);
        assert(res.empty());
    }

    // 测试 7: 全都是同一个字符
    {
        string T = "aaaaa", P = "aa";
        auto res = kmp(T, P);
        assert(res == vector<int>({0, 1, 2, 3}));
    }

    // 测试 8: 长字符串
    {
        string T, P;
        for (int i = 0; i < 10000; i++) T.push_back('a');
        for (int i = 0; i < 100; i++) P.push_back('a');
        auto res = kmp(T, P);
        assert((int)res.size() == 10000 - 100 + 1);
    }

    // 测试 9: 重叠匹配
    {
        string T = "ababa", P = "aba";
        auto res = kmp(T, P);
        assert(res == vector<int>({0, 2}));
    }

    // 测试 10: 单字符
    {
        string T = "abcabc", P = "b";
        auto res = kmp(T, P);
        assert(res == vector<int>({1, 4}));
    }

    // 测试 11: P 出现在 T 末尾
    {
        string T = "xyzabc", P = "abc";
        auto res = kmp(T, P);
        assert(res == vector<int>({3}));
    }

    // 测试 12: P 出现在 T 开头
    {
        string T = "abcxyz", P = "abc";
        auto res = kmp(T, P);
        assert(res == vector<int>({0}));
    }

    // === 随机压力测试 (100 组随机串) ===
    srand(42);
    for (int tc = 0; tc < 100; tc++) {
        string T, P;
        int n = rand() % 200 + 1;
        int m = rand() % 20 + 1;
        for (int i = 0; i < n; i++) T.push_back('a' + rand() % 3); // 小字母集增大重叠概率
        for (int i = 0; i < m; i++) P.push_back('a' + rand() % 3);
        auto res = kmp(T, P);
        auto brt = kmp_brute(T, P);
        if (res != brt) {
            cerr << "KMP MISMATCH: T=" << T << " P=" << P << endl;
            cerr << "  got: ";
            for (int x : res) cerr << x << " ";
            cerr << "  expect: ";
            for (int x : brt) cerr << x << " ";
            cerr << endl;
            abort();
        }
    }

    cout << "KMP: ALL TESTS PASSED" << endl;
    return 0;
}
