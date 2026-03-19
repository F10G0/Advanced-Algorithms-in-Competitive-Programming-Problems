#include <stdio.h>
#include <stdlib.h>

#define MIN(X, Y)  ( (X) < (Y) ? (X) : (Y) )
#define MAX(X, Y)  ( (X) > (Y) ? (X) : (Y) )

int main() {
    int n, s;
    scanf("%d %d\n", &n, &s);

    int *l, *r, *d, *dir;
    l = malloc(sizeof(int) * n);
    r = malloc(sizeof(int) * n);
    d = malloc(sizeof(int) * n);
    dir = malloc(sizeof(int) * n);
    long long sum = 0;
    for (int i = 1; i < n; i++) {
        scanf("%d %d\n", l + i, r + i);
        dir[i] = l[i] >= r[i];
        d[i] = MAX(l[i], r[i]) - MIN(l[i], r[i]);
        sum += MIN(l[i], r[i]);
    }

    int min_d = 0;
    d[min_d] = 1000001;
    if (s < n) {
        for (int i = 1; i <= s; i++) {
            if (dir[i]) { min_d = 0; break; }
            if (d[i] < d[min_d]) min_d = i;
        }
    }
    if (!min_d && s > 1) {
        for (int i = 1; i <= n - s + 1; i++) {
            if (!dir[i]) { min_d = 0; break; }
            if (d[i] < d[min_d]) min_d = i;
        }
    }
    if (min_d) {
        dir[min_d] = !dir[min_d];
        sum += d[min_d];
    }
    printf("%lld\n", sum);

    int *cont = malloc(sizeof(int) * n);
    int idx = 0;
    for (int i = 1; i < n; i++) {
        if (dir[i] != dir[i - 1]) idx++;
        cont[idx]++;
    }

    printf("%d", s);
    int left_bound = 1, right_bound = n;
    char pass_s = 0;
    for (int i = 0; i <= idx; i++, dir[0] = !dir[0]) {
        if (!dir[0]) {
            left_bound += cont[i];
            if (!pass_s && left_bound >= s) {
                left_bound++; pass_s = 1;
            }
            int cur = left_bound;
            while (cont[i]--) printf(" %d", --cur == s ? --cur : cur);
        } else {
            right_bound -= cont[i];
            if (!pass_s && right_bound <= s) {
                right_bound--; pass_s = 1;
            }
            int cur = right_bound;
            while (cont[i]--) printf(" %d", ++cur == s ? ++cur : cur);
        }
    }

    free(l);
    free(r);
    free(d);
    free(dir);
    free(cont);
    return 0;
}
