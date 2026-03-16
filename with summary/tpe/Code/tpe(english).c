#include <stdio.h>
#include <stdlib.h>

#define MAXN 500000           // Maximum length of the string
#define HASH_BASE 29          // Hash base for rolling hash

// — Rolling hash arrays —
// prefix_hash[i]: hash of s[1..i]
// hash_exp[i]   : HASH_BASE^i
static unsigned int prefix_hash[MAXN + 1], hash_exp[MAXN + 1];

// Build prefix hash and power array
static void init_hashtable(const char *s, int n) {
    prefix_hash[0] = 0;
    hash_exp[0] = 1;
    for (int i = 0; i < n; i++) {
        prefix_hash[i + 1] = prefix_hash[i] * HASH_BASE + (s[i] - 'a' + 1);
        hash_exp[i + 1] = hash_exp[i] * HASH_BASE;
    }
}

// Get hash of substring s[l..r] (1-based)
static inline unsigned int hash(int l, int r) {
    return prefix_hash[r] - prefix_hash[l - 1] * hash_exp[r - l + 1];
}

// — Divisor precomputation —
// div_start[i]: offset in 'divisors' where divisors of i start
// div_ptr[i]  : pointer used during filling
static int div_start[MAXN + 2], div_ptr[MAXN + 1];
static int *divisors;  // Flat array holding all divisors of lengths 1..n

// Precompute divisors for all lengths 1..n
static void init_divs(int n) {
    // Count number of divisors for each length
    div_ptr[1] = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j += i) {
            div_ptr[j]++;
        }
    }

    // Compute starting index in divisors[] for each length
    div_start[1] = 0;
    for (int i = 1; i <= n; i++) {
        div_start[i + 1] = div_start[i] + div_ptr[i];
        div_ptr[i] = div_start[i];  // reset pointer
    }

    // Fill all divisors into the array
    divisors = (malloc)(sizeof(int) * div_start[n + 1]);
    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j += i) {
            divisors[div_ptr[j]++] = i;
        }
    }
}

int main() {
    int n, q;
    char s[MAXN + 1];

    // Input string and number of queries
    if (scanf("%d %d\n%s", &n, &q, s) != 3) return 1;

    init_hashtable(s, n);  // Precompute hashes
    init_divs(n);          // Precompute all divisors

    while (q--) {
        int l, r;
        if (scanf("%d %d", &l, &r) != 2) return 1;
        int len = r - l + 1;

        // Access divisors of the substring length
        int *div_ptr = &divisors[div_start[len]];

        // Find the smallest period by checking hashes
        while (1) {
            int p = *div_ptr++;
            if (p == len || hash(l, r - p) == hash(l + p, r)) {
                printf("%d\n", p);
                break;
            }
        }
    }

    // Cleanup
    free(divisors);
    return 0;
}
