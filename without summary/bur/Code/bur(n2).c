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

void swap(Pair *a, Pair *b) {
    Pair t = *a;
    *a = *b;
    *b = t;
}

int partition(Pair *arr, int l, int r) {
    Pair pivot = arr[r];
    int i = l;
    for (int j = l; j < r; j++) {
        if (cmp_pair(&arr[j], &pivot) <= 0) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[r]);
    return i;
}

void sort_by_kth(Pair *arr, int l, int r, int k) {
    while (l < r) {
        int pivot_idx = l + rand() % (r - l + 1);
        swap(&arr[pivot_idx], &arr[r]);
        int p = partition(arr, l, r);
        
        int left_size = p - l + 1;
        if (k == left_size) {
            return;
        } else if (k < left_size) {
            r = p - 1;
        } else {
            k -= left_size;
            l = p + 1;
        }
    }
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
        sort_by_kth(p_queue, 0, i - 1, i / 2);

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
