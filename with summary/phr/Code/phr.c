#include <stdio.h>
#include <stdlib.h>

#define MAX_PATH 64
#define INF_STEP 0x7fffffff
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    int num, den;
    int path[MAX_PATH];
    int path_len;
} FractionPath;

void encode_fraction_path(FractionPath *f) {
    int m = f->num, n = f->den;
    f->path_len = 0;

    int dir = 0;
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

int get_denominator_from_prefix(const FractionPath *a, const FractionPath *b) {
    int l = 1, r = 0;
    int len = MIN(a->path_len, b->path_len);

    int dir = 0;
    for (int i = 0; i < len; i++) {
        int sa = a->path[i];
        int sb = b->path[i];
        int s = MIN(sa, sb);

        if (!dir) {
            r += l * s;
        } else {
            l += r * s;
        }
        dir ^= 1;

        if (sa != sb) break;
    }

    return l + r;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 1;

    char buf[16];
    while (n--) {
        if (scanf("%s", buf) != 1) return 1;

        double val = atof(buf);
        if (val == 0.0 || val == 1.0) {
            puts("1");
            continue;
        }

        int v = 0, base = 1;
        for (const char *p = buf + 2; *p; p++) {
            v = v * 10 + (*p - '0');
            base *= 10;
        }

        int den = base * 2;
        FractionPath l = {v * 2 - 1, den};
        FractionPath r = {v * 2 + 1, den};

        encode_fraction_path(&l);
        encode_fraction_path(&r);

        if (r.path_len & 1) {
            r.path[r.path_len - 1]++;
        } else {
            r.path[r.path_len++] = 1;
        }
        r.path[r.path_len++] = INF_STEP;

        int result = get_denominator_from_prefix(&l, &r);
        printf("%d\n", result);
    }

    return 0;
}
