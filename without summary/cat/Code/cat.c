#include <stdio.h>
#include <stdlib.h>

#define MAXN 200000
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))

typedef struct {
    int x, y;
} Point;
Point parks[MAXN];

int cmp_x(const void *a, const void *b) {
    return parks[*(const int *)a].x - parks[*(const int *)b].x;
}

int cmp_y(const void *a, const void *b) {
    return parks[*(const int *)a].y - parks[*(const int *)b].y;
}

typedef struct {
    int v;
    int dist;
} HNode;

HNode heap[MAXN << 3];
int hsize = 0;

void heap_push(HNode n) {
    int i = ++hsize;
    while (i > 1 && heap[i >> 1].dist > n.dist) {
        heap[i] = heap[i >> 1];
        i >>= 1;
    }
    heap[i] = n;
}

HNode heap_pop() {
    HNode ret = heap[1], last = heap[hsize--];
    int i = 2;
    while (i <= hsize) {
        if (i < hsize && heap[i + 1].dist < heap[i].dist) i++;
        if (last.dist <= heap[i].dist) break;
        heap[i >> 1] = heap[i];
        i <<= 1;
    }
    heap[i >> 1] = last;
    return ret;
}

int dist[MAXN];

void relax(int u, int v) {
    int d = dist[u] + MIN(ABS(parks[u].x - parks[v].x), ABS(parks[u].y - parks[v].y));
    if (d < dist[v]) {
        dist[v] = d;
        heap_push((HNode){ v, d });
    }
}

int main() {
    int n;
    if(scanf("%d", &n) != 1) return 1;
    for (int i = 0; i < n; i++) {
        if(scanf("%d %d", &parks[i].x, &parks[i].y) != 2) return 1;
    }

    int sorted_x[MAXN], sorted_y[MAXN];
    for (int i = 0; i < n; i++) {
        sorted_x[i] = sorted_y[i] = i;
    }
    qsort(sorted_x, n, sizeof(int), cmp_x);
    qsort(sorted_y, n, sizeof(int), cmp_y);

    int pos_x[MAXN], pos_y[MAXN];
    for(int i = 0; i < n; i++){
        pos_x[sorted_x[i]] = i;
        pos_y[sorted_y[i]] = i;
    }

    char visited[MAXN] = {0};

    for(int i = 0; i < n; i++){
        dist[i] = 0x7fffffff;
    }
    dist[0] = 0;
    heap_push((HNode){ 0, 0 });

    while (hsize) {
        HNode cur = heap_pop();
        int v = cur.v;
        if(visited[v]) continue;
        visited[v] = 1;
        if(v == n - 1) break;

        int x = pos_x[v];
        if(x > 0) relax(v, sorted_x[x - 1]);
        if(x < n - 1) relax(v, sorted_x[x + 1]);

        int y = pos_y[v];
        if(y > 0) relax(v, sorted_y[y - 1]);
        if(y < n - 1) relax(v, sorted_y[y + 1]);
    }

    printf("%d\n", dist[n-1]);

    return 0;
}
