#include <stdio.h>
#include <stdlib.h>

#define MAXN 100000
#define MAXK 26
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int heap[MAXN + MAXK], htop = 0;
long long sum = 0;

// swap two integers in the heap
void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Insert a new value into the min-max heap and update running sum
void push(int val) {
    heap[++htop] = val;
    sum += val;
    if (htop == 1) return;

    int i = htop;
    // determine whether current level is a min-level (even) or max-level (odd)
    char is_min_lvl = __builtin_clz(htop) & 1;
    // if at wrong type of level relative to parent, swap and flip level type
    if (is_min_lvl == (heap[i >> 1] < heap[i])) {
        swap(&heap[i], &heap[i >> 1]);
        i >>= 1;
        is_min_lvl = !is_min_lvl;
    }
    // continue up-floating along grandparent links
    for (; i > 3; i >>= 2) {
        if (is_min_lvl == (heap[i >> 2] < heap[i])) break;
        swap(&heap[i], &heap[i >> 2]);
    }
}

// Generalized pop: remove either a min or max element at index i
int pop(int i, char is_min) {
    int ret = heap[i];
    heap[i] = heap[htop--];

    // down-heapify from position i, alternating between child and grandchild levels
    for (int nxt; (nxt = i << 1) <= htop; i = nxt) {
        // compare with sibling at child level
        if (nxt + 1 <= htop && (is_min == (heap[nxt + 1] < heap[nxt]))) nxt++;
        // compare with up to four grandchildren
        for (int j = 0; j < 4; j++) {
            int gc = (i << 2) + j;
            if (gc <= htop && (is_min ? heap[gc] <= heap[nxt] : heap[gc] >= heap[nxt])) nxt = gc;
        }
        // if in correct heap order, stop
        if (is_min == (heap[i] < heap[nxt])) break;
        swap(&heap[i], &heap[nxt]);
        // if we moved down two levels, ensure cross-level order with parent
        if (nxt >= i << 2 && (is_min == (heap[nxt] > heap[nxt >> 1]))) swap(&heap[nxt], &heap[nxt >> 1]);
    }

    sum -= ret;
    return ret;
}

// Remove and return minimum element
int pop_min() {
    return pop(1, 1);
}

// Remove and return maximum element (either child 2 or 3)
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

    // seed heap with zero-cost root
    push(0);
    // build up at least n nonzero words
    while (htop < n || !heap[1]) {
        int min = pop_min();
        for (int i = 0; i < k; i++) {
            push(min + w[i]);
        }
    }
    // trim to exactly n leaves by removing largest extras
    while (htop > n) {
        pop_max();
    }

    // greedy replacement phase
    int popped[MAXK], pop_cnt;
    int d;
    do {
        d = w[0];
        int min = pop_min();
        popped[0] = min;
        // attempt to replace up to k-1 largest leaves
        for (pop_cnt = 1; pop_cnt < k; pop_cnt++) {
            int dd = min + w[pop_cnt] - MAX(heap[2], heap[3]);
            if (dd >= 0) break;
            popped[pop_cnt] = pop_max();
            d += dd;
        }
        // either rollback or commit replacement
        for (int i = 0; i < pop_cnt; i++) {
            push(d >= 0 ? popped[i] : min + w[i]);
        }
    } while (d < 0);

    // output final total cost
    printf("%lld\n", sum);
    return 0;
}
