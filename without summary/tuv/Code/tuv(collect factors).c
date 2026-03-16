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
unsigned long long *factors;
int fac_idx = 0;

// Depth-first search to collect branch factors at each leaf
// 'parent' is the previous node; 'factor' is the product of branch counts along the path
void dfs_calc_facs(int chamber, int parent, unsigned long long factor) {
    // Prune entire branch if smallest possible pile exceeds maximum pile size
    if (factor * k > piles[g - 1]) return;

    // Leaf entrance: no remaining branches at this node
    // Record the accumulated factor for later grouping
    if (!childCount[chamber]) factors[fac_idx++] = factor;
    
    // Recurse into each non-parent neighbor, scaling the factor by the branching count
    for (int eid = head[chamber]; eid; eid = edges[eid].next) {
        int nxt = edges[eid].to;
        if (nxt == parent) continue;
        dfs_calc_facs(nxt, chamber, factor * childCount[chamber]);
    }
}

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

// Comparison for qsort
int cmp_int(const void *a, const void *b) {
    return *(const int*)a - *(const int*)b;
}

int cmp_ull(const void *a, const void *b) {
    return *(const unsigned long long*)a - *(const unsigned long long*)b;
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

    // Allocate and zero factor arrays to collect leaf factors
    factors = malloc(sizeof(unsigned long long) * n * 2);
    memset(factors, 0, sizeof(unsigned long long) * n * 2);

    // Perform DFS on both subtrees to collect factors
    dfs_calc_facs(cam_u, 0, 1);
    dfs_calc_facs(cam_v, 0, 1);

    // Sum contributions: for each unique factor, binary-search matching piles
    unsigned long long sum = 0;
    qsort(factors, fac_idx, sizeof(unsigned long long), cmp_ull);
    for (int i = 0; i < fac_idx;) {
        unsigned long long fac = factors[i];
        unsigned long long amount = (find_first_ge(fac * (k + 1)) - find_first_ge(fac * k)) * k;
        while (factors[i] == fac) {
            sum += amount;
            i++;
        }
    }

    // Compute and print the total captured toys from both subtrees
    printf("%llu\n", sum);
    return 0;
}
