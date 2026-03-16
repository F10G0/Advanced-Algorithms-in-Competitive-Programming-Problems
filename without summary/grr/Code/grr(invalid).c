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

    Edge *scc_edges1 = malloc(sizeof(Edge) * (scc_edge_arr_cnt + 1));
    int *scc_head1 = malloc(sizeof(int) * (scc_cnt + 1));
    int scc_edgeCount1 = 0;
    Edge *scc_edges2 = malloc(sizeof(Edge) * (scc_edge_arr_cnt + 1));
    int *scc_head2 = malloc(sizeof(int) * (scc_cnt + 1));
    int scc_edgeCount2 = 0;

    for (int i = 0; i < scc_edge_arr_cnt; i++) {
        if (!i || scc_edges_arr[i].u != scc_edges_arr[i-1].u || scc_edges_arr[i].v != scc_edges_arr[i-1].v) {
            addEdge(scc_edges_arr[i].u, scc_edges_arr[i].v, scc_edges1, scc_head1, &scc_edgeCount1);
            addEdge(scc_edges_arr[i].v, scc_edges_arr[i].u, scc_edges2, scc_head2, &scc_edgeCount2);
        }
    }

    int *in_deg = malloc(sizeof(int) * (scc_cnt + 1));
    int *out_deg = malloc(sizeof(int) * (scc_cnt + 1));
    int *topo_ids = malloc(sizeof(int) * scc_cnt);
    int topo_cnt = 0;
    for (int u = 1; u <= scc_cnt; u++) {
        for (int e = scc_head1[u]; e; e = scc_edges1[e].next) {
            int v = scc_edges1[e].to;
            out_deg[u]++;
            in_deg[v]++;
        }
    }

    int *queue = malloc(sizeof(int) * scc_cnt);
    int enq = 0, deq = 0;
    long long *from_path_cnt = malloc(sizeof(long long) * (scc_cnt + 1));
    long long *to_path_cnt = malloc(sizeof(long long) * (scc_cnt + 1));
    for (int u = 1; u <= scc_cnt; u++) {
        if (!in_deg[u]) {
            queue[enq++] = u;
            from_path_cnt[u] = 1;
        }
        if (!out_deg[u]) to_path_cnt[u] = 1;
    }
    while (deq < enq) {
        int u = queue[deq++];
        topo_ids[topo_cnt++] = u;
        for (int e = scc_head1[u]; e; e = scc_edges1[e].next) {
            int v = scc_edges1[e].to;
            if (--in_deg[v] == 0) queue[enq++] = v;
        }
    }

    for (int i = 0; i < topo_cnt; i++) {
        int u = topo_ids[i];
        for (int e = scc_head1[u]; e; e = scc_edges1[e].next) {
            int v = scc_edges1[e].to;
            from_path_cnt[v] = (from_path_cnt[v] + from_path_cnt[u]) % MOD;
        }
    }
    for (int i = topo_cnt - 1; i >= 0; i--) {
        int u = topo_ids[i];
        for (int e = scc_head2[u]; e; e = scc_edges2[e].next) {
            int v = scc_edges2[e].to;
            to_path_cnt[v] = (to_path_cnt[v] + to_path_cnt[u]) % MOD;
        }
    }

    long long total_path_cnt = 0;
    for (int u = 1; u <= scc_cnt; u++) {
        if (!out_deg[u]) total_path_cnt = (total_path_cnt + from_path_cnt[u]) % MOD;
    }

    char *is_critical = malloc(sizeof(char) * (scc_cnt + 1));
    for (int u = 1; u <= scc_cnt; u++) {
        if (from_path_cnt[u] * to_path_cnt[u] % MOD == total_path_cnt) {
            is_critical[u] = 1;
        }
    }

    int *ans = malloc(sizeof(int) * n);
    int ans_cnt = 0;
    for (int x = 1; x <= n; x++) {
        if (is_critical[scc_ids[x]]) {
            ans[ans_cnt++] = x;
        }
    }

    printf("%d\n", ans_cnt);
    for (int i = 0; i < ans_cnt; i++) {
        printf("%d ", ans[i]);
    }
    puts("");

    return 0;
}
