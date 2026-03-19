#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Edge {
    int to;
    int next;
} Edge;

Edge *edges;
int *head;
int edgeCount = 0;

void addEdge(int u, int v) {
    edges[++edgeCount] = (Edge){v, head[u]};
    head[u] = edgeCount;
    edges[++edgeCount] = (Edge){u, head[v]};
    head[v] = edgeCount;
}

int n, g, k;
int *piles;
int *childCount;

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
    if (factor * k > piles[g - 1]) return 0;

    if (!childCount[chamber]) return (find_first_ge(factor * (k + 1)) - find_first_ge(factor * k)) * k;
    
    unsigned long long sum = 0;
    for (int eid = head[chamber]; eid; eid = edges[eid].next) {
        int nxt = edges[eid].to;
        if (nxt == parent) continue;
        sum += dfs_calc_sum(nxt, chamber, factor * childCount[chamber]);
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

    edges = malloc(sizeof(Edge) * ((n - 2) * 2 + 1));
    head = malloc(sizeof(int) * (n + 1));
    memset(head, 0, sizeof(int) * (n + 1));

    childCount = malloc(sizeof(int) * (n + 1));
    memset(childCount, -1, sizeof(int) * (n + 1));

    int cam_u, cam_v;
    if (scanf("%d %d", &cam_u, &cam_v) != 2) return 1;
    childCount[cam_u] = childCount[cam_v] = 0;

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
