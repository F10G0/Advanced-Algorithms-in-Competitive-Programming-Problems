#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int limit;
int *tmp_min_heap, tmp_minh_top;
int *tmp_max_heap, tmp_maxh_top;
int *tmp_id_val, tmp_id;
char *tmp_valid;
void reload();

void swap_int(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_ptr(void **a, void **b) {
    void *tmp = *a;
    *a = *b;
    *b = tmp;
}

void heap_push(int val, int *min_heap, int *minh_top, int *max_heap, int *maxh_top, int *id_val, int *id, char *valid) {
    min_heap[++*minh_top] = *id;
    max_heap[++*maxh_top] = *id;
    id_val[*id] = val;
    valid[(*id)++] = 1;
    for (int i = *minh_top; i > 1; i >>= 1) {
        if (id_val[min_heap[i >> 1]] <= id_val[min_heap[i]]) break;
        swap_int(&min_heap[i], &min_heap[i >> 1]);
    }
    for (int i = *maxh_top; i > 1; i >>= 1) {
        if (id_val[max_heap[i >> 1]] >= id_val[max_heap[i]]) break;
        swap_int(&max_heap[i], &max_heap[i >> 1]);
    }
    if (*id == limit - 1) reload();
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

void reload() {
    tmp_minh_top = 0;
    tmp_maxh_top = 0;
    tmp_id = 0;
    memset(tmp_valid, 0, limit);

    for (int i = 1; i <= minh_top; i++) {
        int id = min_heap[i];
        if (valid[id]) heap_push(id_val[id], tmp_min_heap, &tmp_minh_top, tmp_max_heap, &tmp_maxh_top, tmp_id_val, &tmp_id, tmp_valid);
    }
    swap_ptr((void **)&min_heap, (void **)&tmp_min_heap);
    minh_top = tmp_minh_top;
    swap_ptr((void **)&max_heap, (void **)&tmp_max_heap);
    maxh_top = tmp_maxh_top;
    swap_ptr((void **)&id_val, (void **)&tmp_id_val);
    id = tmp_id;
    swap_ptr((void **)&valid, (void **)&tmp_valid);
}

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 1;

    int w[MAXK];
    for (int i = 0; i < k; i++) {
        if (scanf("%d", w + i) != 1) return 1;
    }
    qsort(w, k, sizeof(int), cmp);

    limit = n * 2 + 1;
    min_heap = malloc(sizeof(int) * limit);
    max_heap = malloc(sizeof(int) * limit);
    id_val = malloc(sizeof(int) * limit);
    valid = malloc(sizeof(char) * limit);

    tmp_min_heap = malloc(sizeof(int) * limit);
    tmp_max_heap = malloc(sizeof(int) * limit);
    tmp_id_val = malloc(sizeof(int) * limit);
    tmp_valid = malloc(sizeof(char) * limit);

    for (int i = 0; i < k; i++) {
        heap_push(w[i], min_heap, &minh_top, max_heap, &maxh_top, id_val, &id, valid);
    }

    while (minh_top < n) {
        int min = heap_min();
        heap_pop(MIN);
        for (int i = 0; i < k; i++) {
            heap_push(min + w[i], min_heap, &minh_top, max_heap, &maxh_top, id_val, &id, valid);
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
                heap_push(tmp[j], min_heap, &minh_top, max_heap, &maxh_top, id_val, &id, valid);
            }
            break;
        }
        heap_pop(MIN);
        for (int j = 0; j < i; j++) {
            heap_push(min + w[j], min_heap, &minh_top, max_heap, &maxh_top, id_val, &id, valid);
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
