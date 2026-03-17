#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define MAXN 5000

typedef struct {
    int x, y, r;
} Radar;

Radar radars[MAXN];
int lowerIndex[MAXN], upperIndex[MAXN];
bool destroied[MAXN] = {0};
int upCnt = 0, downCnt = 0, destroyCnt = 0;
int max_dx;

bool intersect(int i, int j) {
    Radar a = radars[i];
    Radar b = radars[j];
    return abs(a.x - b.x) <= max_dx && (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) <= (a.r + b.r) * (a.r + b.r);
}

void markDestroy(int index) {
    destroied[index] = true;
    destroyCnt++;
}


typedef struct {
    unsigned short v, next_delta;
} Edge;

Edge* edges;
int head[MAXN] = {0};
int edgeCount = 0;

void addEdge(int from, int to) {
    if (!(++edgeCount & 0xffff))
        edges[edgeCount++] = (Edge){0};
    edges[edgeCount] = (Edge){to, edgeCount - head[from]};
    head[from] = edgeCount;
}

int match[MAXN];
bool temp_visited[MAXN];

bool dfs_match(int u) {
    for (int i = head[u]; i & 0xffff; i -= edges[i].next_delta) {
        int v = edges[i].v;
        if (temp_visited[v]) continue;
        temp_visited[v] = true;
        if (match[v] == -1 || dfs_match(match[v])) {
            match[v] = u;
            return true;
        }
    }
    return false;
}

void maxMatching() {
    memset(match, -1, sizeof(match));
    for (int u = 0; u < upCnt; u++) {
        memset(temp_visited, 0, sizeof(temp_visited));
        dfs_match(u);
    }
}


bool left_visited[MAXN] = {0};
bool right_visited[MAXN] = {0};

void dfs_cover(int u) {
    left_visited[u] = true;
    for (int i = head[u]; i & 0xffff; i -= edges[i].next_delta) {
        int v = edges[i].v;
        if (match[v] != u && !right_visited[v]) {
            right_visited[v] = true;
            if (match[v] != -1 && !left_visited[match[v]]) {
                dfs_cover(match[v]);
            }
        }
    }
}

void minCover() {
    bool leftMatch[MAXN] = {0};
    for (int v = 0; v < downCnt; v++) {
        if (match[v] != -1) leftMatch[match[v]] = true;
    }
    for (int u = 0; u < upCnt; u++) {
        if (!leftMatch[u]) dfs_cover(u);
    }
}


int main() {
    int n, w, l;
    if (scanf("%d %d %d", &n, &w, &l) != 3) return 1;
    max_dx = (int)(sqrt(3.0) * (w + 100)) + 1;

    for (int i = 0; i < n; i++) {
        int x, y, r;
        if (scanf("%d %d %d", &x, &y, &r) != 3) return 1;
        radars[i] = (Radar){x, y, r};

        if (y <= 100 && y + r < w + 100) {
            lowerIndex[downCnt++] = i;
        } else if (y >= w + 100 && y - r > 100) {
            upperIndex[upCnt++] = i;
        } else {
            markDestroy(i);
        }
    }

    edges = malloc(sizeof(Edge) * n * n / 4);
    for (int i = 0; i < upCnt; i++) {
        for (int j = 0; j < downCnt; j++) {
            if (intersect(upperIndex[i], lowerIndex[j])) {
                addEdge(i, j);
            }
        }
    }

    maxMatching();
    minCover();
    free(edges);

    for (int u = 0; u < upCnt; u++) {
        if (!left_visited[u]) markDestroy(upperIndex[u]);
    }
    for (int v = 0; v < downCnt; v++) {
        if (right_visited[v]) markDestroy(lowerIndex[v]);
    }

    printf("%d\n", destroyCnt);
    for (int i = 0; i < n; i++) {
        if (destroied[i]) {
            destroyCnt--;
            printf("%d%c", i + 1, destroyCnt ? ' ' : '\n');
        }
    }

    return 0;
}
