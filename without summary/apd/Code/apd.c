#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long ull;
ull *pairs;
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

void add_edge(int u, int v) {
    edges[++ecnt] = (Edge){v, head[u]};
    head[u] = ecnt;
    deg[u]++;
    edges[++ecnt] = (Edge){u, head[v]};
    head[v] = ecnt;
    deg[v]++;
}


int *MVC;
int kcnt = 0;

char check_vc(int sz) {
    if (sz < 0) return 0;
    for (int i = 0; i < kcnt; i++) {
        int u = pairs[i] >> 32, v = pairs[i] & 0xffffffff;
        if (MVC[u] || MVC[v]) continue;

        MVC[u] = 2;
        if (check_vc(sz - 1)) return 1;
        MVC[u] = 0;
        
        MVC[v] = 2;
        if (check_vc(sz - 1)) return 1;
        MVC[v] = 0;

        return 0;
    }
    return 1;
}

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
    qsort(pairs, m, sizeof(ull), cmp_ull);

    edges = malloc(sizeof(Edge) * (m << 1 | 1));
    head = malloc(sizeof(int) * (n + 1));
    deg = malloc(sizeof(int) * (n + 1));
    for (int i = 0; i < m; i++) {
        if (!i || pairs[i] != pairs[i - 1]) add_edge(pairs[i] >> 32, pairs[i] & 0xffffffff);
    }
    

    MVC = malloc(sizeof(int) * (n + 1));
    int *stack = malloc(sizeof(int) * n), sp = 0;

    int vc_size = n - k;
    for (int i = 1; i <= n; i++) {
        if (deg[i] > vc_size) stack[sp++] = i;
    }

    while (sp) {
        int u = stack[--sp];
        if (deg[u] <= vc_size) continue;
        MVC[u] = 1;
        vc_size--;

        for (int e = head[u]; e; e = edges[e].next) {
            int v = edges[e].to;
            if (--deg[v] == vc_size) stack[sp++] = v;
        }
        deg[u] = 0;
    }
    k += vc_size;


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

    if (!check_vc(vc_size)) {
        puts("CATASTROPHE");
        return 0;
    }
    clean_MVC(n);
    while (check_vc(--vc_size)) {
        clean_MVC(n);
    }

    check_vc(++vc_size);
    printf("%d\n", k - vc_size);
    for (int i = 1; i <= n; i++) {
        if (!MVC[i]) printf("%d ", i);
    }
    
    return 0;
}
