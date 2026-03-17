#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long ull;

// A function is represented by f(i) = a * i + b
// We aim to construct a prefix-optimal sequence such that each day uses the best available function.
typedef struct {
    ull a, b;
} Func;

// Sort by increasing slope (a); if equal, sort by decreasing intercept (b)
// This ensures prefix-optimal construction.
int cmp_Func(const void* a, const void* b) {
    const Func* fa = (const Func*)a;
    const Func* fb = (const Func*)b;
    if (fa->a != fb->a)
        return (fa->a > fb->a) - (fa->a < fb->a);
    return (fb->b > fa->b) - (fb->b < fa->b);
}

// Node represents a function in a splay tree.
// push_val: cumulative compensation (b += a)
// delay_days: how many days the function is delayed (must subtract a * d)
// size: subtree size (used for in-order day index)
typedef struct Node {
    ull a, b;
    ull push_val, delay_days;
    int size;
    struct Node *left, *right, *parent;
} Node;

Node* root = NULL;

// Evaluate the function at a specific day i (after pushdown already applied)
ull evaluate_at_day(Node* n, int i) {
    return n->a * i + n->b;  // classic linear evaluation: f(i) = a*i + b
}

// Create a new node from a function f
Node* new_node(Func f) {
    Node* node = malloc(sizeof(Node));
    node->a = f.a;
    node->b = f.b;
    node->push_val = 0;
    node->delay_days = 0;
    node->size = 1;
    node->left = node->right = node->parent = NULL;
    return node;
}

int static inline get_size(Node* node) {
    return node ? node->size : 0;
}

// Update the size field based on children
void update_size(Node* node) {
    if (!node) return;
    node->size = 1 + get_size(node->left) + get_size(node->right);
}

// Apply all lazy updates to the current node (push_val and delay_days)
// Also propagate the updates to children.
void apply_lazy_updates(Node* node) {
    if (node && node->push_val) {
        node->b += node->push_val;  // apply b += a compensation
        node->b -= node->a * node->delay_days;  // apply delay penalty
        if (node->left) {
            node->left->push_val += node->push_val;
            node->left->delay_days += node->delay_days;
        }
        if (node->right) {
            node->right->push_val += node->push_val;
            node->right->delay_days += node->delay_days;
        }
        node->push_val = 0;
        node->delay_days = 0;
    }
}

// Perform splay tree rotation, maintaining lazy propagation correctness
void rotate(Node* x) {
    Node* p = x->parent;
    Node* g = p ? p->parent : NULL;
    apply_lazy_updates(p);  // ensure parent is updated
    apply_lazy_updates(x);  // ensure current is updated
    if (p->left == x) {
        p->left = x->right;
        if (x->right) x->right->parent = p;
        x->right = p;
    } else {
        p->right = x->left;
        if (x->left) x->left->parent = p;
        x->left = p;
    }
    x->parent = g;
    p->parent = x;
    if (g) {
        if (g->left == p) g->left = x;
        else g->right = x;
    }
    update_size(p);
    update_size(x);
}

// Splay operation to bring node x to the root
void splay(Node* x) {
    while (x->parent) {
        Node* p = x->parent;
        Node* g = p->parent;
        if (!g) {
            rotate(x);  // zig
        } else if ((g->left == p) == (p->left == x)) {
            rotate(p);  // zig-zig
            rotate(x);
        } else {
            rotate(x);  // zig-zag
            rotate(x);
        }
    }
    root = x;
}

// Insert a new function into the splay tree at the correct position
// Ensures the prefix-optimal condition is maintained
// After insertion, splay to root and apply compensation to right subtree
void insert_node(Func f) {
    Node* node = new_node(f);
    if (!root) {
        root = node;
        return;
    }

    Node* cur = root;
    ull day = get_size(cur->left);  // find correct day (index in in-order)
    while (cur) {
        apply_lazy_updates(cur);
        if (evaluate_at_day(node, day) > evaluate_at_day(cur, day)) {
            if (!cur->left) {
                cur->left = node;
                node->parent = cur;
                break;
            } else {
                cur = cur->left;
                day -= get_size(cur->right) + 1;  // left child reduces day index
            }
        } else {
            if (!cur->right) {
                cur->right = node;
                node->parent = cur;
                break;
            } else {
                cur = cur->right;
                day += get_size(cur->left) + 1;  // right child increases day index
            }
        }
    }

    splay(node);
    if (node->right) {
        node->right->push_val += f.a;   // add compensation to right subtree
        node->right->delay_days++;      // delay by 1 day
    }
}

// Non-recursive in-order traversal of the tree
// Outputs the total value sum day by day
void output_total_value(int n) {
    ull sum = 0;
    Node** stack = malloc(sizeof(Node*) * n);
    int sp = 0, day = 0;
    Node* cur = root;
    while (day < n) {
        while (cur) {
            stack[sp++] = cur;
            apply_lazy_updates(cur);
            cur = cur->left;
        }
        cur = stack[--sp];
        sum += evaluate_at_day(cur, day++);  // compute current day's value
        printf("%llu\n", sum);               // output total up to current day
        cur = cur->right;
    }
    free(stack);
}

int main() {
    int n;
    scanf("%d", &n);

    Func* funcs = malloc(sizeof(Func) * n);
    for (int i = 0; i < n; i++) {
        scanf("%llu %llu", &funcs[i].a, &funcs[i].b);
    }
    qsort(funcs, n, sizeof(Func), cmp_Func);  // sort before insertion

    for (int i = 0; i < n; i++) {
        insert_node(funcs[i]);  // insert each function into splay tree
    }
    output_total_value(n);

    free(funcs);
    return 0;
}
