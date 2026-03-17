#include <stdio.h>
#include <stdlib.h>

// AVL tree node structure: maintains active y-intervals in a balanced BST for O(log n) insert/delete
// Stores interval [y1,y2], rectangle id, node height, and child pointers
typedef struct AVLNode {
    int y1, y2;             // y-axis interval of a rectangle
    int id;                 // rectangle index, used for union-find merging
    int height;             // node height for balance calculations
    struct AVLNode *left, *right; // child pointers
} AVLNode;

// Create a new AVL node with given interval and id
static AVLNode* new_node(int y1, int y2, int id) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->y1 = y1;
    node->y2 = y2;
    node->id = id;
    node->height = 1;       // leaf node height
    node->left = node->right = NULL;
    return node;
}

// Return node height (0 if null)
static int height(AVLNode* node) {
    return node ? node->height : 0;
}

// Compute balance factor = left height - right height
static int get_balance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// Right rotation to rebalance heavy left subtree
static AVLNode* rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));

    return x;
}

// Left rotation to rebalance heavy right subtree
static AVLNode* rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));

    return y;
}

// Insert a new interval [y1,y2] with id into AVL, merge if boundaries touch
// unite(a,b) merges two rectangle components in union-find
static AVLNode* avl_insert(AVLNode* root, int y1, int y2, int id, void (*unite)(int,int)) {
    if (!root)
        return new_node(y1, y2, id);

    if (y1 < root->y1) {
        if (y2 == root->y1) // touch at boundary
            unite(id, root->id);
        root->left = avl_insert(root->left, y1, y2, id, unite);
    } else {
        if (y1 == root->y2) // touch at boundary
            unite(id, root->id);
        root->right = avl_insert(root->right, y1, y2, id, unite);
    }

    // Update height and rebalance
    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));
    int balance = get_balance(root);

    // Rebalance cases: LL, RR, LR, RL
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

// Delete interval with lower bound y1 from AVL
static AVLNode* avl_delete(AVLNode* root, int y1) {
    if (!root) return NULL;

    if (y1 < root->y1)
        root->left = avl_delete(root->left, y1);
    else if (y1 > root->y1)
        root->right = avl_delete(root->right, y1);
    else {
        // Node found: replace with child or inorder successor
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

    // Update height and rebalance
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
#define ENTER 0 // rectangle enters scan line
#define EXIT  1 // rectangle exits scan line

// Rectangle info: x-range, y-range, and precomputed area
typedef struct {
    int x1, x2;
    int y1, y2;
    long long area;
} Rect;

// Scan line event: x-coordinate, rectangle id, base y, type
typedef struct {
    int id, x, y, type;
} Event;

static int parent[MAXN];       // union-find parent array
static long long group_area[MAXN]; // accumulated area per component

// Sort events by x, then ENTER before EXIT, then by y
static int cmp_event(const void* a, const void* b) {
    const Event* ea = a;
    const Event* eb = b;
    if (ea->x != eb->x) return ea->x - eb->x;
    if (ea->type != eb->type) return ea->type - eb->type;
    return ea->y - eb->y;
}

// Union-find find with path compression (amortized O(1))
static int find(int x) {
    return parent[x] == x ? x : (parent[x] = find(parent[x]));
}

// Union-find union: merge b into a and sum areas
static void unite(int a, int b) {
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

    // Read rectangles, init union-find and events
    for (int i = 0; i < n; i++) {
        int x, y, w, h;
        if (scanf("%d %d %d %d", &x, &y, &w, &h) != 4) return 1;
        rects[i] = (Rect){x, x + w, y, y + h, (long long)w * h};
        parent[i] = i;
        group_area[i] = rects[i].area;
        events[i*2] = (Event){i, rects[i].x1, rects[i].y1, ENTER};
        events[i*2+1] = (Event){i, rects[i].x2, rects[i].y1, EXIT};
    }
    qsort(events, n*2, sizeof(Event), cmp_event);

    AVLNode* root = NULL;
    int enter_ids[MAXN], exit_ids[MAXN];
    int i = 0;

    // Sweep line over x
    while (i < n*2) {
        int x = events[i].x;
        int e_cnt = 0, x_cnt = 0;

        // Collect ENTER events at x
        while (i < n*2 && events[i].x == x && events[i].type == ENTER)
            enter_ids[e_cnt++] = events[i++].id;
        // Collect EXIT events at x
        while (i < n*2 && events[i].x == x && events[i].type == EXIT)
            exit_ids[x_cnt++] = events[i++].id;

        // Remove intervals for EXIT first to avoid false touches
        for (int j = 0; j < x_cnt; j++)
            root = avl_delete(root, rects[exit_ids[j]].y1);

        // Two-pointer merge check between entering and exiting intervals
        for (int ei = 0, xi = 0; ei < e_cnt && xi < x_cnt; ) {
            int id_e = enter_ids[ei];
            int id_x = exit_ids[xi];
            if (rects[id_e].y2 < rects[id_x].y2) {
                if (rects[id_e].y2 >= rects[id_x].y1)
                    unite(id_e, id_x);
                ei++;
            } else {
                if (rects[id_e].y1 <= rects[id_x].y2)
                    unite(id_e, id_x);
                xi++;
            }
        }

        // Insert new intervals for ENTER
        for (int j = 0; j < e_cnt; j++)
            root = avl_insert(root, rects[enter_ids[j]].y1, rects[enter_ids[j]].y2, enter_ids[j], unite);
    }

    // Find component with max area
    int max_root = 0;
    for (int i = 1; i < n; i++) {
        int r = find(i);
        if (group_area[r] > group_area[max_root])
            max_root = r;
    }

    // Output total area and sorted list of rectangle indices
    printf("%lld\n", group_area[max_root]);
    for (int i = 0; i < n; i++) {
        if (find(i) == max_root)
            printf("%d ", i);
    }
    puts("");

    return 0;
}
