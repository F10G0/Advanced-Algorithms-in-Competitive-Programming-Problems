// 题目概述：
// 在一个无向图中，有 k 个关键节点，q 个查询点。每次从某个查询点出发，需要最小代价将信号传递到所有关键节点。

// 解法核心：
// 本题的关键在于：
// - 查询点到某个关键节点的最短路径（接入代价）
// - 关键节点之间形成的最小连接网络（传播代价）

// 解决方案如下：
// 1. 对每个关键节点，单独跑一遍 Dijkstra，求出它到所有关键节点的最短路径 → 构建一个 k*k 的完全图。
// 2. 在这个完全图上跑 Prim 算法，求出最小生成树（MST）代价，表示所有关键节点间的最小传播代价。
// 3. 所有关键节点作为起点，跑一次多源 Dijkstra，求出所有点到最近关键节点的距离（接入代价）。
// 4. 每次查询点的答案为：接入代价 + 传播代价。

// 算法复杂度：
// - k 次 Dijkstra：O(k * m log n)
// - Prim 建 MST：O(k^2 log k)（实际约 128²）
// - 多源 Dijkstra：O(m log n)
// - 总体复杂度极优，适合大图稀疏结构。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100000   // 最大点数（原图节点数）
#define MAXM 500000   // 最大边数
#define MAXK 128      // 最大关键节点数

// 图的边结构（链式前向星）
typedef struct {
    int to;             // 目标节点编号
    int next;           // 下一条边在 edges[] 中的索引
    long long cost;     // 边的权重（边的代价）
} Edge;

Edge edges[MAXM * 2 + 1]; // 边数组，注意无向图每条边存两次
int head[MAXN];           // head[u] 指向节点 u 的第一条边
int edgeCnt;              // 当前边的总数（从 1 开始递增）

// 初始化图
void initEdgeGraph(int n) {
    memset(head, -1, sizeof(head));
    edgeCnt = 0;
}

// 插入一条边到图中（链式前向星）
void addEdge(int from, int to, long long cost) {
    edges[++edgeCnt] = (Edge){to, head[from], cost};
    head[from] = edgeCnt;
}

// 小根堆结点（用于 Dijkstra / Prim）
typedef struct {
    int u;              // 当前节点编号
    long long dist;     // 到起点的估计距离
} HeapNode;

HeapNode heap[MAXN * 2]; // 堆数组
int heap_size = 0;       // 当前堆大小

// 交换两个堆结点
void static inline swap(HeapNode *a, HeapNode *b) {
    HeapNode tmp = *a;
    *a = *b;
    *b = tmp;
}

// 向堆中插入一个节点（上滤）
void push(HeapNode x) {
    heap[++heap_size] = x;
    for (int i = heap_size; i > 1 && heap[i].dist < heap[i >> 1].dist; i >>= 1) {
        swap(&heap[i], &heap[i >> 1]);
    }
}

// 弹出堆顶（最小值）结点（下滤）
HeapNode pop() {
    HeapNode top = heap[1];
    heap[1] = heap[heap_size--];

    int i = 1;
    while (1) {
        int l = i * 2, r = i * 2 + 1, next = i;
        if (l <= heap_size && heap[l].dist < heap[next].dist) next = l;
        if (r <= heap_size && heap[r].dist < heap[next].dist) next = r;
        if (next == i) break;
        swap(&heap[i], &heap[next]);
        i = next;
    }
    return top;
}

long long dist[MAXN];   // 单/多源 Dijkstra 的距离数组
char visited[MAXN];     // 访问标记（防止重复 pop）

// Dijkstra 算法（支持多源）
void dijkstra(int n, const int *src_nodes, int src_count) {
    memset(dist, 0x3f, sizeof(long long) * n);  // 初始化为 INF
    memset(visited, 0, sizeof(char) * n);
    heap_size = 0;

    // 所有源点入堆
    for (int i = 0; i < src_count; i++) {
        int s = src_nodes[i];
        dist[s] = 0;
        push((HeapNode){s, 0});
    }

    // 主循环
    while (heap_size > 0) {
        HeapNode cur = pop();
        int u = cur.u;
        if (visited[u]) continue;
        visited[u] = 1;

        // 松弛邻接边
        for (int i = head[u]; i != -1; i = edges[i].next) {
            int v = edges[i].to;
            long long cost = edges[i].cost;
            if (dist[v] > dist[u] + cost) {
                dist[v] = dist[u] + cost;
                push((HeapNode){v, dist[v]});
            }
        }
    }
}

long long kGraph[MAXK][MAXK];  // 关键点之间的最短路径（完全图）

// 构建关键点之间的最短路径图（k*k 矩阵）
void build_key_graph(int n, int k, int key_nodes[MAXK]) {
    for (int i = 0; i < k; i++) {
        dijkstra(n, &key_nodes[i], 1); // 从第 i 个关键点出发
        for (int j = 0; j < k; j++) {
            kGraph[i][j] = dist[key_nodes[j]];
        }
    }
}

// Prim 算法在关键点之间构建最小生成树（用于信号传播）
long long prim_mst(int k) {
    char visited[MAXK] = {0};
    long long total_cost = 0;
    heap_size = 0;

    for (int count = 1, u = 0; count < k; count++) {
        visited[u] = 1;

        // 将所有 u 的边插入堆
        for (int v = 0; v < k; v++) {
            if (!visited[v]) push((HeapNode){v, kGraph[u][v]});
        }

        // 选出下一条连接未访问节点的最小边
        while (heap_size > 0) {
            HeapNode edge = pop();
            if (!visited[edge.u]) {
                u = edge.u;
                total_cost += edge.dist;
                break;
            }
        }
    }
    return total_cost;
}

int main() {
    int n, m, k, q;
    if (scanf("%d %d %d %d", &n, &m, &k, &q) != 4) return 1;
    
    int key_nodes[MAXK] = {0};  // 关键节点数组
    for (int i = 0; i < k; i++) {
        if (scanf("%d", &key_nodes[i]) != 1) return 1;
        key_nodes[i]--;
    }

    // 构建图
    initEdgeGraph(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        long long c;
        if (scanf("%d %d %lld", &u, &v, &c) != 3) return 1;
        u--; v--;
        addEdge(u, v, c);
        addEdge(v, u, c);
    }

    // 构建关键点间距离图 + Prim 求最小传播代价
    build_key_graph(n, k, key_nodes);
    long long mst_cost = prim_mst(k);

    // 多源 Dijkstra 求接入代价（所有点到最近关键点）
    dijkstra(n, key_nodes, k);

    // 回答每个查询
    for (int i = 0; i < q; i++) {
        int query;
        if (scanf("%d", &query) != 1) return 1;
        printf("%lld\n", dist[query - 1] + mst_cost);
    }

    return 0;
}
