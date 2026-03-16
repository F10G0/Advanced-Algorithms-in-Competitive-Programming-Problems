#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long ull;

typedef struct {
    ull a, b;
} Func;

int cmp_Func(const void* a, const void* b) {
    const Func* fa = (const Func*)a;
    const Func* fb = (const Func*)b;
    if (fa->a != fb->a)
        return (fa->a > fb->a) - (fa->a < fb->a);
    return (fb->b > fa->b) - (fb->b < fa->b);
}

typedef struct Node {
    ull a, b;
    ull push_val, delay_days;
    int size;
    struct Node *left, *right, *parent;
} Node;

Node* root = NULL;

ull evaluate_at_day(Node* n, int i) {
    return n->a * i + n->b;
}

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

void update_size(Node* node) {
    if (!node) return;
    node->size = 1 + get_size(node->left) + get_size(node->right);
}

void apply_lazy_updates(Node* node) {
    if (node && node->push_val) {
        node->b += node->push_val;
        node->b -= node->a * node->delay_days;
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

void rotate(Node* x) {
    Node* p = x->parent;
    Node* g = p ? p->parent : NULL;
    apply_lazy_updates(p);
    apply_lazy_updates(x);
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

void splay(Node* x) {
    while (x->parent) {
        Node* p = x->parent;
        Node* g = p->parent;
        if (!g) {
            rotate(x);
        } else if ((g->left == p) == (p->left == x)) {
            rotate(p);
            rotate(x);
        } else {
            rotate(x);
            rotate(x);
        }
    }
    root = x;
}

void insert_node(Func f) {
    Node* node = new_node(f);
    if (!root) {
        root = node;
        return;
    }

    Node* cur = root;
    ull day = get_size(cur->left);
    while (cur) {
        apply_lazy_updates(cur);
        if (evaluate_at_day(node, day) > evaluate_at_day(cur, day)) {
            if (!cur->left) {
                cur->left = node;
                node->parent = cur;
                break;
            } else {
                cur = cur->left;
                day -= get_size(cur->right) + 1;
            }
        } else {
            if (!cur->right) {
                cur->right = node;
                node->parent = cur;
                break;
            } else {
                cur = cur->right;
                day += get_size(cur->left) + 1;
            }
        }
    }

    splay(node);
    if (node->right) {
        node->right->push_val += f.a;
        node->right->delay_days++;
    }
}

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
        sum += evaluate_at_day(cur, day++);
        printf("%llu\n", sum);
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
    qsort(funcs, n, sizeof(Func), cmp_Func);

    for (int i = 0; i < n; i++) {
        insert_node(funcs[i]);
    }
    output_total_value(n);

    free(funcs);
    return 0;
}
