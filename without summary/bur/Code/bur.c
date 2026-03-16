#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXA 64

typedef struct Node {
    int key;
    int priority;
    int size;
    struct Node *left;
    struct Node *right;
} Node;

int get_size(Node *t) {
    return t ? t->size : 0;
}

void update_size(Node* t) {
    t->size = get_size(t->left) + get_size(t->right) + 1;
}

Node* rotate_left(Node* a) {
    Node* b = a->right;
    a->right = b->left;
    b->left = a;
    update_size(a);
    update_size(b);
    return b;
}

Node* rotate_right(Node* b) {
    Node* a = b->left;
    b->left = a->right;
    a->right = b;
    update_size(b);
    update_size(a);
    return a;
}

Node* insert(Node* t, int key) {
    if (!t) {
        t = malloc(sizeof(Node));
        *t = (Node){ key, rand(), 1, NULL, NULL };
        return t;
    }
    if (key < t->key) {
        t->left = insert(t->left, key);
        if (t->left->priority < t->priority) t = rotate_right(t);
    } else {
        t->right = insert(t->right, key);
        if (t->right->priority < t->priority) t = rotate_left(t);
    }
    update_size(t);
    return t;
}

void split(Node* t, int key, Node** l, Node** r) {
    if (!t) {
        *l = *r = NULL;
        return;
    }
    if (t->key <= key) {
        split(t->right, key, &t->right, r);
        *l = t;
    } else {
        split(t->left, key, l, &t->left);
        *r = t;
    }
    update_size(t);
}

void merge(Node** a, Node** b) {
    if (!*b) return;
    if (!*a) {
        *a = *b;
        return;
    }
    if ((*a)->priority > (*b)->priority) {
        Node * tmp = *a;
        *a = *b;
        *b = tmp;
    }
    Node *l, *r;
    split(*b, (*a)->key, &l, &r);
    merge(&(*a)->left, &l);
    merge(&(*a)->right, &r);
    update_size(*a);
    return;
}

int count_le(Node* t, int k) {
    if (!t) return 0;
    if (k < t->key) return count_le(t->left, k);
    return get_size(t->left) + 1 + count_le(t->right, k);
}

int n, m;
Node *table[MAXA + 2][MAXA + 1];
Node *next_table[MAXA + 2][MAXA + 1];

void move_to(int c, int a, Node **src) {
    int dest = c + a > MAXA ? MAXA + 1 : c + a;
    merge(&next_table[dest][a], src);
}

int *b;
void save_result(Node *t, int c) {
    if (!t) return;
    b[t->key] = c;
    save_result(t->left, c);
    save_result(t->right, c);
}

int func(int i, char record_round) {
    int remaining_bundles = m, remaining_cats = i >> 1;
    int split_round = -1, split_cnt;
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
    if (!record_round && remaining_bundles < 0) return -1;

    int c;
    for (c = 0; c < split_round; c++) {
        for (int a = 1; a <= MAXA; a++) {
            if (record_round) save_result(table[c][a], c);
            else if (table[c][a]) move_to(c, a, &table[c][a]);
        }
    }
    if (split_cnt) {
        int l = 0, u = n - 1;
        while (l < u) {
            int mid = (l + u) >> 1;
            int sum = 0;
            for (int a = 1; a <= MAXA; a++) {
                sum += count_le(table[c][a], mid);
            }
            if (sum < split_cnt) l = mid + 1;
            else u = mid;
        }
        int split_key = l;
        
        for (int a = 1; a <= MAXA; a++) {
            if (!table[c][a]) continue;
            Node *l, *r;
            split(table[c][a], split_key, &l, &r);
            if (record_round) {
                save_result(l, c);
                save_result(r, 0);
            } else {
                move_to(c, a, &l);
                move_to(0, a, &r);
            }
        }
        c++;
    }
    for (; c <= MAXA + 1; c++) {
        for (int a = 1; a <= MAXA; a++) {
            if (record_round) save_result(table[c][a], 0);
            else if (table[c][a]) move_to(0, a, &table[c][a]);
        }
    }

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
    for (int i = 0; i <= MAXA + 1; i++) {
        for (int j = 1; j <= MAXA; j++) {
            table[i][j] = next_table[i][j] = NULL;
        }
    }

    int minus_cnt = 0;
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
    if (func(n - 1, 1) >= 0) {
        for (int i = n - 1; i >= 0; i--) {
            printf("%d ", b[i]);
        }
        return 0;
    }

    minus_cnt++;
    for (int i = 0; i <= MAXA + 1; i++) {
        for (int j = 1; j <= MAXA; j++) {
            table[i][j] = next_table[i][j] = NULL;
        }
    }
    
    for (int i = 0; i < n - minus_cnt - 1; i++) {
        int remaining_bundles = func(i, 0);
        if (remaining_bundles < 0) {
            table[0][a[i]] = insert(table[0][a[i]], i);
        } else {
            int dest = (remaining_bundles + a[i]) > MAXA ? (MAXA + 1) : (remaining_bundles + a[i]);
            table[dest][a[i]] = insert(table[dest][a[i]], i);
        }
    }

    func(n - minus_cnt - 1, 1);
    int i;
    for (i = n - 1; i >= n - minus_cnt; i--) {
        printf("-1 ");
    }
    for (; i >= 0; i--) {
        printf("%d ", b[i]);
    }

    return 0;
}
