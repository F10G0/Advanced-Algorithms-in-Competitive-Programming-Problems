#include <stdio.h>
#include <stdlib.h>

#define MAXN 50005
typedef struct {
    int x, y, w, h, area;
} Rect;

Rect rects[MAXN];
int parent[MAXN];
int group_area[MAXN];

int find(int x) {
    return parent[x] == x ? x : (parent[x] = find(parent[x]));
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a != b) {
        parent[b] = a;
        group_area[a] += group_area[b];
    }
}

int touch(Rect* a, Rect* b) {
    return !(a->x + a->w < b->x || b->x + b->w < a->x || a->y + a->h < b->y || b->y + b->h < a->y);
}

int main() {
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        scanf("%d %d %d %d", &rects[i].x, &rects[i].y, &rects[i].w, &rects[i].h);
        parent[i] = i;
        rects[i].area = rects[i].w * rects[i].h;
        group_area[i] = rects[i].area;
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (touch(&rects[i], &rects[j])) {
                unite(i, j);
            }
        }
    }
    
    int max_root = 0;
    for (int i = 1; i < n; i++) {
        int root = find(i);
        if (group_area[root] > group_area[max_root]) {
            max_root = root;
        }
    }

    printf("%d\n", group_area[max_root]);
    for (int i = 0; i < n; i++) {
        if (find(i) == max_root) {
            printf("%d ", i);
        }
    }
    puts("");
    
    return 0;
}
