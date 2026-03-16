#include <stdio.h>
#include <stdlib.h>

typedef struct AVLNode {
    int y1, y2;
    int id;
    int height;
    struct AVLNode *left, *right;
} AVLNode;

AVLNode* new_node(int y1, int y2, int id) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->y1 = y1;
    node->y2 = y2;
    node->id = id;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

int height(AVLNode* node) {
    return node ? node->height : 0;
}

int get_balance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

AVLNode* rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    return x;
}

AVLNode* rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    return y;
}

AVLNode* avl_insert(AVLNode* root, int y1, int y2, int id, void (*unite)(int a, int b)) {
    if (!root) return new_node(y1, y2, id);

    if (y1 < root->y1) {
        if (y2 == root->y1) unite(id, root->id);
        root->left = avl_insert(root->left, y1, y2, id, unite);
    } else {
        if (y1 == root->y2) unite(id, root->id);
        root->right = avl_insert(root->right, y1, y2, id, unite);
    }

    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));
    int balance = get_balance(root);

    if (balance > 1 && y1 < root->left->y1)
        return rotate_right(root);
    if (balance < -1 && y1 > root->right->y1)
        return rotate_left(root);
    if (balance > 1 && y1 > root->left->y1) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    if (balance < -1 && y1 < root->right->y1) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

AVLNode* avl_delete(AVLNode* root, int y1) {
    if (!root) return NULL;

    if (y1 < root->y1)
        root->left = avl_delete(root->left, y1);
    else if (y1 > root->y1)
        root->right = avl_delete(root->right, y1);
    else {
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        }

        AVLNode* temp = root->right;
        while (temp->left) temp = temp->left;
        root->y1 = temp->y1;
        root->y2 = temp->y2;
        root->id = temp->id;
        root->right = avl_delete(root->right, temp->y1);
    }

    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));
    int balance = get_balance(root);

    if (balance > 1 && get_balance(root->left) >= 0)
        return rotate_right(root);
    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    if (balance < -1 && get_balance(root->right) <= 0)
        return rotate_left(root);
    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

#define MAXN 50000
#define ENTER 0
#define EXIT  1

typedef struct {
    int x1, x2;
    int y1, y2;
    long long area;
} Rect;

typedef struct {
    int id, x, y, type;
} Event;

int parent[MAXN];
long long group_area[MAXN];

int cmp_event(const void* a, const void* b) {
    const Event* ea = (const Event*)a;
    const Event* eb = (const Event*)b;
    if (ea->x != eb->x) return ea->x - eb->x;
    if (ea->type != eb->type) return ea->type - eb->type;
    return ea->y - eb->y;
}

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

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 1;

    Rect rects[MAXN];
    Event events[MAXN * 2];
    for (int i = 0; i < n; i++) {
        int x, y, w, h;
        if (scanf("%d %d %d %d", &x, &y, &w, &h) != 4) return 1;
        rects[i] = (Rect){x, x + w, y, y + h, (long long)w * h};
        parent[i] = i;
        group_area[i] = rects[i].area;

        events[i * 2] = (Event){i, rects[i].x1, rects[i].y1, ENTER};
        events[i * 2 + 1] = (Event){i, rects[i].x2, rects[i].y1, EXIT};
    }
    qsort(events, n * 2, sizeof(Event), cmp_event);

    AVLNode* root = NULL;
    int enter_ids[MAXN], exit_ids[MAXN];
    int i = 0;
    while (i < n * 2) {
        int x = events[i].x;
        int enter_n = 0, exit_n = 0;

        while (events[i].x == x && events[i].type == ENTER)
            enter_ids[enter_n++] = events[i++].id;
        while (events[i].x == x && events[i].type == EXIT)
            exit_ids[exit_n++] = events[i++].id;

        for (int j = 0; j < exit_n; j++) {
            int id = exit_ids[j];
            root = avl_delete(root, rects[id].y1);
        }

        for (int e_i = 0, x_i = 0; e_i < enter_n && x_i < exit_n;) {
            int enter_id = enter_ids[e_i];
            int exit_id = exit_ids[x_i];
            if (rects[enter_id].y2 < rects[exit_id].y2) {
                if (rects[enter_id].y2 >= rects[exit_id].y1) unite(enter_id, exit_id);
                e_i++;
            } else {
                if (rects[enter_id].y1 <= rects[exit_id].y2) unite(enter_id, exit_id);
                x_i++;
            }
        }

        for (int j = 0; j < enter_n; j++) {
            int id = enter_ids[j];
            root = avl_insert(root, rects[id].y1, rects[id].y2, id, unite);
        }
    }

    int max_root = 0;
    for (int i = 1; i < n; i++) {
        int root = find(i);
        if (group_area[root] > group_area[max_root]) {
            max_root = root;
        }
    }

    printf("%lld\n", group_area[max_root]);
    for (int i = 0; i < n; i++) {
        if (find(i) == max_root) {
            printf("%d ", i);
        }
    }
    puts("");

    return 0;
}
