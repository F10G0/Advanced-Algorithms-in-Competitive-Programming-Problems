#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXN 250001

struct Edge {
    int u, v;
};

int parent[MAXN];
int falltime[MAXN];

// Find with path compression and propagate the falltime of the original parent
int find(int x) {
    if (parent[x] != x) {
        int p = parent[x];
        parent[x] = find(p);
        // Propagate the falltime from the original parent (p) down to x,
        // so that during path compression, x inherits the time at which
        // its branch first reconnected to node 1.
        if (p != 1) 
            falltime[x] = falltime[p];
    }
    return parent[x];
}

// Union by simple rank (by index)
bool unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a > b) 
        parent[a] = b;
    else if (a < b) 
        parent[b] = a;
    return a == b;
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1;

    // Read initial paw connections for cats 2..n
    int paws[MAXN][4];
    for (int i = 2; i <= n; i++) {
        scanf("%d %d %d %d", paws[i], paws[i] + 1, paws[i] + 2, paws[i] + 3);
    }

    // Record each “claw release” event as an edge to be added in reverse
    struct Edge* events = malloc(sizeof(struct Edge) * (m + 1));
    for (int i = 1; i <= m; i++) {
        int v, e;
        scanf("%d %d", &v, &e);
        events[i] = (struct Edge){ v, paws[v][e - 1] };
        // Mark this claw as released
        paws[v][e - 1] = -1;
    }

    // Initialize DSU and set all falltimes to -1 (meaning “not fallen”)
    for (int i = 0; i <= n; i++) {
        parent[i] = i;
        falltime[i] = -1;
    }

    // Build the DSU for the final state: unite all edges that never get deleted
    for (int i = 2; i <= n; i++) {
        for (int j = 0; j < 4; j++) {
            if (paws[i][j] != -1)
                unite(i, paws[i][j]);
        }
    }

    // Process events in reverse: re-add each released claw
    for (int t = m; t >= 1; t--) {
        int u = events[t].u, v = events[t].v;

        // If u’s component was disconnected from 1 but v’s was connected,
        // then u’s root is the node that first rejoins 1 at time t.
        if (find(u) != 1 && find(v) == 1)
            // Record t on u’s root (after compression) so that
            // all nodes in that branch will inherit it when compressed.
            falltime[parent[u]] = t;  

        // Vice versa for v
        if (find(v) != 1 && find(u) == 1)
            // Same logic: v’s branch first reconnects at time t.
            falltime[parent[v]] = t;  

        // Finally, unite the two components
        unite(u, v);
    }

    // Output each cat’s fall time: compress paths one last time to update falltime
    for (int i = 2; i <= n; i++) {
        find(i);
        printf("%d\n", falltime[i]);
    }

    return 0;
}
