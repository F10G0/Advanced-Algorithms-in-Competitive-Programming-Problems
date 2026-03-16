#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXN 200000
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))

typedef struct {
    int x, y;
} Point;
Point parks[MAXN];

// Compare indices by x-coordinate
int cmp_x(const void *a, const void *b) {
    return parks[*(const int *)a].x - parks[*(const int *)b].x;
}

// Compare indices by y-coordinate
int cmp_y(const void *a, const void *b) {
    return parks[*(const int *)a].y - parks[*(const int *)b].y;
}

typedef struct {
    int v;     // node index
    int dist;  // current best distance
} HNode;

HNode heap[MAXN << 3];
int hsize = 0;

// Push a new element onto the min-heap
void heap_push(HNode n) {
    int i = ++hsize;
    while (i > 1 && heap[i >> 1].dist > n.dist) {
        heap[i] = heap[i >> 1];
        i >>= 1;
    }
    heap[i] = n;
}

// Pop the smallest element from the min-heap
HNode heap_pop() {
    HNode ret = heap[1], last = heap[hsize--];
    int i = 1;
    while (1) {
        int c = i << 1;
        if (c > hsize) break;
        if (c < hsize && heap[c + 1].dist < heap[c].dist) c++;
        if (last.dist <= heap[c].dist) break;
        heap[i] = heap[c];
        i = c;
    }
    heap[i] = last;
    return ret;
}

int dist[MAXN];

// Relax edge from u to v with appropriate cost
void relax(int u, int v) {
    int d = dist[u]
          + MIN(ABS(parks[u].x - parks[v].x),
                ABS(parks[u].y - parks[v].y));
    if (d < dist[v]) {
        dist[v] = d;
        heap_push((HNode){ v, d });
    }
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 1;

    // Read in park coordinates
    for (int i = 0; i < n; i++) {
        if (scanf("%d %d", &parks[i].x, &parks[i].y) != 2) return 1;
    }

    // Prepare arrays for sorting by x and by y
    int sorted_x[MAXN], sorted_y[MAXN];
    for (int i = 0; i < n; i++) {
        sorted_x[i] = sorted_y[i] = i;
    }
    // Sort indices by their x and y values
    qsort(sorted_x, n, sizeof(int), cmp_x);
    qsort(sorted_y, n, sizeof(int), cmp_y);

    // Build inverse maps: pos_x[u] gives u's position in sorted_x[]
    int pos_x[MAXN], pos_y[MAXN];
    for (int i = 0; i < n; i++) {
        pos_x[sorted_x[i]] = i;
        pos_y[sorted_y[i]] = i;
    }

    // Track visited nodes in Dijkstra
    char visited[MAXN] = {0};

    // Initialize distances to "infinity"
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
    }
    dist[0] = 0;
    heap_push((HNode){ 0, 0 });

    // Main Dijkstra loop
    while (hsize) {
        HNode cur = heap_pop();
        int v = cur.v;
        if (visited[v]) continue;
        visited[v] = 1;
        if (v == n - 1) break;  // stop when reaching target

        // Relax neighbor in sorted_x (left/right in x-order)
        int xi = pos_x[v];
        if (xi > 0)     relax(v, sorted_x[xi - 1]);
        if (xi < n - 1) relax(v, sorted_x[xi + 1]);

        // Relax neighbor in sorted_y (up/down in y-order)
        int yi = pos_y[v];
        if (yi > 0)     relax(v, sorted_y[yi - 1]);
        if (yi < n - 1) relax(v, sorted_y[yi + 1]);
    }

    // Print minimal stroll distance from park 1 to park n
    printf("%d\n", dist[n-1]);
    return 0;
}
