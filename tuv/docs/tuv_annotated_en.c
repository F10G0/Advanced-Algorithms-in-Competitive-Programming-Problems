#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Static adjacency list edge structure
// Each edge stores the target node and the index of the next edge in the list
typedef struct Edge {
    int to;
    int next;
} Edge;

Edge *edges; // Array of edges
int *head; // Head pointers for adjacency lists
int edgeCount = 0;

// Add an undirected edge between u and v
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

// Binary search: return the first index in piles[] where value >= val
int find_first_ge(unsigned long long val) {
    int l = 0, r = g;
    while (l < r) {
        int m = (l + r) >> 1;
        if (piles[m] < val) l = m + 1;
        else r = m;
    }
    return l;
}

// Depth-first calculation of total toys captured from subtree rooted at 'chamber'
// 'parent' is the previous node; 'factor' is the product of branch counts along the path
unsigned long long dfs_calc_sum(int chamber, int parent, unsigned long long factor) {
    // Prune entire branch if smallest possible pile exceeds maximum pile size
    if (factor * k > piles[g - 1]) return 0;

    // If this node has no children, it is a leaf entrance
    // Count piles with size in [factor*k, factor*(k+1))
    if (!childCount[chamber]) return (find_first_ge(factor * (k + 1)) - find_first_ge(factor * k)) * k;
    
    // Otherwise, recurse into each child (neighbors except parent)
    unsigned long long sum = 0;
    for (int eid = head[chamber]; eid; eid = edges[eid].next) {
        int nxt = edges[eid].to;
        if (nxt == parent) continue;
        sum += dfs_calc_sum(nxt, chamber, factor * childCount[chamber]);
    }
    return sum;
}

// Comparison for qsort
int cmp_int(const void *a, const void *b) {
    return *(const int*)a - *(const int*)b;
}

int main() {
    if (scanf("%d %d %d", &n, &g, &k) != 3) return 1;

    // Read piles and sort
    piles = malloc(sizeof(int) * g);
    for (int i = 0; i < g; i++) {
        if (scanf("%d", piles + i) != 1) return 1;
    }
    qsort(piles, g, sizeof(int), cmp_int);

    // Allocate space for the non-camera edges (n-2 edges, each stored in two directions)
    edges = malloc(sizeof(Edge) * ((n - 2) * 2 + 1));
    head = malloc(sizeof(int) * (n + 1));
    memset(head, 0, sizeof(int) * (n + 1));

    // childCount[i] = degree(i) - 1, initialized to -1 and then incremented per edge
    childCount = malloc(sizeof(int) * (n + 1));
    memset(childCount, -1, sizeof(int) * (n + 1));

    // Read the camera edge and split the tree into two rooted subtrees
    int cam_u, cam_v;
    if (scanf("%d %d", &cam_u, &cam_v) != 2) return 1;
    childCount[cam_u] = childCount[cam_v] = 0;

    // Read the remaining edges, build adjacency lists, and update childCount
    for (int i = 2; i < n; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) return 1;
        addEdge(u, v);
        childCount[u]++;
        childCount[v]++;
    }

    // Compute and print the total captured toys from both subtrees
    printf("%llu\n", dfs_calc_sum(cam_u, 0, 1) + dfs_calc_sum(cam_v, 0, 1));
    return 0;
}
