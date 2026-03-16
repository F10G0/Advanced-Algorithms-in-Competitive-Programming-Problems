#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

#define MAXN 500000
#define MAXM 1000000
#define MOD 1000000007
#define MIN(a, b) ( (a) < (b) ? (a) : (b) )

// Print MEOW-NO and exit immediately
void exit_meow_no() {
    printf("MEOW-NO\n");
    exit(0);
}

typedef struct Edge {
    int to;       // destination vertex
    int next;     // index of next edge in adjacency list
} Edge;

Edge *edges;     // edge pool (2 * M entries)
int *head;       // head[u] = index of first edge from u
int edgeCount = 0;

// Add undirected edge (u <-> v)
void addEdge(int u, int v) {
    edges[++edgeCount] = (Edge){v, head[u]};
    head[u] = edgeCount;
    edges[++edgeCount] = (Edge){u, head[v]};
    head[v] = edgeCount;
}

int *ids, id = 0;    // discovery times
int *low;            // low-link values
int *stack, stackTop = 0;  // stack of edge indices

int cycleLen, cycleCnt = 0;  // global cycle length and count
char inBCC[MAXN + 1] = {0};  // mark vertices in current BCC
int BCC[MAXN];               // list of vertices in BCC

int E = 0, V = 0;
// Process one Biconnected Component given by stopping at edge 'stop'
void processBCC(int stop) {
    // clear previous inBCC marks
    for (int i = 0; i < V; i++) {
        inBCC[BCC[i]] = 0;
    }

    // reinitialize E (edges) and V (vertices) count
    E = 0, V = 0;
    int e;
    // pop edges until we reach 'stop', collect vertices
    do {
        e = stack[--stackTop];
        E++;
        int u = edges[e].to;
        int v = edges[((e - 1) ^ 1) + 1].to;
        if (!inBCC[u]) {
            inBCC[u] = 1;
            BCC[V++] = u;
        }
        if (!inBCC[v]) {
            inBCC[v] = 1;
            BCC[V++] = v;
        }
    } while (e != stop);

    // trivial component of one edge -> skip
    if (E == 1) return;

    // simple cycle: edges == vertices
    if (E == V) {
        if (!cycleLen) cycleLen = E;
        else if (E != cycleLen) exit_meow_no();
        cycleCnt++;
        return;
    }

    // identify two hubs with degree > 2 in this BCC
    int hCnt = 0, s = 0, t = 0;
    for (int i = 0; i < V; i++){
        int u = BCC[i], d = 0;
        for (int e = head[u]; e; e = edges[e].next) {
            int v = edges[e].to;
            if (inBCC[v]) d++;
        }
        if (d > 2) {
            if (hCnt == 0) s = u;
            else if (hCnt == 1) t = u;
            hCnt++;
        } else if (d != 2) exit_meow_no();
    }
    if (hCnt != 2) exit_meow_no();

    // collect all path lengths from s to t
    int len = 0, lCnt = 0;
    for (int e = head[s]; e; e = edges[e].next) {
        int cur = edges[e].to;
        if (!inBCC[cur]) continue;
        int prev = s, cur_len = 1;
        // follow unique path until t
        while (cur != t) {
            int nxt;
            // scan neighbors of cur to find the next step
            for (int f = head[cur]; f; f = edges[f].next) {
                int w = edges[f].to;
                if (inBCC[w] && w != prev) {
                    nxt = w;
                    break;
                }
            }
            prev = cur;
            cur = nxt;
            cur_len++;
        }
        // ensure all paths have same length
        if (!len) len = cur_len;
        else if (cur_len != len) exit_meow_no();
        lCnt++;
    }

    // enforce a uniform cycle length
    if (!cycleLen) cycleLen = len * 2;
    else if (len * 2 != cycleLen) exit_meow_no();
    // add the number of simple cycles in a theta structure: C(lCnt, 2)
    cycleCnt = (cycleCnt + (long long)lCnt * (lCnt - 1) / 2) % MOD;
}

// Tarjan DFS to find BCCs
void dfs(int u, int p) {
    ids[u] = low[u] = ++id;
    for (int e = head[u]; e; e = edges[e].next) {
        int v = edges[e].to;
        if (v == p) continue;  // skip edge back to parent
        if (!ids[v]) {
            stack[stackTop++] = e;  // push tree edge
            dfs(v, u);
            low[u] = MIN(low[u], low[v]);
            if (low[v] >= ids[u]) processBCC(e);
        } else if (ids[v] < ids[u]) {
            // back edge
            stack[stackTop++] = e;
            low[u] = MIN(low[u], ids[v]);
        }
    }
}

int main() {
    // increase stack limit for deep recursion
    struct rlimit rl;
    if (getrlimit(RLIMIT_STACK, &rl) == 0) {
        rl.rlim_cur = rl.rlim_max;
        setrlimit(RLIMIT_STACK, &rl);
    }
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1;

    // allocate graph structures
    edges = malloc(sizeof(Edge) * (m * 2 + 1));
    head  = malloc(sizeof(int) * (n + 1));
    ids   = malloc(sizeof(int) * (n + 1));
    low   = malloc(sizeof(int) * (n + 1));
    stack = malloc(sizeof(int) * (m * 2));

    // initialize arrays
    memset(head, 0, sizeof(int) * (n + 1));
    memset(ids,  0, sizeof(int) * (n + 1));
    memset(low,  0, sizeof(int) * (n + 1));

    // read edges
    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        addEdge(u, v);
    }

    // run Tarjan on each component
    for (int i = 1; i <= n; i++) {
        if (!ids[i]) dfs(i, 0);
    }

    // output result
    if (cycleCnt == 0) printf("FUR-LORN\n");
    else {
        long long total = (2LL * cycleLen % MOD) * cycleCnt % MOD;
        printf("PURR-FECT\n%d %lld\n", cycleLen, total);
    }
    return 0;
}
