#include <stdio.h>
#include <stdlib.h>

#define MAXA 64

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1;

    unsigned char *a = aligned_alloc(sizeof(long long), n);
    for (int i = n - 1; i >= 0; i--) {
        int ai;
        if (scanf("%d", &ai) != 1) return 1;
        a[i] = (unsigned char)ai;
    }
    
    int bucket[0x100];
    unsigned char *b = aligned_alloc(sizeof(long long), n);
    unsigned char tmp0, tmp1, tmp2, tmp3;
    int exceed = 0, last_valid = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= MAXA; j++) {
            bucket[j] = 0;
        }

        bucket[0] = i - last_valid - 1;
        if (!exceed) bucket[a[last_valid] + b[last_valid]]++;
        int j = 0;
        for (; j < (last_valid - 7) >> 3; j++) {
            long long tmp = ((long long *)a)[j] + ((long long *)b)[j];
            
            bucket[(tmp >> 56) & 0xff]++;
            bucket[(tmp >> 48) & 0xff]++;
            bucket[(tmp >> 40) & 0xff]++;
            bucket[(tmp >> 32) & 0xff]++;
            bucket[(tmp >> 24) & 0xff]++;
            bucket[(tmp >> 16) & 0xff]++;
            bucket[(tmp >> 8) & 0xff]++;
            bucket[tmp & 0xff]++;
        }
        for (j <<= 3; j < last_valid; j++) {
            int tmp = a[j] + b[j];
            bucket[tmp]++;
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
            for (; j < last_valid - 3; j += 4) {
                tmp0 = a[j] + b[j];
                tmp1 = a[j + 1] + b[j + 1];
                tmp2 = a[j + 2] + b[j + 2];
                tmp3 = a[j + 3] + b[j + 3];
                
                b[j] = (tmp0 < kth || (tmp0 == kth && equal_cnt-- > 0)) ? tmp0 : 0;
                b[j + 1] = (tmp1 < kth || (tmp1 == kth && equal_cnt-- > 0)) ? tmp1 : 0;
                b[j + 2] = (tmp2 < kth || (tmp2 == kth && equal_cnt-- > 0)) ? tmp2 : 0;
                b[j + 3] = (tmp3 < kth || (tmp3 == kth && equal_cnt-- > 0)) ? tmp3 : 0;
            }
            for (; j < last_valid; j++) {
                unsigned char tmp = a[j] + b[j];
                b[j] = (tmp < kth || (tmp == kth && equal_cnt-- > 0)) ? tmp : 0;
            }
            unsigned char tmp = a[j] + b[j];
            b[j++] = (!exceed && (tmp < kth || (tmp == kth && equal_cnt-- > 0))) ? tmp : 0;
            while (j++ < i) {
                b[j] = 0;
            }

            if (rest + a[i] <= MAXA) {
                b[i] = rest;
                exceed = 0;
            } else exceed = rest;
            last_valid = i;
        }
    }
    
    for (int i = n - 1; i > last_valid; i--) {
        printf("-1 ");
    }
    printf("%d ", exceed ? exceed : b[last_valid]);
    for (int i = last_valid - 1; i >= 0; i--) {
        printf("%d ", b[i]);
    }

    return 0;
}
