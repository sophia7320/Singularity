#include <iostream>
#include <cassert>
#include <cmath>
using namespace std;

const int N = 1e5 + 10;

int st[N][20];

void build(int n) {
    for (int j = 1; j < 20; j++)
        for (int i = 1; i + (1 << j) - 1 <= n; i++)
            st[i][j] = max(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
}

int query(int l, int r) {
    int k = __lg(r - l + 1);
    return max(st[l][k], st[r - (1 << k) + 1][k]);
}

// 暴力最大值
int brute_max(int l, int r) {
    int res = st[l][0];
    for (int i = l; i <= r; i++)
        res = max(res, st[i][0]);
    return res;
}

int main() {
    // 测试 1: 单元素
    {
        int n = 1;
        st[1][0] = 42;
        build(n);
        assert(query(1, 1) == 42);
    }

    // 测试 2: 小规模全排列
    {
        int n = 100;
        for (int i = 1; i <= n; i++)
            st[i][0] = (i * 17 + 53) % 200;
        build(n);
        for (int l = 1; l <= n; l++)
            for (int r = l; r <= n; r++)
                assert(query(l, r) == brute_max(l, r));
    }

    // 测试 3: 升序数组
    {
        int n = 200;
        for (int i = 1; i <= n; i++) st[i][0] = i;
        build(n);
        for (int l = 1; l <= n; l++)
            for (int r = l; r <= n; r++)
                assert(query(l, r) == r);
    }

    // 测试 4: 降序数组
    {
        int n = 200;
        for (int i = 1; i <= n; i++) st[i][0] = n - i + 1;
        build(n);
        for (int l = 1; l <= n; l++)
            for (int r = l; r <= n; r++)
                assert(query(l, r) == n - l + 1);
    }

    // 测试 5: 全相等
    {
        int n = 100;
        for (int i = 1; i <= n; i++) st[i][0] = 7;
        build(n);
        for (int l = 1; l <= n; l++)
            for (int r = l; r <= n; r++)
                assert(query(l, r) == 7);
    }

    // 测试 6: 边界 - 长度为 2 的各种区间
    {
        int n = 10;
        int arr[] = {0, 5, 3, 8, 2, 9, 1, 7, 4, 6, 10}; // 1-indexed
        for (int i = 1; i <= n; i++) st[i][0] = arr[i];
        build(n);
        assert(query(1, 2) == 5);
        assert(query(2, 3) == 8);
        assert(query(1, n) == 10);
    }

    // 测试 7: 负数值
    {
        int n = 50;
        for (int i = 1; i <= n; i++) st[i][0] = -(i * 3 % 20);
        build(n);
        for (int l = 1; l <= n; l++)
            for (int r = l; r <= n; r++)
                assert(query(l, r) == brute_max(l, r));
    }

    // 测试 8: 大数据集抽样
    {
        int n = 100000;
        for (int i = 1; i <= n; i++) st[i][0] = (i * 1234567LL + 78901) % (int)1e9;
        build(n);
        // 抽样测试1000个随机区间
        for (int t = 0; t < 1000; t++) {
            int l = ((t * 137 + 53) % n) + 1;
            int r = ((t * 271 + 97) % n) + 1;
            if (l > r) swap(l, r);
            assert(query(l, r) == brute_max(l, r));
        }
    }

    // === n=2 精确验证 ===
    {
        int n = 2;
        st[1][0] = 5; st[2][0] = 3;
        build(n);
        assert(query(1, 1) == 5);
        assert(query(2, 2) == 3);
        assert(query(1, 2) == 5);
    }

    // === n=3 精确验证 ===
    {
        int n = 3;
        st[1][0] = 5; st[2][0] = 9; st[3][0] = 2;
        build(n);
        assert(query(1, 1) == 5);
        assert(query(2, 2) == 9);
        assert(query(3, 3) == 2);
        assert(query(1, 2) == 9);
        assert(query(2, 3) == 9);
        assert(query(1, 3) == 9);
    }

    // === n=4 精确验证 ===
    {
        int n = 4;
        st[1][0] = 1; st[2][0] = 4; st[3][0] = 2; st[4][0] = 3;
        build(n);
        assert(query(1, 4) == 4);
        assert(query(2, 3) == 4);
        assert(query(1, 3) == 4);
        assert(query(2, 4) == 4);
    }

    cout << "ST_Table: ALL TESTS PASSED" << endl;
    return 0;
}
