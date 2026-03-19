#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long ull;

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 1;

    ull *jumps = malloc(sizeof(ull) * n);
    ull l, u = 0;
    for (int i = 0; i < n; i++) {
        ull energy;
        if (scanf("%llu", &energy) != 1) return 1;
        jumps[i] = energy;
        u += energy;
    }
    l = u / (k + 1);
    u++;

    while (l < u) {
        ull init = (u + l) >> 1;
        int index = 0;
        for (int i = 0; i < init && i <= k; i++) {
            ull temp = init - i;
            while (index < n && temp >= jumps[index]) temp -= jumps[index++];
        }
        if (index == n) {
            u = init;
        } else {
            l = init + 1;
        }
    }

    printf("%llu\n", l);
    free(jumps);
    return 0;
}
