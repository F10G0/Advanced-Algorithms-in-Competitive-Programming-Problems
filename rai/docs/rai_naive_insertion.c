// RAI - Naive insertion solution (O(n^2))

// Idea:
// - Treat each glade as a function f(t) = a*t + b
// - Maintain a sequence
// - Insert each function into the earliest position where it is better
// - Shift subsequent functions and update their intercepts

// This version is used for understanding and debugging.
// The optimized version uses a splay tree with lazy propagation.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//#define DEBUG_PRINT
#ifdef DEBUG_PRINT
#define DEBUG(x) do { x; } while (0)
#else
#define DEBUG(x)
#endif

typedef struct {
    uint64_t a, b;
} Func;

uint64_t value(Func f, uint64_t i) {
    return f.a * i + f.b;
}

int cmp(const void* a, const void* b) {
    return ((Func*)a)->a - ((Func*)b)->a;
}

int main() {
    DEBUG(puts("Input:\nn: "));
    int n;
    scanf("%d", &n);
    Func* funcs = malloc(n * sizeof(Func));
    for (int i = 0; i < n; i++) {
        DEBUG(printf("%d: ", i));
        scanf("%ld %ld", &funcs[i].a, &funcs[i].b);
    }

    qsort(funcs, n, sizeof(Func), cmp);

    for (int t = 1; t < n; t++) {
        for (int i = 0; i < t; i++) {
            Func ft = funcs[t];
            if (value(ft, i) > value(funcs[i], i)) {
                for (int j = t; j > i; j--) {
                    funcs[j] = funcs[j - 1];
                    funcs[j].b += ft.a - funcs[j].a;
                }
                funcs[i] = ft;
                break;
            }
        }
    }

    DEBUG(puts("Output:"));
    uint64_t sum = 0;
    for (int i = 0; i < n; i++) {
        DEBUG(printf("%d: ", i));
        sum += value(funcs[i], i);
        printf("%ld\n", sum);
    }

    free(funcs);
    return 0;
}
