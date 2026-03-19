#include <stdio.h>
#include <stdlib.h>

#define MAXN 100000
#define MAXK 26

#define MIN 1
#define MAX 0

int cmp(const void *a, const void *b) {
    return *(const int*)a - *(const int*)b;
}

int *min_heap, minh_top = 0;
int *max_heap, maxh_top = 0;
int *id_val, id = 0;
char *valid;

void heap_push(int val) {
    min_heap[++minh_top] = id;
    max_heap[++maxh_top] = id;
    id_val[id] = val;
    valid[id++] = 1;
    for (int i = minh_top; i > 1; i >>= 1) {
        if (id_val[min_heap[i >> 1]] <= id_val[min_heap[i]]) break;
        int temp = min_heap[i];
        min_heap[i] = min_heap[i >> 1];
        min_heap[i >> 1] = temp;
    }
    for (int i = maxh_top; i > 1; i >>= 1) {
        if (id_val[max_heap[i >> 1]] >= id_val[max_heap[i]]) break;
        int temp = max_heap[i];
        max_heap[i] = max_heap[i >> 1];
        max_heap[i >> 1] = temp;
    }
}

void clean(int *heap, int *htop, char min) {
    while (!valid[heap[1]]) {
        int last = heap[(*htop)--];
        int i, child;
        for (i = 1; (child = i << 1) <= *htop; i = child) {
            if (child + 1 <= *htop && (min ? id_val[heap[child + 1]] < id_val[heap[child]] : id_val[heap[child + 1]] > id_val[heap[child]])) ++child;
            if (min ? id_val[last] <= id_val[heap[child]] : id_val[last] >= id_val[heap[child]]) break;
            heap[i] = heap[child];
        }
        heap[i] = last;
    }
}

void heap_pop(char min) {
    int *heap = min ? min_heap : max_heap, *htop = min ? &minh_top : &maxh_top;
    clean(heap, htop, min);
    valid[heap[1]] = 0;
    int last = heap[(*htop)--];
    int i, child;
    for (i = 1; (child = i << 1) <= *htop; i = child) {
        if (child + 1 <= *htop && (min ? id_val[heap[child + 1]] < id_val[heap[child]] : id_val[heap[child + 1]] > id_val[heap[child]])) ++child;
        if (min ? id_val[last] <= id_val[heap[child]] : id_val[last] >= id_val[heap[child]]) break;
        heap[i] = heap[child];
    }
    heap[i] = last;
}

int heap_min() {
    clean(min_heap, &minh_top, MIN);
    return id_val[min_heap[1]];
}

int heap_max() {
    clean(max_heap, &maxh_top, MAX);
    return id_val[max_heap[1]];
}

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 1;

    int w[MAXK];
    for (int i = 0; i < k; i++) {
        if (scanf("%d", w + i) != 1) return 1;
    }
    qsort(w, k, sizeof(int), cmp);

    min_heap = malloc(sizeof(int) * (n * k + 1));
    max_heap = malloc(sizeof(int) * (n * k + 1));
    id_val = malloc(sizeof(int) * (n * k + 1));
    valid = malloc(sizeof(char) * (n * k + 1));

    for (int i = 0; i < k; i++) {
        heap_push(w[i]);
    }

    while (minh_top < n) {
        int min = heap_min();
        heap_pop(MIN);
        for (int i = 0; i < k; i++) {
            heap_push(min + w[i]);
        }
    }
    int cnt = minh_top;
    while (cnt-- > n) {
        heap_pop(MAX);
    }

    while (1) {
        int min = heap_min();
        int d = w[0];
        int tmp[MAXK];
        int i;
        for (i = 1; i < k; i++) {
            int max = heap_max();
            int val = min + w[i];
            if (val >= max) break;
            tmp[i] = max;
            d += val - max;
            heap_pop(MAX);
        }
        if (d >= 0) {
            for (int j = 1; j < i; j++) {
                heap_push(tmp[j]);
            }
            break;
        }
        heap_pop(MIN);
        for (int j = 0; j < i; j++) {
            heap_push(min + w[j]);
        }
    }

    long long sum = 0;
    while (n--) {
        sum += heap_min();
        heap_pop(MIN);
    }
    printf("%lld\n", sum);

    return 0;
}
