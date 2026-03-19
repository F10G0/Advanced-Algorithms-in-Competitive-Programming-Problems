#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 静态邻接表的边结构
typedef struct Edge {
    int to;
    int next;
} Edge;

Edge *edges; // 边数组
int *head; // 邻接表头
int edgeCount = 0;

// 添加无向边 u<->v
void addEdge(int u, int v) {
    // u -> v
    edges[++edgeCount] = (Edge){v, head[u]};
    head[u] = edgeCount;
    // v -> u
    edges[++edgeCount] = (Edge){u, head[v]};
    head[v] = edgeCount;
}

int n, g, k;
int *piles;
int *childCount;

// 二分查找：返回 piles 中第一个 >= val 的下标
int find_first_ge(unsigned long long val) {
    int l = 0, r = g;
    while (l < r) {
        int m = (l + r) >> 1;
        if (piles[m] < val) l = m + 1;
        else r = m;
    }
    return l;
}

unsigned long long dfs_calc_sum(int chamber, int parent, unsigned long long factor) {
    // 全局维护 piles[g-1] 为最大堆大小
    if (factor * k > piles[g - 1]) return; // 这个分支及其子孙再多叶子也不可能匹配任何堆

    // 除了父节点以外没有邻居（即没有子节点）时为叶子节点，即入口，计算所有符合放大倍率[k*factor, (k+1)*factor)下的玩具堆
    if (!childCount[chamber]) return (find_first_ge(factor * (k + 1)) - find_first_ge(factor * k)) * k;
    
    // 除调用此节点的父节点以外，剩下所有邻居皆为子节点，递归调用它们
    unsigned long long sum = 0;
    for (int eid = head[chamber]; eid; eid = edges[eid].next) {
        int nxt = edges[eid].to;
        if (nxt == parent) continue;
        sum += dfs_calc_sum(nxt, chamber, factor * childCount[chamber]); //倍率放大子节点数量
    }
    return sum;
}

int cmp_int(const void *a, const void *b) {
    return *(const int*)a - *(const int*)b;
}

int main() {
    if (scanf("%d %d %d", &n, &g, &k) != 3) return 1;

    piles = malloc(sizeof(int) * g);
    for (int i = 0; i < g; i++) {
        if (scanf("%d", piles + i) != 1) return 1;
    }
    qsort(piles, g, sizeof(int), cmp_int);

    edges = malloc(sizeof(Edge) * ((n - 2) * 2 + 1)); // 为 n-2 条非监控边分配空间，两个方向各添加一次，edges的索引是1-based
    head = malloc(sizeof(int) * (n + 1));
    memset(head, 0, sizeof(int) * (n + 1));

    childCount = malloc(sizeof(int) * (n + 1));
    memset(childCount, -1, sizeof(int) * (n + 1)); //子节点为除了父节点以外的邻居节点，数量为邻居数量-1（根节点除外）
    int cam_u, cam_v;
    if (scanf("%d %d", &cam_u, &cam_v) != 2) return 1; // 切断监控边，将两端视为两棵子树的根
    childCount[cam_u] = childCount[cam_v] = 0;

    // 读入其余边并构建邻接表，同时统计 childCount
    for (int i = 2; i < n; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) return 1;
        addEdge(u, v);
        childCount[u]++;
        childCount[v]++;
    }

    printf("%llu\n", dfs_calc_sum(cam_u, 0, 1) + dfs_calc_sum(cam_v, 0, 1));

    return 0;
}
