#include <stdio.h>
#include <stdlib.h>

#define MAXN 100001

int main() {
    int n, d;
    if (scanf("%d %d\n", &n, &d) != 2) return 1;

    long long treats[MAXN] = {0}; // negative means net owes, positive means net is owed
    for (int i = 0; i < d; i++) {
        int x, y, t;
        if (scanf("%d %d %d\n", &x, &y, &t) != 3) return 1;
        treats[x] -= t;
        treats[y] += t;
    }

    long long sum = 0;
    for (int i = 1; i <= n; i++) {
        if (treats[i] > 0) sum += treats[i]; // total owed equals total owing
    }
    printf("%lld\n", sum);

    int i = 1, j = 1;
    while (1) {
        while (i <= n && treats[i] >= 0) i++;
        while (j <= n && treats[j] <= 0) j++;
        if (i > n) break; // when i passes n, j must also pass n

        int t = treats[i] + treats[j] > 0 ? -treats[i] : treats[j]; // transfer the smaller balance
        printf("%d %d %d\n", i, j, t);
        treats[i] += t;
        treats[j] -= t;
    }

    return 0;
}
