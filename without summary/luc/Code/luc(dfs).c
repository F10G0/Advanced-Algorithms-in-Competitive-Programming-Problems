#include <stdio.h>
#include <stdlib.h>

#define MAXN 10000
#define MAXM 200000

typedef struct Edge {
    int to;
    int next;
} Edge;

Edge c2m[MAXM * 2 + 1];
int c2m_head[MAXN + 1] = {0};
int edgeCount = 0;

void addEdge(int cat, int match) {
    c2m[++edgeCount] = (Edge){match, c2m_head[cat]};
    c2m_head[cat] = edgeCount;
}

int m2c[MAXM + 1][2];
int winner[MAXM + 1];
int winCnt[MAXN + 1] = {0};

int cat_visited[MAXN + 1] = {0}, match_visited[MAXM + 1] = {0}, rnd = 0;
int bfs_queue[MAXM * 3], enq, deq;
int prev_matches[MAXM + 1];

int dfs(int cur, int limit) {
    cat_visited[cur] = rnd;
    for (int e = c2m_head[cur]; e; e = c2m[e].next) {
        int mid = c2m[e].to;
        if (match_visited[mid] == rnd) continue;
        if (cur != winner[mid]) continue;

        int next = m2c[mid][0] ^ m2c[mid][1] ^ cur;
        if (cat_visited[next] == rnd) continue;
        if (winCnt[next] < limit) {
            winCnt[next]++;
            winner[mid] ^= m2c[mid][0] ^ m2c[mid][1];
            return 1;
        }
        match_visited[mid] = rnd;
        if (dfs(next, limit)) {
            winner[mid] ^= m2c[mid][0] ^ m2c[mid][1];
            return 1;
        }
    }
    return 0;
}

int augment_from(int cat, int limit) {
    rnd++;
    return dfs(cat, limit);
}

int check(int limit, int n) {
    for (int cat = 1; cat <= n; cat++) {
        while (winCnt[cat] > limit) {
            if (!augment_from(cat, limit)) return 0;
            winCnt[cat]--;
        }
    }
    return 1;
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1;
    
    for (int i = 1; i <= m; i++) {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2) return 1;
        m2c[i][0] = x;
        m2c[i][1] = y;
        addEdge(x, i);
        addEdge(y, i);

        int greedy_winner = winCnt[x] < winCnt[y] ? x : y;
        winner[i] = greedy_winner;
        winCnt[greedy_winner]++;
    }
    
    int l = m / n, u = m;
    while (l < u) {
        int limit = (l + u) >> 1;
        if (check(limit, n)) u = limit;
        else l = limit + 1;
    }
    printf("%d\n", l);

    check(l, n);
    for (int i = 1; i <= m; i++) {
        printf("%d\n", winner[i] == m2c[i][0] ? 1 : 0);
    }

    return 0;
}
