#include <stdio.h>
#include <stdlib.h>

#define MAXN 50000
#define HASH_SIZE 0x40000

typedef struct {
    int x, y, w, h, area;
} Rect;

typedef struct Node {
    int index;
    struct Node* next;
} BucketNode;

typedef struct {
    int xi, yi;
    BucketNode* head;
} Bucket;

Rect rects[MAXN];
int parent[MAXN];
int group_area[MAXN];

Bucket* hash_table[HASH_SIZE];

static inline int find(int x) {
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

static inline int touch(const Rect* a, const Rect* b) {
    return !(a->x + a->w < b->x || b->x + b->w < a->x || a->y + a->h < b->y || b->y + b->h < a->y);
}

static inline int bucket_index(int coordination, int block) {
    return (coordination >= 0) ? (coordination / block) : ((coordination + 1) / block - 1);
}

static inline unsigned int hash(int x, int y) {
    return ((unsigned int)(x * 73856093u) ^ (unsigned int)(y * 19349663u)) % HASH_SIZE;
}

Bucket** find_slot(int x, int y) {
    unsigned int h = hash(x, y);
    while (hash_table[h] && !(hash_table[h]->xi == x && hash_table[h]->yi == y)) {
        h = (h + 1) % HASH_SIZE;
    }
    return &hash_table[h];
}

void insert_bucket(int x, int y, int index) {
    Bucket** slot = find_slot(x, y);
    if (!*slot) {
        *slot = (Bucket*)malloc(sizeof(Bucket));
        **slot = (Bucket){ x, y, NULL };
    }
    BucketNode* node = (BucketNode*)malloc(sizeof(BucketNode));
    node->index = index;
    node->next = (*slot)->head;
    (*slot)->head = node;
}

static inline BucketNode* get_bucket(int x, int y) {
    Bucket** slot = find_slot(x, y);
    return (*slot) ? (*slot)->head : NULL;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 1;

    int block_w = 1, block_h = 1;
    for (int i = 0; i < n; i++) {
        if (scanf("%d %d %d %d", &rects[i].x, &rects[i].y, &rects[i].w, &rects[i].h) != 4) return 1;
        rects[i].area = rects[i].w * rects[i].h;
        parent[i] = i;
        group_area[i] = rects[i].area;
        if (rects[i].w > block_w) block_w = rects[i].w;
        if (rects[i].h > block_h) block_h = rects[i].h;
    }

    for (int i = 0; i < n; i++) {
        int bx = bucket_index(rects[i].x, block_w);
        int by = bucket_index(rects[i].y, block_h);
        insert_bucket(bx, by, i);
    }

    for (int i = 0; i < n; i++) {
        int bx = bucket_index(rects[i].x, block_w);
        int by = bucket_index(rects[i].y, block_h);
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                BucketNode* cur = get_bucket(bx + dx, by + dy);
                while (cur) {
                    int j = cur->index;
                    if (j <= i) break;
                    if (touch(&rects[i], &rects[j])) {
                        unite(i, j);
                    }
                    cur = cur->next;
                }
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
