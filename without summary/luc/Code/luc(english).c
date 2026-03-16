#include <stdio.h>
#include <stdlib.h>

#define MAXN 10000
#define MAXM 200000

// Structure for cat-to-match adjacency list
typedef struct Edge {
    int to; // match index
    int next; // next pointer in linked list
} Edge;

// Adjacency list storage: for each cat, list of matches it plays
Edge c2m[MAXM * 2 + 1];
int c2m_head[MAXN + 1] = {0};
int edgeCount = 0;

// Add edge from cat to match
void addEdge(int cat, int match) {
    c2m[++edgeCount] = (Edge){match, c2m_head[cat]};
    c2m_head[cat] = edgeCount;
}

// For each match i, store its two participating cats
int m2c[MAXM + 1][2];
// Current winner of match i (one of m2c[i][0/1])
int winner[MAXM + 1];
// Current win counts for each cat
int winCnt[MAXN + 1] = {0};

// Visitation marks with round IDs to avoid memset
int cat_visited[MAXN + 1] = {0}, match_visited[MAXM + 1] = {0}, rnd = 0;
// BFS queue for augmenting path search (storing match,cat pairs)
int bfs_queue[MAXM * 2], enq, deq;
int prev_matches[MAXM + 1];

// Attempt to free one win-slot from 'cat' by finding an augmenting path
int augment_from(int cat, int limit) {
    enq = 0, deq = 0;
    // Initialize BFS from the overloaded cat
    bfs_queue[enq++] = 0; // dummy previous match id for start
    bfs_queue[enq++] = cat;
    cat_visited[cat] = ++rnd;

    // BFS over alternating paths (match<->cat)
    while (deq < enq) {
        int prev_mid = bfs_queue[deq++];
        int cur = bfs_queue[deq++];

        // Explore all matches involving cur
        for (int e = c2m_head[cur]; e; e = c2m[e].next) {
            int mid = c2m[e].to;
            if (match_visited[mid] == rnd) continue; // skip visited match
            int next = m2c[mid][0] ^ m2c[mid][1] ^ cur;
            if (cat_visited[next] == rnd) continue; // skip visited cat
            // Only follow edges where cur is current winner
            if (cur != winner[mid]) continue;

            // If opponent has capacity, flip along this edge and backtrack
            if (winCnt[next] < limit) {
                prev_matches[mid] = prev_mid;
                // Perform flips along the found path
                winCnt[next]++;
                while (mid) {
                    // Flip winner of this match
                    winner[mid] ^= m2c[mid][0] ^ m2c[mid][1];
                    mid = prev_matches[mid];
                }
                winCnt[cat]--;
                return 1; // successful augmentation
            }
            
            // enqueue for further search
            bfs_queue[enq++] = mid;
            bfs_queue[enq++] = next;
            match_visited[mid] = rnd;
            prev_matches[mid] = prev_mid;
            cat_visited[next] = rnd;
        }
    }
    return 0; // no augmenting path found
}

// Check if we can ensure every cat has at most 'limit' wins
int check(int limit, int n) {
    for (int cat = 1; cat <= n; cat++) {
        // While cat is overloaded, try to free slots
        while (winCnt[cat] > limit) {
            if (!augment_from(cat, limit)) return 0;
        }
    }
    return 1;
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1;
    
    // Read matches and build initial greedy assignment
    for (int i = 1; i <= m; i++) {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2) return 1;
        m2c[i][0] = x;
        m2c[i][1] = y;
        addEdge(x, i);
        addEdge(y, i);

        // Greedy: assign win to cat with fewer wins so far
        int greedy_winner = winCnt[x] < winCnt[y] ? x : y;
        winner[i] = greedy_winner;
        winCnt[greedy_winner]++;
    }
    
    // Binary search on minimal max wins
    int l = m / n, u = m;
    while (l < u) {
        int limit = (l + u) >> 1;
        if (check(limit, n)) u = limit;
        else l = limit + 1;
    }
    printf("%d\n", l);

    // Final orientation
    check(l, n);
    for (int i = 1; i <= m; i++) {
        // Print 1 if the first cat won, else 0
        printf("%d\n", winner[i] == m2c[i][0] ? 1 : 0);
    }

    return 0;
}
