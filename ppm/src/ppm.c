#include <stdio.h>
#include <stdlib.h>

#define MAXN 100000
#define MAXK 26
#define MAX(a, b) (( (a) > (b) ) ? (a) : (b))

int heap[MAXN + MAXK], htop = 0;
long long sum = 0;

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void push(int val) {
    heap[++htop] = val;
    sum += val;
    if (htop == 1) return;

    int i = htop;
    char is_min_lvl = __builtin_clz(htop) & 1;
    if (is_min_lvl == (heap[i >> 1] < heap[i])) {
        swap(&heap[i], &heap[i >> 1]);
        i >>= 1;
        is_min_lvl = !is_min_lvl;
    }
    for (; i > 3; i >>= 2) {
        if (is_min_lvl == (heap[i >> 2] < heap[i])) break;
        swap(&heap[i], &heap[i >> 2]);
    }
}

int pop(int i, char is_min) {
    int ret = heap[i];
    heap[i] = heap[htop--];

    for (int nxt; (nxt = i << 1) <= htop; i = nxt) {
        if (nxt + 1 <= htop && (is_min == (heap[nxt + 1] < heap[nxt]))) nxt++;
        for (int j = 0; j < 4; j++) {
            int gc = (i << 2) + j;
            if (gc <= htop && (is_min ? heap[gc] <= heap[nxt] : heap[gc] >= heap[nxt])) nxt = gc;
        }

        if (is_min == (heap[i] < heap[nxt])) break;
        swap(&heap[i], &heap[nxt]);
        if (nxt >= i << 2 && (is_min == (heap[nxt] > heap[nxt >> 1]))) swap(&heap[nxt], &heap[nxt >> 1]);
    }
    
    sum -= ret;
    return ret;
}

int pop_min() {
    return pop(1, 1);
}

int pop_max() {
    return pop(heap[2] > heap[3] ? 2 : 3, 0);
}

int cmp(const void *a, const void *b) {
    return *(const int*)a - *(const int*)b;
}

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 1;

    int w[MAXK];
    for (int i = 0; i < k; i++) {
        if (scanf("%d", w + i) != 1) return 1;
    }
    qsort(w, k, sizeof(int), cmp);
    
    push(0);
    while (htop < n || !heap[1]) {
        int min = pop_min();
        for (int i = 0; i < k; i++) {
            push(min + w[i]);
        }
    }
    while (htop > n) {
        pop_max();
    }
    
    int popped[MAXK], pop_cnt;
    int d;
    do {
        d = w[0];
        int min = pop_min();
        popped[0] = min;
        for (pop_cnt = 1; pop_cnt < k; pop_cnt++) {
            int dd = min + w[pop_cnt] - MAX(heap[2], heap[3]);
            if (dd >= 0) break;
            popped[pop_cnt] = pop_max();
            d += dd;
        }
        for (int i = 0; i < pop_cnt; i++) {
            push(d >= 0 ? popped[i] : min + w[i]);
        }
    } while (d < 0);

    printf("%lld\n", sum);

    return 0;
}
