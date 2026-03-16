#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXA 64

typedef struct Node {
    int key;            // node key (index of cat)
    int priority;       // heap priority for treap balance
    int size;           // size of subtree rooted at this node
    struct Node *left;  // left child pointer
    struct Node *right; // right child pointer
} Node;

int get_size(Node *t) {
    return t ? t->size : 0;  // return subtree size or 0 if null
}

void update_size(Node* t) {
    t->size = get_size(t->left) + get_size(t->right) + 1;  // recalc size
}

Node* rotate_left(Node* a) {
    Node* b = a->right;             // right child becomes new root
    a->right = b->left;             // shift b's left subtree
    b->left = a;                    // attach a as left child
    update_size(a);
    update_size(b);
    return b;                       // return new root
}

Node* rotate_right(Node* b) {
    Node* a = b->left;              // left child becomes new root
    b->left = a->right;             // shift a's right subtree
    a->right = b;                   // attach b as right child
    update_size(b);
    update_size(a);
    return a;                       // return new root
}

Node* insert(Node* t, int key) {
    if (!t) {
        t = malloc(sizeof(Node));
        *t = (Node){ key, rand(), 1, NULL, NULL };  // new leaf
        return t;
    }
    if (key < t->key) {
        t->left = insert(t->left, key);             // insert in left subtree
        if (t->left->priority < t->priority)        // heap property
            t = rotate_right(t);
    } else {
        t->right = insert(t->right, key);           // insert in right subtree
        if (t->right->priority < t->priority)
            t = rotate_left(t);
    }
    update_size(t);                                 // update subtree size
    return t;
}

void split(Node* t, int key, Node** l, Node** r) {
    if (!t) {
        *l = *r = NULL;                             // empty split
        return;
    }
    if (t->key <= key) {
        split(t->right, key, &t->right, r);         // split right subtree
        *l = t;                                     // t goes to left part
    } else {
        split(t->left, key, l, &t->left);           // split left subtree
        *r = t;                                     // t goes to right part
    }
    update_size(t);                                // update size after split
}

void merge(Node** a, Node** b) {
    if (!*b) return;                               // nothing to merge
    if (!*a) {                                     // if a is empty
        *a = *b;
        return;
    }
    if ((*a)->priority > (*b)->priority) {         // choose higher priority
        Node * tmp = *a;
        *a = *b;
        *b = tmp;
    }
    Node *l, *r;
    split(*b, (*a)->key, &l, &r);                  // split b around a->key
    merge(&(*a)->left, &l);                        // merge left parts
    merge(&(*a)->right, &r);                       // merge right parts
    update_size(*a);                               // update size
    return;
}

int count_le(Node* t, int k) {
    if (!t) return 0;                                      // no nodes
    if (k < t->key) return count_le(t->left, k);           // look left
    return get_size(t->left) + 1 + count_le(t->right, k);  // include this node
}

int n, m;
Node *table[MAXA + 2][MAXA + 1];
Node *next_table[MAXA + 2][MAXA + 1];

void move_to(int c, int a, Node **src) {
    int dest = c + a > MAXA ? MAXA + 1 : c + a;
    merge(&next_table[dest][a], src);              // merge subtree into next_table
}

int *b;
void save_result(Node *t, int c) {
    if (!t) return;
    b[t->key] = c;                                 // record cost for this node
    save_result(t->left, c);
    save_result(t->right, c);
}

// Core function: simulate proposals for cat i
int func(int i, char record_round) {
    int remaining_bundles = m, remaining_cats = i >> 1;
    int split_round = -1, split_cnt;
    // 1) Determine split_round and remainder cost
    for (int c = 0; c <= MAXA + 1; c++) {
        split_cnt = remaining_cats;
        if (!remaining_cats) {
            split_round = c;
            break;
        }
        for (int a = 1; a <= MAXA; a++) {
            int cnt = get_size(table[c][a]);
            if (remaining_cats >= cnt) {
                remaining_bundles -= cnt * c;
                remaining_cats -= cnt;
            } else {
                remaining_bundles -= remaining_cats * c;
                split_round = c;
                break;
            }
        }
        if (split_round != -1) break;
    }
    if (!record_round && remaining_bundles < 0) return -1;      // negative means cast away

    int c;
    // 2) Merge groups fully before split_round
    for (c = 0; c < split_round; c++) {
        for (int a = 1; a <= MAXA; a++) {
            if (record_round)
                save_result(table[c][a], c);
            else if (table[c][a])
                move_to(c, a, &table[c][a]);
        }
    }
    // 3) Handle the split_round group partially
    if (split_cnt) {
        int l = 0, u = n - 1;
        while (l < u) {
            int mid = (l + u) >> 1;
            int sum = 0;
            for (int a = 1; a <= MAXA; a++)
                sum += count_le(table[c][a], mid);
            if (sum < split_cnt) l = mid + 1;
            else u = mid;
        }
        int split_key = l;
        for (int a = 1; a <= MAXA; a++) {
            if (!table[c][a]) continue;
            Node *L = NULL, *R = NULL;
            split(table[c][a], split_key, &L, &R);
            if (record_round) {
                save_result(L, c);
                save_result(R, 0);
            } else {
                move_to(c, a, &L);
                move_to(0, a, &R);
            }
        }
        c++;
    }
    // 4) Merge remaining groups after split_round
    for (; c <= MAXA + 1; c++) {
        for (int a = 1; a <= MAXA; a++) {
            if (record_round)
                save_result(table[c][a], 0);
            else if (table[c][a])
                move_to(0, a, &table[c][a]);
        }
    }
    // 5) Swap next_table into table for next iteration
    for (int c = 0; c <= MAXA + 1; c++) {
        for (int a = 1; a <= MAXA; a++) {
            table[c][a] = next_table[c][a];
            next_table[c][a] = NULL;
        }
    }
    return (b[i] = remaining_bundles < 0 ? -1 : remaining_bundles);
}

int main() {
    if (scanf("%d %d", &n, &m) != 2) return 1;

    int *a = malloc(sizeof(int) * n);
    for (int i = n - 1; i >= 0; i--) {
        if (scanf("%d", a + i) != 1) return 1;
    }

    srand((unsigned)time(NULL));
    b = malloc(sizeof(int) * n);
    // initialize tables
    for (int i = 0; i <= MAXA + 1; i++)
        for (int j = 1; j <= MAXA; j++)
            table[i][j] = next_table[i][j] = NULL;

    int minus_cnt = 0;
    // run through cats except last
    for (int i = 0; i < n - 1; i++) {
        int remaining_bundles = func(i, 0);
        if (remaining_bundles < 0) {
            table[0][a[i]] = insert(table[0][a[i]], i);
            minus_cnt++;
        } else {
            int dest = (remaining_bundles + a[i]) > MAXA ? (MAXA + 1) : (remaining_bundles + a[i]);
            table[dest][a[i]] = insert(table[dest][a[i]], i);
            minus_cnt = 0;
        }
    }
    // handle last cat with record
    if (func(n - 1, 1) >= 0) {
        for (int i = n - 1; i >= 0; i--) {
            printf("%d ", b[i]);
        }
        return 0;
    }

    // fallback for single -1 sequence
    minus_cnt++;
    // reset tables and re-run up to failure point
    for (int i = 0; i <= MAXA + 1; i++)
        for (int j = 1; j <= MAXA; j++)
            table[i][j] = next_table[i][j] = NULL;

    for (int i = 0; i < n - minus_cnt - 1; i++) {
        int remaining_bundles = func(i, 0);
        if (remaining_bundles < 0)
            table[0][a[i]] = insert(table[0][a[i]], i);
        else {
            int dest = (remaining_bundles + a[i]) > MAXA ? (MAXA + 1) : (remaining_bundles + a[i]);
            table[dest][a[i]] = insert(table[dest][a[i]], i);
        }
    }

    func(n - minus_cnt - 1, 1);
    int i;
    // print trailing -1s
    for (i = n - 1; i >= n - minus_cnt; i--)
        printf("-1 ");
    // then print remaining_bundles of b[]
    for (; i >= 0; i--)
        printf("%d ", b[i]);

    return 0;
}
