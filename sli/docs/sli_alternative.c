#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, s;
    scanf("%d %d\n", &n, &s);

    int *l = malloc(sizeof(int) * (n - 1));
    int *r = malloc(sizeof(int) * (n - 1));
    int *d = malloc(sizeof(int) * (n - 1));
    for (int i = 0; i < n - 1; i++) {
        scanf("%d %d\n", l + i, r + i);
        d[i] = r[i] - l[i];
    }

    char *dir = malloc(sizeof(char) * (n - 1));
    for (int i = 0; i < n - 1; i++) {
        dir[i] = l[i] < r[i];
    }

    char has_right = 0;
    for (int i = 0; i <= s - 1; i++) {
        if (!dir[i]) {
            has_right = 1;
            break;
        }
    }
    if (s < n && !has_right) {
        int min_d = 0;
        for (int i = 1; i <= s - 1; i++) {
            if (d[i] < d[min_d]) min_d = i;
        }
        dir[min_d] = !dir[min_d];
    }

    char has_left = 0;
    for (int i = 0; i <= n - s; i++) {
        if (dir[i]) {
            has_left = 1;
            break;
        }
    }
    if (s > 1 && !has_left) {
        int max_d = 0;
        for (int i = 1; i <= n - s; i++) {
            if (d[i] > d[max_d]) max_d = i;
        }
        dir[max_d] = !dir[max_d];
    }

    long long sum = 0;
    for (int i = 0; i < n - 1; i++) {
        sum += dir[i] ? l[i] : r[i];
    }
    printf("%lld\n", sum);

    int *cont_lefts = malloc(sizeof(int) * (n - 1));
    int *cont_rights = malloc(sizeof(int) * (n - 1));
    int cnt = 0;
    for (int i = n - 2; i >= 0; i--) {
        if (!cnt || dir[i] != dir[i + 1]) cnt = 0;
        if (dir[i]) cont_lefts[i] = ++cnt;
        else cont_rights[i] = ++cnt;
    }

    printf("%d", s);
    int left_bound = 1, right_bound = n;
    int cur = s;
    for (int i = 0; i < n - 1; i++) {
        if (cont_lefts[i]) {
            if (!i || !cont_lefts[i - 1]) {
                left_bound += cont_lefts[i];
                if (left_bound > s && left_bound - cont_lefts[i] <= s) left_bound++;
                cur = left_bound;
            }
            printf(" %d", --cur == s ? --cur : cur);
        } else {
            if (!i || !cont_rights[i - 1]) {
                right_bound -= cont_rights[i];
                if (right_bound < s && right_bound + cont_rights[i] >= s) right_bound--;
                cur = right_bound;
            }
            printf(" %d", ++cur == s ? ++cur : cur);
        }
    }

    free(l);
    free(r);
    free(d);
    free(dir);
    free(cont_lefts);
    free(cont_rights);
}
