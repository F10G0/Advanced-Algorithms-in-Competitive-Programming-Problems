#include <stdio.h>
#include <stdlib.h>

#define MAXA 64

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1;

    int *a = malloc(sizeof(int) * n);
    for (int i = n - 1; i >= 0; i--) {
        if (scanf("%d", a + i) != 1) return 1;
    }
    
    int bucket[MAXA + 1];
    int *b = malloc(sizeof(int) * n);
    int *prev = malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= MAXA; j++) {
            bucket[j] = 0;
        }
        for (int j = 0; j < i; j++) {
            prev[j] = b[j] + a[j];
            if (prev[j] > MAXA) continue;
            bucket[prev[j]]++;
        }

        int cnt = i >> 1;
        int rest = m;
        int equal_cnt = 0;
        int kth;
        for (kth = 0; kth <= MAXA; kth++) {
            if (cnt >= bucket[kth]) {
                cnt -= bucket[kth];
                rest -= bucket[kth] * kth;
            } else {
                equal_cnt = cnt;
                rest -= cnt * kth;
                break;
            }
        }

        if (rest >= 0) {
            for (int j = 0; j < i; j++) {
                if (prev[j] < kth || (prev[j] == kth && equal_cnt-- > 0)) {
                    b[j] = prev[j];
                } else {
                    b[j] = 0;
                }
            }
            b[i] = rest;
        } else b[i] = -a[i];
    }
    
    for (int i = n - 1; i >= 0; i--) {
        printf("%d ", b[i] < 0 ? -1 : b[i]);
    }

    return 0;
}
