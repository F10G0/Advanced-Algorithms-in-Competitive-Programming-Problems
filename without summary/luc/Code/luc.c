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
int bfs_queue[MAXM * 2], enq, deq;
int prev_matches[MAXM + 1];

int augment_from(int cat, int limit) {
    enq = 0, deq = 0;
    bfs_queue[enq++] = 0;
    bfs_queue[enq++] = cat;
    cat_visited[cat] = ++rnd;

    while (deq < enq) {
        int prev_mid = bfs_queue[deq++];
        int cur = bfs_queue[deq++];

        for (int e = c2m_head[cur]; e; e = c2m[e].next) {
            int mid = c2m[e].to;
            if (match_visited[mid] == rnd) continue;
            int next = m2c[mid][0] ^ m2c[mid][1] ^ cur;
            if (cat_visited[next] == rnd) continue;
            if (cur != winner[mid]) continue;

            if (winCnt[next] < limit) {
                prev_matches[mid] = prev_mid;
                winCnt[next]++;
                while (mid) {
                    winner[mid] ^= m2c[mid][0] ^ m2c[mid][1];
                    mid = prev_matches[mid];
                }
                winCnt[cat]--;
                return 1;
            }
            
            bfs_queue[enq++] = mid;
            bfs_queue[enq++] = next;
            match_visited[mid] = rnd;
            prev_matches[mid] = prev_mid;
            cat_visited[next] = rnd;
        }
    }
    return 0;
}

int check(int limit, int n) {
    for (int cat = 1; cat <= n; cat++) {
        while (winCnt[cat] > limit) {
            if (!augment_from(cat, limit)) return 0;
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
