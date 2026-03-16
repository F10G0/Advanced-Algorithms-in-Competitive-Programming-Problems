#include <stdio.h>
#include <stdlib.h>

#define MAX_PATH 64
#define INF_STEP 0x7fffffff
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Represents a rational number and its path in the Stern-Brocot tree.
// The path is stored in compressed form: alternating L/R steps as segment counts.
typedef struct {
    int num, den;
    int path[MAX_PATH];
    int path_len;
} FractionPath;

// Encodes a fraction num/den into its path in the Stern-Brocot tree.
// The path is stored as alternating L and R step counts.
void encode_fraction_path(FractionPath *f) {
    int m = f->num;
    int n = f->den;
    int dir = 0;
    f->path_len = 0;

    while (m != n) {
        int s;
        if (!dir) {
            s = (n - 1) / m;
            n -= s * m;
        } else {
            s = (m - 1) / n;
            m -= s * n;
        }
        f->path[f->path_len++] = s;
        dir ^= 1;
    }
}

// Simulates traversal of the Stern-Brocot tree along the common prefix
// of two encoded paths and returns the denominator of the resulting fraction.
int get_denominator_from_prefix(const FractionPath *a, const FractionPath *b) {
    int l = 1, r = 0;
    int dir = 0;
    int len = MIN(a->path_len, b->path_len);

    for (int i = 0; i < len; i++) {
        int sa = a->path[i];
        int sb = b->path[i];
        int s = MIN(sa, sb);

        if (!dir) {
            r += l * s;
        } else {
            l += r * s;
        }

        if (sa != sb) break;
        dir ^= 1;
    }

    return l + r;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 1;

    char buf[16];
    while (n--) {
        if (scanf("%s", buf) != 1) return 1;

        // Handle exact 0.0 or 1.0 cases directly.
        double val = atof(buf);
        if (val == 0.0 || val == 1.0) {
            puts("1");
            continue;
        }

        // Parse decimal digits after "0." into integer v and base = 10^p
        int v = 0, base = 1;
        for (const char *p = buf + 2; *p; p++) {
            v = v * 10 + (*p - '0');
            base *= 10;
        }

        // Construct the rounding interval [l, r)
        int den = base * 2;
        FractionPath l = {v * 2 - 1, den};
        FractionPath r = {v * 2 + 1, den};

        encode_fraction_path(&l);
        encode_fraction_path(&r);

        // To avoid hitting the open upper bound,
        // extend r's path by L:1 + R:INF
        if (r.path_len & 1) {
            r.path[r.path_len - 1]++;
        } else {
            r.path[r.path_len++] = 1;
        }
        r.path[r.path_len++] = INF_STEP;

        // Output the smallest valid denominator
        int result = get_denominator_from_prefix(&l, &r);
        printf("%d\n", result);
    }

    return 0;
}
