#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) ( ( (a) < (b) ) ? (a) : (b) )
#define MAX(a, b) ( ( (a) > (b) ) ? (a) : (b) )

long long *max_days, *pushdown;

// apply a lazy increment to a single node
void push(int node, long long push_val) {
    max_days[node] += push_val;   // add value to this segment's max
    pushdown[node] += push_val;   // accumulate lazy tag
}

// add `value` to all positions in [q_from..q_to]
void update_range(int q_from, int q_to, int node, int n_from, int n_to, int value) {
    // if current segment [n_from..n_to] is fully covered, apply and return
    if (q_from <= n_from && n_to <= q_to) {
        push(node, value);
        return;
    }

    int left_sub = 2 * node, right_sub = left_sub + 1;
    // propagate this node's pending lazy to children
    push(left_sub, pushdown[node]);
    push(right_sub, pushdown[node]);
    pushdown[node] = 0;  // clear after pushing down

    int mid = (n_from + n_to) >> 1;
    if (q_from <= mid)
        update_range(q_from, MIN(q_to, mid), left_sub, n_from, mid, value);
    if (mid < q_to)
        update_range(MAX(q_from, mid + 1), q_to, right_sub, mid + 1, n_to, value);

    // recalculate this node's max from its children
    max_days[node] = MAX(max_days[left_sub], max_days[right_sub]);
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1;

    int *f = malloc(sizeof(int) * (n + 1));
    int *w = malloc(sizeof(int) * (m + 1));
    for (int i = 1; i <= n; i++) {
        if (scanf("%d", f + i) != 1) return 1;
    }
    for (int j = 1; j <= m; j++) {
        if (scanf("%d", w + j) != 1) return 1;
    }
    
    // allocate segment tree arrays (4*n is safe for tree size)
    max_days = calloc(4 * n, sizeof(long long));
    pushdown = calloc(4 * n, sizeof(long long));
    int *last = calloc(m + 1, sizeof(int));        // last occurrence
    int *last_last = calloc(m + 1, sizeof(int));   // second last occurrence

    long long max = 0;
    for (int i = 1; i <= n; i++) {
        int j = f[i];
        int l1 = last[j];
        int l2 = last_last[j];
        
        // if this scent appeared before, remove previous contribution
        if (l2 < l1)
            update_range(l2 + 1, l1, 1, 1, n, -w[j]);
        // add new contribution for current appearance
        update_range(l1 + 1, i, 1, 1, n, w[j]);

        last_last[j] = l1;
        last[j] = i;
        
        // root node holds the current maximum over all intervals
        max = MAX(max, max_days[1]);
    }
    printf("%lld\n", max);

    return 0;
}
