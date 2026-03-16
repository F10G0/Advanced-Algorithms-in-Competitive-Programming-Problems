#include <stdio.h>
#include <stdlib.h>

// Use 64-bit integers to encode undirected edges as (u<<32)|v
typedef unsigned long long ull;
ull *pairs;
// Compare two encoded edges for sorting and deduplication
int cmp_ull(const void *a, const void*b) {
    return *(ull *)a - *(ull *)b;
}

typedef struct {
    int to;
    int next;
} Edge;

Edge *edges;
int *head, ecnt = 0;
int *deg;

// Add undirected edge (u, v) to adjacency lists and update degrees
void add_edge(int u, int v) {
    edges[++ecnt] = (Edge){v, head[u]};
    head[u] = ecnt;
    deg[u]++;
    edges[++ecnt] = (Edge){u, head[v]};
    head[v] = ecnt;
    deg[v]++;
}

int *MVC;      // Marks vertices in the current vertex cover (0=not chosen, 1=kernel, 2=branch)
int kcnt = 0;  // Number of kernel edges stored in pairs[]

// Recursive check: can we cover all kernel edges with <= sz additional vertices?
char check_vc(int sz) {
    if (sz < 0) return 0;  // Budget exhausted
    for (int i = 0; i < kcnt; i++) {
        int u = pairs[i] >> 32, v = pairs[i] & 0xffffffff;
        if (MVC[u] || MVC[v]) continue;  // Edge already covered

        // Branch: choose u
        MVC[u] = 2;
        if (check_vc(sz - 1)) return 1;
        MVC[u] = 0;
        
        // Branch: choose v
        MVC[v] = 2;
        if (check_vc(sz - 1)) return 1;
        MVC[v] = 0;

        return 0;  // Neither branch worked for this edge
    }
    return 1;  // All edges covered
}

// Clean up branch markers, keep only kernel-selected vertices
void clean_MVC(int n) {
    for (int i = 1; i <= n; i++) {
        MVC[i] &= 1;
    }
}

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 1;

    int m;
    if (scanf("%d", &m) != 1) return 1;

    // Read and normalize m directed edges into pairs[]
    pairs = malloc(sizeof(ull) * m);
    for (int i = 0; i < m; i++) {
        int a, b;
        if (scanf("%d %d", &a, &b) != 2) return 1;
        if (b < a) {
            int t = a;
            a = b;
            b = t;
        }
        pairs[i] = (ull)a << 32 | b;
    }
    // Sort and deduplicate to build a simple undirected graph
    qsort(pairs, m, sizeof(ull), cmp_ull);

    // Allocate graph structures
    edges = malloc(sizeof(Edge) * (m << 1 | 1));
    head = malloc(sizeof(int) * (n + 1));
    deg = malloc(sizeof(int) * (n + 1));
    for (int i = 0; i < m; i++) {
        if (!i || pairs[i] != pairs[i - 1]) add_edge(pairs[i] >> 32, pairs[i] & 0xffffffff);
    }
    
    // Allocate MVC array and stack for kernelization
    MVC = malloc(sizeof(int) * (n + 1));
    int *stack = malloc(sizeof(int) * n), sp = 0;

    // Initial vertex cover size budget: choose n-k vertices
    int vc_size = n - k;
    // Kernelization: force include all vertices with degree > vc_size
    for (int i = 1; i <= n; i++) {
        if (deg[i] > vc_size) stack[sp++] = i;
    }
    while (sp) {
        int u = stack[--sp];
        if (deg[u] <= vc_size) continue;
        MVC[u] = 1;  // Mark kernel cover
        vc_size--;

        // Decrement neighbor degrees, push newly over-budget vertices
        for (int e = head[u]; e; e = edges[e].next) {
            int v = edges[e].to;
            if (--deg[v] == vc_size) stack[sp++] = v;
        }
        deg[u] = 0;  // Remove u from graph
    }
    k += vc_size;  // Adjust total cover size

    // Build kernel edge list back in pairs[], count in kcnt
    for (int u = 1; u <= n; u++) {
        if (MVC[u]) continue;
        for (int e = head[u]; e; e = edges[e].next) {
            int v = edges[e].to;
            if (u < v && !MVC[v]) {
                pairs[kcnt++] = (ull)u << 32 | v;
                if (kcnt > vc_size * vc_size) {
                    puts("CATASTROPHE");
                    return 0;
                }
            }
        }
    }

    // FPT branching to check if remaining kernel edges can be covered
    if (!check_vc(vc_size)) {
        puts("CATASTROPHE");
        return 0;
    }
    // Clean branch marks, keep only kernel selections
    clean_MVC(n);
    // Iteratively reduce cover size while still feasible
    while (check_vc(--vc_size)) {
        clean_MVC(n);
    }

    // Final check to restore correct branch marks
    check_vc(++vc_size);
    // Output size and list of vertices in maximum independent set
    printf("%d\n", k - vc_size);
    for (int i = 1; i <= n; i++) {
        if (!MVC[i]) printf("%d ", i);
    }
    
    return 0;
}
