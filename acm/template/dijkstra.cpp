#include <iostream>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;
using ll = long long;

const int N = 1e5 + 10;
const ll INF = 1e18;

struct Edge { int v; ll w; };
vector<Edge> g[N];
ll dist[N];
bool vis[N];

void dijkstra(int s, int n) {
    using P = pair<ll, int>;
    priority_queue<P, vector<P>, greater<P>> pq;
    fill(dist + 1, dist + n + 1, INF);
    fill(vis + 1, vis + n + 1, false);

    dist[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        int u = pq.top().second; pq.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (auto &e : g[u]) {
            if (dist[e.v] > dist[u] + e.w) {
                dist[e.v] = dist[u] + e.w;
                pq.push({dist[e.v], e.v});
            }
        }
    }
}

void clear_graph(int n) {
    for (int i = 1; i <= n; i++) g[i].clear();
}

int main() {
    // 测试 1: 单点图
    {
        int n = 1;
        clear_graph(n);
        dijkstra(1, n);
        assert(dist[1] == 0);
    }

    // 测试 2: 两点图
    {
        int n = 2;
        clear_graph(n);
        g[1].push_back({2, 5});
        dijkstra(1, n);
        assert(dist[1] == 0);
        assert(dist[2] == 5);
    }

    // 测试 3: 不可达点
    {
        int n = 3;
        clear_graph(n);
        g[1].push_back({2, 1});
        dijkstra(1, n);
        assert(dist[1] == 0);
        assert(dist[2] == 1);
        assert(dist[3] == INF);
    }

    // 测试 4: 多条路径取最短
    {
        int n = 4;
        clear_graph(n);
        g[1].push_back({2, 10});
        g[1].push_back({3, 1});
        g[3].push_back({2, 2});
        g[2].push_back({4, 5});
        g[3].push_back({4, 8});
        dijkstra(1, n);
        assert(dist[1] == 0);
        assert(dist[2] == 3);  // 1->3->2
        assert(dist[3] == 1);
        assert(dist[4] == 8);  // 1->3->2->4
    }

    // 测试 5: 自环
    {
        int n = 2;
        clear_graph(n);
        g[1].push_back({1, 100});
        g[1].push_back({2, 3});
        dijkstra(1, n);
        assert(dist[2] == 3);
    }

    // 测试 6: 重边
    {
        int n = 2;
        clear_graph(n);
        g[1].push_back({2, 100});
        g[1].push_back({2, 1});
        dijkstra(1, n);
        assert(dist[2] == 1);
    }

    // 测试 7: 中等规模稠密图
    {
        int n = 100;
        clear_graph(n);
        for (int i = 1; i < n; i++) {
            g[i].push_back({i + 1, 1});
            if (i + 2 <= n) g[i].push_back({i + 2, 3});
        }
        dijkstra(1, n);
        // 全走长度为 1 的边: 1->2->3->...->100 共 99 条 = 99
        // 3 权边比 2 条 1 权边 (cost=2) 更贵, 不走
        assert(dist[n] == 99);
    }

    // 测试 8: 大规模星形图
    {
        int n = 1000;
        clear_graph(n);
        for (int i = 2; i <= n; i++)
            g[1].push_back({i, (ll)i * 10});
        dijkstra(1, n);
        for (int i = 2; i <= n; i++)
            assert(dist[i] == (ll)i * 10);
        // 不可达点
        assert(dist[1] == 0);
    }

    // 测试 9: Floyd-Warshall 交叉验证 (n=50 随机图)
    {
        const int n = 50;
        // 邻接矩阵 (Floyd 用)
        ll adj[51][51];
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++)
                adj[i][j] = (i == j) ? 0 : INF;
        }
        clear_graph(n);
        srand(123);
        // 生成随机 DAG + 随机正权边
        for (int i = 1; i <= n; i++) {
            for (int j = i + 1; j <= n; j++) {
                if (rand() % 3 == 0) {
                    ll w = rand() % 100 + 1;
                    g[i].push_back({j, w});
                    adj[i][j] = min(adj[i][j], w);
                }
            }
        }
        // Floyd-Warshall
        for (int k = 1; k <= n; k++)
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++)
                    if (adj[i][k] != INF && adj[k][j] != INF)
                        adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][j]);
        // Dijkstra 验证
        dijkstra(1, n);
        for (int i = 1; i <= n; i++)
            assert(dist[i] == adj[1][i]);
    }

    cout << "Dijkstra: ALL TESTS PASSED" << endl;
    return 0;
}
