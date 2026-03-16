#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

#define MAXN 500000
#define MAXM 1000000
#define MOD 1000000007
#define MIN(a, b) ( (a) < (b) ? (a) : (b) )

typedef struct Edge {
    int to;
    int next;
} Edge;

void addEdge(int u, int v, Edge *edges, int *head, int *edgeCount) {
    edges[++*edgeCount] = (Edge){v, head[u]};
    head[u] = *edgeCount;
}

Edge *raw_edges;
int *raw_head;
int raw_edgeCount = 0;

int *ids, id = 1;
int *low;
char *onStack;
int *stack, stackTop = 0;

int *scc_ids, scc_cnt = 0;

void scc_dfs(int u) {
    stack[stackTop++] = u;
    onStack[u] = 1;
    ids[u] = low[u] = id++;

    for (int e = raw_head[u]; e; e = raw_edges[e].next) {
        int v = raw_edges[e].to;
        if (!ids[v]) {
            scc_dfs(v);
            low[u] = MIN(low[u], low[v]);
        } else if (onStack[v]) {
            low[u] = MIN(low[u], ids[v]);
        }
    }

    if (ids[u] == low[u]) {
        ++scc_cnt;
        int cur;
        do {
            cur = stack[--stackTop];
            onStack[cur] = 0;
            scc_ids[cur] = scc_cnt;
        } while (cur != u);
    }
}

typedef struct {
    int u, v;
} SCCEdge;
SCCEdge *scc_edges_arr;
int scc_edge_arr_cnt = 0;

int cmp_scc_edges_arr(const void *a, const void *b) {
    SCCEdge *A = (SCCEdge *)a, *B = (SCCEdge *)b;
    if (A->u != B->u) return A->u - B->u;
    return A->v - B->v;
}

Edge *scc_edges;
int *scc_head;

int *topo, topo_cnt;
char *topo_visited;

void topo_dfs(int u) {
    topo_visited[u] = 1;
    for (int e = scc_head[u]; e; e = scc_edges[e].next) {
        int v = scc_edges[e].to;
        if (!topo_visited[v]) topo_dfs(v);
    }
    topo[--topo_cnt] = u;
}

int main() {
    struct rlimit rl;
    if (getrlimit(RLIMIT_STACK, &rl) == 0) {
        rl.rlim_cur = rl.rlim_max;
        setrlimit(RLIMIT_STACK, &rl);
    }

    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1;

    raw_edges = malloc(sizeof(Edge) * (m + 1));
    raw_head = malloc(sizeof(int) * (n + 1));

    for (int i = 0; i < m; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) return 1;
        addEdge(u, v, raw_edges, raw_head, &raw_edgeCount);
    }

    ids = malloc(sizeof(int) * (n + 1));
    low = malloc(sizeof(int) * (n + 1));
    onStack = malloc(sizeof(char) * (n + 1));
    stack = malloc(sizeof(int) * n);
    scc_ids = malloc(sizeof(int) * (n + 1));

    for (int i = 1; i <= n; i++) {
        if (!ids[i]) scc_dfs(i);
    }

    scc_edges_arr = malloc(sizeof(SCCEdge) * m);
    for (int u = 1; u <= n; u++) {
        for (int e = raw_head[u]; e; e = raw_edges[e].next) {
            int v = raw_edges[e].to;
            int scc_u = scc_ids[u], scc_v = scc_ids[v];
            if (scc_u != scc_v) scc_edges_arr[scc_edge_arr_cnt++] = (SCCEdge) { scc_u, scc_v };
        }
    }
    qsort(scc_edges_arr, scc_edge_arr_cnt, sizeof(SCCEdge), cmp_scc_edges_arr);

    scc_edges = malloc(sizeof(Edge) * (scc_edge_arr_cnt + 1));
    scc_head = malloc(sizeof(int) * (scc_cnt + 1));
    int scc_edgeCount = 0;

    int *in_deg = malloc(sizeof(int) * (scc_cnt + 1));
    int *out_deg = malloc(sizeof(int) * (scc_cnt + 1));

    for (int i = 0; i < scc_edge_arr_cnt; i++) {
        int su = scc_edges_arr[i].u;
        int sv = scc_edges_arr[i].v;
        if (!i || su != scc_edges_arr[i-1].u || sv != scc_edges_arr[i-1].v) {
            addEdge(su, sv, scc_edges, scc_head, &scc_edgeCount);
            out_deg[su]++;
            in_deg[sv]++;
        }
    }

    topo = malloc(sizeof(int) * scc_cnt);
    topo_cnt = scc_cnt;
    topo_visited = malloc(sizeof(char) * (scc_cnt + 1));
    for (int u = 1; u <= scc_cnt; u++) {
        if (!topo_visited[u]) topo_dfs(u);
    }

    char *is_critical = malloc(sizeof(char) * (scc_cnt + 1));
    int deg = 0;
    for (int u = 1; u <= scc_cnt; u++) {
        if (!in_deg[u]) {
            deg++;
            in_deg[u]++;
        }
        if (!out_deg[u]) out_deg[u]++;
    }
    for (int i = 0; i < scc_cnt; i++) {
        int u = topo[i];
        deg -= in_deg[u];
        if (!deg) is_critical[u] = 1;
        deg += out_deg[u];
    }

    char *buf = malloc(n * 7 + 1);
    char *buf_ptr = buf;
    int cnt = 0;
    for (int i = 1; i <= n; i++) {
        if (is_critical[scc_ids[i]]) {
            buf_ptr += sprintf(buf_ptr, "%d ", i);
            cnt++;
        }
    }
    *buf_ptr = '\0';
    printf("%d\n%s\n", cnt, buf);
    
    return 0;
}
