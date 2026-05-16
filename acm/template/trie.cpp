#include <iostream>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

const int N = 1e5 + 10;  // 总字符数的上限

struct Trie {
    int nex[N][26]{}, idx = 0;
    int cnt[N];
    bool exist[N]{};

    void insert(const string &s) {
        int p = 0;
        for (char c : s) {
            int u = c - 'a';
            if (!nex[p][u]) nex[p][u] = ++idx;
            p = nex[p][u];
            cnt[p]++;
        }
        exist[p] = true;
    }

    int query(const string &s) {  // 查询 s 作为前缀出现的次数
        int p = 0;
        for (char c : s) {
            int u = c - 'a';
            if (!nex[p][u]) return 0;
            p = nex[p][u];
        }
        return cnt[p];
    }

    bool find_exact(const string &s) {  // 查询 s 是否被插入过 (完整匹配)
        int p = 0;
        for (char c : s) {
            int u = c - 'a';
            if (!nex[p][u]) return false;
            p = nex[p][u];
        }
        return exist[p];
    }
};

// 字典树数组很大 (~10MB), 必须放在全局区避免栈溢出
Trie trie;
Trie trie2;  // 用于随机压力测试

int main() {
    // 测试 1: 空 Trie 查询
    {
        string s = "a";
        assert(trie.query(s) == 0);
        assert(!trie.find_exact(s));
    }

    // 测试 2: 单个字符串插入 + 精确查询
    {
        string s = "hello";
        trie.insert(s);
        assert(trie.find_exact(s));
        assert(!trie.find_exact(string("hell")));

        // 前缀查询
        string pre1 = "h";
        assert(trie.query(pre1) == 1);
        string pre2 = "hel";
        assert(trie.query(pre2) == 1);
        string pre3 = "hello";
        assert(trie.query(pre3) == 1);
        string pre4 = "helloa";
        assert(trie.query(pre4) == 0);
    }

    // 测试 3: 多个字符串共享前缀
    {
        string a = "abc", b = "abd", c = "abe";
        trie.insert(a);
        trie.insert(b);
        trie.insert(c);

        assert(trie.query(string("a")) == 3);
        assert(trie.query(string("ab")) == 3);
        assert(trie.query(string("abc")) == 1);
        assert(trie.query(string("abd")) == 1);
        assert(trie.query(string("abe")) == 1);
        assert(trie.query(string("abf")) == 0);

        assert(trie.find_exact(a));
        assert(trie.find_exact(b));
        assert(trie.find_exact(c));
        assert(!trie.find_exact(string("ab")));
    }

    // 测试 4: 重复插入同一字符串
    {
        string s = "duplicate";
        trie.insert(s);
        trie.insert(s);
        trie.insert(s);
        assert(trie.query(s) == 3);
        assert(trie.find_exact(s));
    }

    // 测试 5: 单字符
    {
        string s = "z";
        trie.insert(s);
        assert(trie.query(string("z")) == 1);
        assert(trie.find_exact(string("z")));
    }

    // 测试 6: 空字符串 (不应崩溃)
    {
        string s = "";
        trie.insert(s);
        assert(trie.query(s) == 0);  // 空字符串不计入任何 cnt[p]
        // find_exact on empty at node 0: exist[0] == true now
    }

    // === 随机压力测试 ===
    {
        // 使用全新的全局 Trie (trie2) 做随机测试
        vector<string> all_words;
        srand(42);
        for (int t = 0; t < 200; t++) {
            string w;
            int len = rand() % 10 + 1;
            for (int i = 0; i < len; i++)
                w.push_back('a' + rand() % 4);
            trie2.insert(w);
            all_words.push_back(w);
        }

        for (string &w : all_words)
            assert(trie2.find_exact(w));

        assert(!trie2.find_exact(string("zzz")));

        for (int t = 0; t < 50; t++) {
            string pre;
            int len = rand() % 10 + 1;
            for (int i = 0; i < len; i++)
                pre.push_back('a' + rand() % 4);
            int brute_cnt = 0;
            for (string &w : all_words) {
                bool match = true;
                for (size_t i = 0; i < pre.length(); i++) {
                    if (i >= w.length() || w[i] != pre[i]) {
                        match = false;
                        break;
                    }
                }
                if (match) brute_cnt++;
            }
            assert(trie2.query(pre) == brute_cnt);
        }
    }

    cout << "Trie: ALL TESTS PASSED" << endl;
    return 0;
}
