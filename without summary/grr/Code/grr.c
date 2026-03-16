#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

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


int *topo, topo_cnt;
char *topo_visited;

void topo_dfs(int u, Edge *edges, int *head) {
    topo_visited[u] = 1;
    for (int e = head[u]; e; e = edges[e].next) {
        int v = edges[e].to;
        if (!topo_visited[v]) topo_dfs(v, edges, head);
    }
    topo[topo_cnt++] = u;
}


typedef struct Node {
    int val;
    struct Node *next;
} Node;

Node *insert_node(Node *head, int val) {
    Node *n = malloc(sizeof(Node));
    *n = (Node){val, head};
    return n;
}

typedef struct {
    Node *list;
    int *hashtable;
    int cap;
    int size;
} Set;

static inline int hash(int x) {
    x *= 2654435761;
    return x ^ (x >> 16);
}

void set_init(Set *s) {
    *s = (Set){NULL, calloc(4, sizeof(int)), 4, 0};
}

void set_insert(Set *s, int val) {
    if (s->cap < s->size * 2) {
        free(s->hashtable);
        s->cap <<= 1;
        s->size = 0;
        s->hashtable = calloc(s->cap, sizeof(int));
        for (Node *n = s->list; n; n = n->next) {
            set_insert(s, n->val);
        }
    }

    int idx = hash(val) & (s->cap - 1);
    while (s->hashtable[idx]) {
        if (s->hashtable[idx] == val) return;
        idx = (idx + 1) & (s->cap - 1);
    }
    s->hashtable[idx] = val;
    s->list = insert_node(s->list, val);
    s->size++;
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
        if (!i || scc_edges_arr[i].u != scc_edges_arr[i - 1].u || scc_edges_arr[i].v != scc_edges_arr[i - 1].v) {
            addEdge(scc_edges_arr[i].u, scc_edges_arr[i].v, scc_edges1, scc_head1, &scc_edgeCount1);
            addEdge(scc_edges_arr[i].v, scc_edges_arr[i].u, scc_edges2, scc_head2, &scc_edgeCount2);
        }
    }


    topo = malloc(sizeof(int) * scc_cnt);
    topo_cnt = 0;
    topo_visited = malloc(sizeof(char) * (scc_cnt + 1));
    for (int u = 1; u <= scc_cnt; u++) {
        if (!topo_visited[u]) topo_dfs(u, scc_edges1, scc_head1);
    }

    
    Set *forward_reach = malloc(sizeof(Set) * (scc_cnt + 1));
    int *forward_cnt = malloc(sizeof(int) * (scc_cnt + 1));
    Set *backward_reach = malloc(sizeof(Set) * (scc_cnt + 1));
    int *backward_cnt = malloc(sizeof(int) * (scc_cnt + 1));
    
    for (int u = 1; u <= scc_cnt; u++) {
        set_init(&forward_reach[u]);
        set_insert(&forward_reach[u], u);
        set_init(&backward_reach[u]);
        set_insert(&backward_reach[u], u);
    }
    
    for (int i = 0; i < topo_cnt; i++) {
        int u = topo[i];
        for (int e = scc_head1[u]; e; e = scc_edges1[e].next) {
            int to = scc_edges1[e].to;
            if (forward_reach[u].size < forward_reach[to].size) {
                Set tmp = forward_reach[u];
                forward_reach[u] = forward_reach[to];
                forward_reach[to] = tmp;
            }
            for (Node *n = forward_reach[to].list; n; n = n->next) {
                set_insert(&forward_reach[u], n->val);
            }
        }
        forward_cnt[u] = forward_reach[u].size - 1;
    }

    for (int i = topo_cnt - 1; i >= 0; i--) {
        int u = topo[i];
        for (int e = scc_head2[u]; e; e = scc_edges2[e].next) {
            int from = scc_edges2[e].to;
            if (backward_reach[u].size < backward_reach[from].size) {
                Set tmp = backward_reach[u];
                backward_reach[u] = backward_reach[from];
                backward_reach[from] = tmp;
            }
            for (Node *n = backward_reach[from].list; n; n = n->next) {
                set_insert(&backward_reach[u], n->val);
            }
        }
        backward_cnt[u] = backward_reach[u].size - 1;
    }
    
    
    char *buf = malloc(n * 7 + 1);
    char *buf_ptr = buf;
    int cnt = 0;
    for (int i = 1; i <= n; i++) {
        int u = scc_ids[i];
        if (forward_cnt[u] + backward_cnt[u] == scc_cnt - 1) {
            buf_ptr += sprintf(buf_ptr, "%d ", i);
            cnt++;
        }
    }
    *buf_ptr = '\0';
    printf("%d\n%s\n", cnt, buf);

    return 0;
}
