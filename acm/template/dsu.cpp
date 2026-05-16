#include <iostream>
#include <cassert>
using namespace std;

const int N = 1e5 + 10;

struct DSU {
    int fa[N], sz[N];

    void init(int n) {
        for (int i = 1; i <= n; i++) fa[i] = i, sz[i] = 1;
    }

    int find(int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }

    void merge(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return;
        if (sz[ra] < sz[rb]) swap(ra, rb);
        fa[rb] = ra;
        sz[ra] += sz[rb];
    }

    bool same(int a, int b) { return find(a) == find(b); }
};

int main() {
    DSU dsu;

    // 边界: 单元素
    dsu.init(1);
    assert(dsu.find(1) == 1);
    assert(dsu.same(1, 1));

    // 常规: 合并
    dsu.init(10);
    assert(!dsu.same(1, 2));
    dsu.merge(1, 2);
    assert(dsu.same(1, 2));
    assert(!dsu.same(1, 3));

    // 合并同一个集合不应该改变结果
    dsu.merge(1, 2);
    assert(dsu.same(1, 2));

    // 路径压缩验证: find(2) 之后 fa[2] 应指向根
    dsu.find(2);
    assert(dsu.fa[2] == dsu.find(1));

    // 多集合合并
    dsu.init(10);
    dsu.merge(1, 2);
    dsu.merge(2, 3);
    dsu.merge(4, 5);
    dsu.merge(5, 6);
    assert(dsu.same(1, 3));
    assert(dsu.same(4, 6));
    assert(!dsu.same(1, 4));

    dsu.merge(3, 6);
    assert(dsu.same(1, 4));
    assert(dsu.same(2, 5));

    // 大小: 合并后 sz 正确
    dsu.init(5);
    assert(dsu.sz[1] == 1);
    dsu.merge(1, 2);
    assert(dsu.sz[dsu.find(1)] == 2);
    dsu.merge(2, 3);
    assert(dsu.sz[dsu.find(1)] == 3);
    dsu.merge(4, 5);
    assert(dsu.sz[dsu.find(4)] == 2);
    dsu.merge(1, 4);
    assert(dsu.sz[dsu.find(1)] == 5);

    // 大量元素
    dsu.init(100000);
    for (int i = 2; i <= 100000; i++)
        dsu.merge(1, i);
    assert(dsu.sz[dsu.find(1)] == 100000);
    for (int i = 2; i <= 100000; i++)
        assert(dsu.same(1, i));

    cout << "DSU: ALL TESTS PASSED" << endl;
    return 0;
}
