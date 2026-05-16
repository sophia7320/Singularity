#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
using namespace std;

const int N = 1e5 + 10;

int in[N];
vector<int> g[N];

vector<int> topo(int n) {
    vector<int> res;
    queue<int> q;
    for (int i = 1; i <= n; i++)
        if (!in[i]) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        res.push_back(u);
        for (int v : g[u])
            if (--in[v] == 0) q.push(v);
    }
    return res;
}

void clear_graph(int n) {
    for (int i = 1; i <= n; i++) {
        g[i].clear();
        in[i] = 0;
    }
}

int main() {
    // 测试 1: 单点
    {
        int n = 1;
        clear_graph(n);
        auto res = topo(n);
        assert(res.size() == 1 && res[0] == 1);
    }

    // 测试 2: 简单链 1->2->3->4
    {
        int n = 4;
        clear_graph(n);
        g[1].push_back(2); in[2]++;
        g[2].push_back(3); in[3]++;
        g[3].push_back(4); in[4]++;
        auto res = topo(n);
        assert(res.size() == 4);
        assert(res == vector<int>({1, 2, 3, 4}));
    }

    // 测试 3: DAG 分支
    {
        int n = 5;
        clear_graph(n);
        // 1 -> 2, 1 -> 3, 2 -> 4, 3 -> 4, 4 -> 5
        g[1].push_back(2); in[2]++;
        g[1].push_back(3); in[3]++;
        g[2].push_back(4); in[4]++;
        g[3].push_back(4); in[4]++;
        g[4].push_back(5); in[5]++;
        auto res = topo(n);
        assert(res.size() == 5);
        // 检查每个节点的前驱都在其前面
        int pos[6];
        for (int i = 0; i < 5; i++) pos[res[i]] = i;
        assert(pos[1] < pos[2] && pos[1] < pos[3]);
        assert(pos[2] < pos[4] && pos[3] < pos[4]);
        assert(pos[4] < pos[5]);
    }

    // 测试 4: 有环 (res.size() < n)
    {
        int n = 3;
        clear_graph(n);
        g[1].push_back(2); in[2]++;
        g[2].push_back(3); in[3]++;
        g[3].push_back(1); in[1]++;
        auto res = topo(n);
        assert(res.size() < 3);
    }

    // 测试 5: 自环
    {
        int n = 2;
        clear_graph(n);
        g[1].push_back(2); in[2]++;
        g[2].push_back(2); in[2]++; // 自环
        auto res = topo(n);
        assert(res.size() < 2);
    }

    // 测试 6: 多个入度为 0 的起点
    {
        int n = 4;
        clear_graph(n);
        g[1].push_back(3); in[3]++;
        g[2].push_back(4); in[4]++;
        auto res = topo(n);
        assert(res.size() == 4);
        int pos[5];
        for (int i = 0; i < 4; i++) pos[res[i]] = i;
        assert(pos[1] < pos[3]);
        assert(pos[2] < pos[4]);
    }

    // 测试 7: 空图 (无边)
    {
        int n = 10;
        clear_graph(n);
        auto res = topo(n);
        assert(res.size() == 10);
    }

    cout << "TopoSort: ALL TESTS PASSED" << endl;
    return 0;
}
