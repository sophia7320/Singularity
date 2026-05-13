#include <iostream>
#include <vector>
#include <functional>

using namespace std;

struct Trie {
    //p 是字典树上节点的编号 ~ 和idx相对应

    int nex[10000][26]{}, idx = 0;
    int cnt[10000];
    vector<int> exist = vector<int>(10000);

    void insert(string s) {
        int p = 0;int l = s.length();
        for (int i = 0;i < l;i++) {
            int c = s[i] - 'a';

            if (!nex[p][c]) nex[p][c] = ++idx;

            p = nex[p][c];cnt[p]++;
        }

        exist[p] = true;
    }

    int find(string s) {
        int p = 0, l = s.length();

        for (int i = 0;i < l;i++) {
            int c = s[i] - 'a';
            if (!nex[p][c]) return 0;
            p = nex[p][c];
        }

        return cnt[p];
    }
};

void testing() {
    int n, q;cin >> n >> q;
    Trie trie;

    while (n--) {
        string s;cin >> s;
        trie.insert(s);
    }

    while (q--) {
        string pa;cin >> pa;
        cout << trie.find(pa) << endl;
    }

}

int main() {
    ios::sync_with_stdio(false);
    cout.tie(nullptr), cin.tie(nullptr);

    int t = 1;cin >> t;
    while (t--)
        testing();
}