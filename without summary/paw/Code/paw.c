#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

#define MAXN 500000
#define MAXM 1000000
#define MOD 1000000007
#define MIN(a, b) ( (a) < (b) ? (a) : (b) )

void exit_meow_no() {
    printf("MEOW-NO\n");
    exit(0);
}

typedef struct Edge {
    int to;
    int next;
} Edge;

Edge *edges;
int *head;
int edgeCount = 0;

void addEdge(int u, int v) {
    edges[++edgeCount] = (Edge){v, head[u]};
    head[u] = edgeCount;
    edges[++edgeCount] = (Edge){u, head[v]};
    head[v] = edgeCount;
}

int *ids, id = 0;
int *low;
int *stack, stackTop = 0;

int cycleLen, cycleCnt = 0;
char inBCC[MAXN + 1] = {0};
int BCC[MAXN];

int E = 0, V = 0;
void processBCC(int stop) {
    for (int i = 0; i < V; i++) {
        inBCC[BCC[i]] = 0;
    }

    E = 0, V = 0;
    int e;
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

    if (E == 1) return;
    if (E == V) {
        if (!cycleLen) cycleLen = E;
        else if (E != cycleLen) exit_meow_no();
        cycleCnt++;
        return;
    }
    
    int hCnt = 0, s = 0, t = 0;
    for (int i = 0; i < V; i++){
        int u = BCC[i], d = 0;
        for (int e = head[u]; e; e = edges[e].next) {
            int v = edges[e].to;
            if (inBCC[v]) d++;
        }
        if (d > 2){
            if (hCnt == 0) s = u;
            else if (hCnt == 1) t = u;
            hCnt++;
        } else if(d != 2) exit_meow_no();
    }
    if (hCnt != 2) exit_meow_no();
    
    int len = 0, lCnt = 0;
    for (int e = head[s]; e; e = edges[e].next) {
        int cur = edges[e].to;
        if (!inBCC[cur]) continue;
        int prev = s, cur_len = 1;
        while (cur != t) {
            int nxt;
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

        if (!len) len = cur_len;
        else if (cur_len != len) exit_meow_no();
        lCnt++;
    }

    if (!cycleLen) cycleLen = len * 2;
    else if (len * 2 != cycleLen) exit_meow_no();
    cycleCnt = (cycleCnt + (long long)lCnt * (lCnt - 1) / 2) % MOD;
}

void dfs(int u, int p) {
    ids[u] = low[u] = ++id;

    for (int e = head[u]; e; e = edges[e].next) {
        int v = edges[e].to;
        if (v == p) continue;
        if (!ids[v]) {
            stack[stackTop++] = e;
            dfs(v, u);
            low[u] = MIN(low[u], low[v]);
            if (low[v] >= ids[u]) processBCC(e);
        } else if (ids[v] < ids[u]) {
            stack[stackTop++] = e;
            low[u] = MIN(low[u], ids[v]);
        }
    }
}

int main() {
    struct rlimit rl;
    if (getrlimit(RLIMIT_STACK, &rl) == 0) {
        rl.rlim_cur = rl.rlim_max;
        setrlimit(RLIMIT_STACK, &rl);
    }
    
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1;

    edges = malloc(sizeof(Edge) * (m * 2 + 1));
    head = malloc(sizeof(int) * (n + 1));

    for (int i = 0; i < m; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) return 1;
        addEdge(u, v);
    }

    ids = malloc(sizeof(int) * (n + 1));
    low = malloc(sizeof(int) * (n + 1));
    stack = malloc(sizeof(int) * (m * 2));

    for (int i = 1; i <= n; i++) {
        if (!ids[i]) dfs(i, 0);
    }

    if (cycleCnt == 0) {
        printf("FUR-LORN\n");
    } else {
        long long total = ((long long)2 * cycleLen % MOD) * cycleCnt % MOD;
        printf("PURR-FECT\n%d %lld\n", cycleLen, total);
    }
    
    return 0;
}
