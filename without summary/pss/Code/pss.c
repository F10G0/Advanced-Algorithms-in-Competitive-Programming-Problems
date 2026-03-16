#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) ( ( (a) < (b) ) ? (a) : (b) )
#define MAX(a, b) ( ( (a) > (b) ) ? (a) : (b) )

long long *max_days, *pushdown;

void push(int node, long long push_val) {
    max_days[node] += push_val;
    pushdown[node] += push_val;
}

void update_range(int q_from, int q_to, int node, int n_from, int n_to, int value) {
    if (q_from <= n_from && n_to <= q_to) {
        push(node, value);
        return;
    }

    int left_sub = 2 * node, right_sub = left_sub + 1;
    push(left_sub, pushdown[node]);
    push(right_sub, pushdown[node]);
    pushdown[node] = 0;

    int mid = (n_from + n_to) >> 1;
    if (q_from <= mid) update_range(q_from, MIN(q_to, mid), left_sub, n_from, mid, value);
    if (mid < q_to) update_range(MAX(q_from, mid + 1), q_to, right_sub, mid + 1, n_to, value);
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
    
    max_days = calloc(4 * n, sizeof(long long));
    pushdown = calloc(4 * n, sizeof(long long));
    int *last = calloc(m + 1, sizeof(int));
    int *last_last = calloc(m + 1, sizeof(int));

    long long max = 0;
    for (int i = 1; i <= n; i++) {
        int j = f[i];
        int l1 = last[j];
        int l2 = last_last[j];
        
        if (l2 < l1) update_range(l2 + 1, l1, 1, 1, n, -w[j]);
        update_range(l1 + 1, i, 1, 1, n, w[j]);

        last_last[j] = l1;
        last[j] = i;
        
        max = MAX(max, max_days[1]);
    }
    printf("%lld\n", max);

    return 0;
}
