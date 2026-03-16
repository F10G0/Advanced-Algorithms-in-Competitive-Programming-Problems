#include <stdio.h>
#include <stdlib.h>

#define MAXA 64

typedef struct {
    int a, b;
} Cat;

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1;

    Cat *c = malloc(sizeof(Cat) * n);
    for (int i = n - 1; i >= 0; i--) {
        if (scanf("%d", &c[i].a) != 1) return 1;
    }
    
    int bucket[MAXA + 1];
    int tmp0, tmp1, tmp2, tmp3;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= MAXA; j++) {
            bucket[j] = 0;
        }

        int j = 0;
        int i4 = i - 3;
        for (; j < i4; j += 4) {
            tmp0 = c[j].b + c[j].a;
            tmp1 = c[j + 1].b + c[j + 1].a;
            tmp2 = c[j + 2].b + c[j + 2].a;
            tmp3 = c[j + 3].b + c[j + 3].a;
            
            if (tmp0 <= MAXA) bucket[tmp0]++;
            if (tmp1 <= MAXA) bucket[tmp1]++;
            if (tmp2 <= MAXA) bucket[tmp2]++;
            if (tmp3 <= MAXA) bucket[tmp3]++;
        }
        for (; j < i; j++) {
            int tmp = c[j].b + c[j].a;
            if (tmp <= MAXA) bucket[tmp]++;
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
            int j = 0;
            int i4 = i - 3;
            for (; j < i4; j += 4) {
                tmp0 = c[j].b + c[j].a;
                tmp1 = c[j + 1].b + c[j + 1].a;
                tmp2 = c[j + 2].b + c[j + 2].a;
                tmp3 = c[j + 3].b + c[j + 3].a;
                
                c[j].b = (tmp0 < kth || (tmp0 == kth && equal_cnt-- > 0)) ? tmp0 : 0;
                c[j + 1].b = (tmp1 < kth || (tmp1 == kth && equal_cnt-- > 0)) ? tmp1 : 0;
                c[j + 2].b = (tmp2 < kth || (tmp2 == kth && equal_cnt-- > 0)) ? tmp2 : 0;
                c[j + 3].b = (tmp3 < kth || (tmp3 == kth && equal_cnt-- > 0)) ? tmp3 : 0;
            }
            for (; j < i; j++) {
                int tmp = c[j].b + c[j].a;
                c[j].b = (tmp < kth || (tmp == kth && equal_cnt-- > 0)) ? tmp : 0;
            }
            c[i].b = rest;
        } else c[i].b = -c[i].a;
    }
    
    for (int i = n - 1; i >= 0; i--) {
        printf("%d ", c[i].b < 0 ? -1 : c[i].b);
    }

    return 0;
}
