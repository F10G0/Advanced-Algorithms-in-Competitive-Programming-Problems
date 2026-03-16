#include <stdio.h>
#include <stdlib.h>

#define MAXN 500000
#define HASH_BASE 29
#define MAX_DIV_CNT 200

unsigned int prefix_hash[MAXN + 1], hash_exp[MAXN + 1];

void build_hash(const char *s, int n) {
    prefix_hash[0] = 0;
    hash_exp[0] = 1;
    for (int i = 0; i < n; i++) {
        prefix_hash[i + 1] = prefix_hash[i] * HASH_BASE + (s[i] - 'a' + 1);
        hash_exp[i + 1] = hash_exp[i] * HASH_BASE;
    }
}

static inline unsigned int hash(int l, int r) {
    return prefix_hash[r] - prefix_hash[l - 1] * hash_exp[r - l + 1];
}


int smallest_prime_factor[MAXN + 1];
int divs[MAX_DIV_CNT];

int cmp_int(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

void build_spf(int n) {
    smallest_prime_factor[0] = smallest_prime_factor[1] = 1;
    for (int i = n; i > 1; i--) {
        for (int j = i; j <= n; j += i) {
            smallest_prime_factor[j] = i;
        }
    }
}

void get_divisors(int num) {
    divs[0] = 1;
    int div_cnt = 1;
    for (int offset; num > 1; div_cnt += offset) {
        int prime = smallest_prime_factor[num];
        for (offset = 0; num % prime == 0; offset += div_cnt) {
            num /= prime;
            for (int i = offset; i < offset + div_cnt; i++) {
                divs[i + div_cnt] = divs[i] * prime;
            }
        }
    }
    qsort(divs, div_cnt, sizeof(int), cmp_int);
}


int main() {
    int n, q;
    char s[MAXN + 1];
    if (scanf("%d %d\n%s", &n, &q, s) != 3) return 1;

    build_hash(s, n);
    build_spf(n);

    while (q--) {
        int l, r;
        if (scanf("%d %d", &l, &r) != 2) return 1;
        int len = r - l + 1;

        get_divisors(len);
        int i = 0;
        while (1) {
            int p = divs[i++];
            if (p == len || hash(l, r - p) == hash(l + p, r)) {
                printf("%d\n", p);
                break;
            }
        }
    }

    return 0;
}
