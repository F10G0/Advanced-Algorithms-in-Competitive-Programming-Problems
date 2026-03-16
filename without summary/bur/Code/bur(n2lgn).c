#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int val;
} Pair;

int cmp_pair(const void *a, const void *b) {
    const Pair *pa = a, *pb = b;
    if (pa->val != pb->val) return pa->val - pb->val;
    return pa->id - pb->id;
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1;

    int *a = malloc(sizeof(int) * n);
    for (int i = n - 1; i >= 0; i--) {
        if (scanf("%d", a + i) != 1) return 1;
    }
    
    Pair *p_queue = malloc(sizeof(Pair) * n);
    int *b = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            p_queue[j] = (Pair){ j, b[j] + a[j] };
        }
        qsort(p_queue, i, sizeof(Pair), cmp_pair);

        int rest = m;
        for (int j = 0; j < i / 2; j++) {
            rest -= p_queue[j].val;
        }

        if (rest >= 0) {
            for (int j = 0; j < i; j++) {
                b[p_queue[j].id] = j < i / 2 ? p_queue[j].val : 0;
            }
            b[i] = rest;
        } else b[i] = -a[i];
    }
    
    for (int i = n - 1; i >= 0; i--) {
        printf("%d ", b[i] < 0 ? -1 : b[i]);
    }

    return 0;
}
