#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define MAXN 5000

// 结构体表示一个雷达的位置与半径
typedef struct {
    int x, y, r;
} Radar;

Radar radars[MAXN];        // 原始雷达数据
int lowerIndex[MAXN];      // 存放下侧雷达的编号
int upperIndex[MAXN];      // 存放上侧雷达的编号
bool destroied[MAXN] = {0}; // 标记每个雷达是否被摧毁
int upCnt = 0, downCnt = 0, destroyCnt = 0;
int max_dx; // 上下侧雷达可能相交的最大x距离（通过几何计算得出）

// 判断两个雷达是否相交（也就是能否构成阻挡路径）
bool intersect(int i, int j) {
    Radar a = radars[i];
    Radar b = radars[j];
    return abs(a.x - b.x) <= max_dx &&
           (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) <= (a.r + b.r) * (a.r + b.r);
}

// 标记雷达被摧毁，并增加计数器
void markDestroy(int index) {
    destroied[index] = true;
    destroyCnt++;
}

// 压缩边结构：用两个 unsigned short 表示目标点编号 和 与上一个边的偏移量
// ✅ 使用 short 节省空间（4字节/边），比传统链式结构少一半空间
typedef struct {
    unsigned short v, next_delta;
} Edge;

Edge* edges;             // 所有边的数组（动态分配）
int head[MAXN] = {0};    // 每个点的链表头部指针（存的是最后一条边的编号）
int edgeCount = 0;       // 当前边的编号

// 插入一条 from → to 的边
void addEdge(int from, int to) {
    // ✅ 关键优化点：每当边数达到 65536，就插入一个 dummy 边（防止 delta 溢出为 0）
    // 因为 delta 不能为 0，否则链表会死循环
    if (!(++edgeCount & 0xffff))
        edges[edgeCount++] = (Edge){0};

    // 插入真实边：delta = 当前编号 - 旧的 head
    edges[edgeCount] = (Edge){to, edgeCount - head[from]};
    head[from] = edgeCount;
}

int match[MAXN];            // match[v] 表示下侧点 v 匹配到哪个上侧点 u
bool temp_visited[MAXN];    // 临时访问标记数组，用于 DFS

// 匈牙利算法：寻找一条增广路径
bool dfs_match(int u) {
    for (int i = head[u]; i & 0xffff; i -= edges[i].next_delta) {
        int v = edges[i].v;
        if (temp_visited[v]) continue;
        temp_visited[v] = true;
        if (match[v] == -1 || dfs_match(match[v])) {
            match[v] = u;
            return true;
        }
    }
    return false;
}

// 主函数：寻找最大匹配
void maxMatching() {
    memset(match, -1, sizeof(match));
    for (int u = 0; u < upCnt; u++) {
        memset(temp_visited, 0, sizeof(temp_visited));
        dfs_match(u);
    }
}

bool left_visited[MAXN] = {0};   // dfs_cover中标记上侧点是否访问
bool right_visited[MAXN] = {0};  // dfs_cover中标记下侧点是否访问

// 交错树搜索（用于构造最小顶点覆盖）
void dfs_cover(int u) {
    left_visited[u] = true;
    for (int i = head[u]; i & 0xffff; i -= edges[i].next_delta) {
        int v = edges[i].v;
        if (match[v] != u && !right_visited[v]) {
            right_visited[v] = true;
            if (match[v] != -1 && !left_visited[match[v]]) {
                dfs_cover(match[v]);
            }
        }
    }
}

// 构造最小顶点覆盖（König定理：最大匹配 = 最小覆盖）
// 最小顶点覆盖 = 未访问到的左侧点 ∪ 访问到的右侧点
void minCover() {
    bool leftMatch[MAXN] = {0};
    for (int v = 0; v < downCnt; v++) {
        if (match[v] != -1)
            leftMatch[match[v]] = true;
    }
    for (int u = 0; u < upCnt; u++) {
        if (!leftMatch[u])
            dfs_cover(u);
    }
}

int main() {
    int n, w, l;
    if (scanf("%d %d %d", &n, &w, &l) != 3) return 1;

    // 通过几何计算，任意两个雷达半径为w+100的情况下，x轴方向最远可能有 √3 × (w+100) 的交集
    max_dx = (int)(sqrt(3.0) * (w + 100)) + 1;

    // 读取雷达并分类
    for (int i = 0; i < n; i++) {
        int x, y, r;
        if (scanf("%d %d %d", &x, &y, &r) != 3) return 1;
        radars[i] = (Radar){x, y, r};

        if (y <= 100 && y + r < w + 100) {
            lowerIndex[downCnt++] = i;
        } else if (y >= w + 100 && y - r > 100) {
            upperIndex[upCnt++] = i;
        } else {
            // 雷达已经触碰到边界，必须摧毁
            markDestroy(i);
        }
    }

    // ✅ 合理内存分配：考虑稀疏图实际边数远小于 n²
    edges = malloc(sizeof(Edge) * n * n / 4);

    // 建图：如果上下雷达相交，则连边
    for (int i = 0; i < upCnt; i++) {
        for (int j = 0; j < downCnt; j++) {
            if (intersect(upperIndex[i], lowerIndex[j])) {
                addEdge(i, j);
            }
        }
    }

    maxMatching(); // 最大匹配
    minCover();    // 构造最小顶点覆盖
    free(edges);   // 释放动态边数组

    // 根据交错树访问结果构造最小删除集合
    for (int u = 0; u < upCnt; u++) {
        if (!left_visited[u])
            markDestroy(upperIndex[u]);
    }
    for (int v = 0; v < downCnt; v++) {
        if (right_visited[v])
            markDestroy(lowerIndex[v]);
    }

    // 输出结果
    printf("%d\n", destroyCnt);
    for (int i = 0; i < n; i++) {
        if (destroied[i]) {
            destroyCnt--;
            printf("%d%c", i + 1, destroyCnt ? ' ' : '\n');
        }
    }

    return 0;
}
