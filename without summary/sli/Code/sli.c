#include <stdio.h>
#include <stdlib.h>

#define MIN(X, Y)  ( (X) < (Y) ? (X) : (Y) )  // macro for minimum
#define MAX(X, Y)  ( (X) > (Y) ? (X) : (Y) )  // macro for maximum

int main() {
    int n, s;
    scanf("%d %d\n", &n, &s);  // read number of slots and start slot

    int *l, *r, *d, *dir;
    l = malloc(sizeof(int) * n);    // cost arrays and direction flags
    r = malloc(sizeof(int) * n);
    d = malloc(sizeof(int) * n);
    dir = malloc(sizeof(int) * n);
    long long sum = 0;
    for (int i = 1; i < n; i++) {
        scanf("%d %d\n", l + i, r + i);        // read left/right push costs
        dir[i] = l[i] >= r[i];                 // choose cheaper direction: 0=left, 1=right
        d[i] = MAX(l[i], r[i]) - MIN(l[i], r[i]);  // extra cost if flipping direction
        sum += MIN(l[i], r[i]);                // accumulate base cost
    }

    int min_d = 0;
    d[min_d] = 1000001;  // sentinel for selecting flip candidate
    if (s < n) {
        for (int i = 1; i <= s; i++) {
            if (dir[i]) { min_d = 0; break; }     // if any right move exists, no need flip
            if (d[i] < d[min_d]) min_d = i;       // track cheapest flip candidate
        }
    }
    if (!min_d && s > 1) {
        for (int i = 1; i <= n - s + 1; i++) {
            if (!dir[i]) { min_d = 0; break; }    // if any left move exists, no need flip
            if (d[i] < d[min_d]) min_d = i;
        }
    }
    if (min_d) {
        dir[min_d] = !dir[min_d];                 // perform flip of direction
        sum += d[min_d];                          // add flip cost
    }
    printf("%lld\n", sum);                        // output minimal total energy

    int *cont = malloc(sizeof(int) * n);
    int idx = 0;
    for (int i = 1; i < n; i++) {
        if (dir[i] != dir[i - 1]) idx++;          // build segments of same-direction runs
        cont[idx]++;
    }

    printf("%d", s);                              // start output with initial slot
    int left_bound = 1, right_bound = n;
    char pass_s = 0;
    for (int i = 0; i <= idx; i++, dir[0] = !dir[0]) {
        if (!dir[0]) {
            left_bound += cont[i];                // adjust left boundary for right moves
            if (!pass_s && left_bound >= s) {
                left_bound++; pass_s = 1;         // skip over start slot once
            }
            int cur = left_bound;
            while (cont[i]--) printf(" %d", --cur == s ? --cur : cur);  // print right segment
        } else {
            right_bound -= cont[i];               // adjust right boundary for left moves
            if (!pass_s && right_bound <= s) {
                right_bound--; pass_s = 1;        // skip over start slot once
            }
            int cur = right_bound;
            while (cont[i]--) printf(" %d", ++cur == s ? ++cur : cur);  // print left segment
        }
    }

    free(l);
    free(r);
    free(d);
    free(dir);
    free(cont);
    return 0;
}
