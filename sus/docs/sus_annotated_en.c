// Problem Overview:
// Given a graph with k key nodes and q queries, we want to determine the minimal cost 
// to send a signal from each query node to all key nodes. 
// The cost includes connecting to the nearest key node (access cost)
// and connecting all key nodes among themselves (propagation cost via MST).

// Core Algorithm:
// - k × Dijkstra to build shortest paths between key nodes
// - Prim's algorithm to find MST cost among key nodes
// - Multi-source Dijkstra to compute access cost for all nodes
// - Final answer = access cost + MST cost for each query

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100000   // Maximum number of nodes
#define MAXM 500000   // Maximum number of edges
#define MAXK 128      // Maximum number of key nodes

// Edge structure for forward star graph representation
typedef struct {
    int to, next;          // destination node and next edge index
    long long cost;        // weight of the edge
} Edge;

Edge edges[MAXM * 2 + 1];  // Edge list (undirected, so *2)
int head[MAXN];            // head[u] points to the first edge from node u
int edgeCnt;               // total number of edges

// Initialize graph
void initEdgeGraph(int n) {
    memset(head, -1, sizeof(head));
    edgeCnt = 0;
}

// Add one undirected edge (both directions)
void addEdge(int from, int to, long long cost) {
    edges[++edgeCnt] = (Edge){to, head[from], cost};
    head[from] = edgeCnt;
}

// Heap structure for Dijkstra and Prim
typedef struct {
    int u;                 // node id
    long long dist;        // current distance
} HeapNode;

HeapNode heap[MAXN * 2];   // Binary min-heap
int heap_size = 0;         // Current heap size

// Swap two heap elements
void static inline swap(HeapNode *a, HeapNode *b) {
    HeapNode tmp = *a;
    *a = *b;
    *b = tmp;
}

// Push a node into the min-heap
void push(HeapNode x) {
    heap[++heap_size] = x;
    for (int i = heap_size; i > 1 && heap[i].dist < heap[i >> 1].dist; i >>= 1) {
        swap(&heap[i], &heap[i >> 1]);
    }
}

// Pop the top (minimum) element from the heap
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

long long dist[MAXN];     // Distance array for Dijkstra
char visited[MAXN];       // Visited flags for Dijkstra

// Standard Dijkstra (supports multiple sources)
void dijkstra(int n, const int *src_nodes, int src_count) {
    memset(dist, 0x3f, sizeof(long long) * n);  // Initialize distances to INF
    memset(visited, 0, sizeof(char) * n);
    heap_size = 0;

    // Initialize sources
    for (int i = 0; i < src_count; i++) {
        int s = src_nodes[i];
        dist[s] = 0;
        push((HeapNode){s, 0});
    }

    // Main Dijkstra loop
    while (heap_size > 0) {
        HeapNode cur = pop();
        int u = cur.u;
        if (visited[u]) continue;
        visited[u] = 1;

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

long long kGraph[MAXK][MAXK];  // Shortest distances between key nodes

// Run k Dijkstra calls to build the complete graph between key nodes
void build_key_graph(int n, int k, int key_nodes[MAXK]) {
    for (int i = 0; i < k; i++) {
        dijkstra(n, &key_nodes[i], 1);
        for (int j = 0; j < k; j++) {
            kGraph[i][j] = dist[key_nodes[j]];
        }
    }
}

// Prim's algorithm to compute MST on the complete key-node graph
long long prim_mst(int k) {
    char visited[MAXK] = {0};
    long long total_cost = 0;
    heap_size = 0;

    for (int count = 1, u = 0; count < k; count++) {
        visited[u] = 1;
        for (int v = 0; v < k; v++) {
            if (!visited[v]) push((HeapNode){v, kGraph[u][v]});
        }
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
    
    int key_nodes[MAXK] = {0};  // Array of key node indices
    for (int i = 0; i < k; i++) {
        if (scanf("%d", &key_nodes[i]) != 1) return 1;
        key_nodes[i]--;
    }

    initEdgeGraph(n);  // Initialize graph
    for (int i = 0; i < m; i++) {
        int u, v;
        long long c;
        if (scanf("%d %d %lld", &u, &v, &c) != 3) return 1;
        u--; v--;
        addEdge(u, v, c);
        addEdge(v, u, c);
    }

    build_key_graph(n, k, key_nodes);  // Build key-node complete graph
    long long mst_cost = prim_mst(k);  // Compute MST cost among key nodes

    dijkstra(n, key_nodes, k);         // Multi-source Dijkstra from key nodes

    // Answer each query
    for (int i = 0; i < q; i++) {
        int query;
        if (scanf("%d", &query) != 1) return 1;
        printf("%lld\n", dist[query - 1] + mst_cost);
    }

    return 0;
}
