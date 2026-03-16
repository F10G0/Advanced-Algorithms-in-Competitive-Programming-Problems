#include <stdio.h>
#include <stdlib.h>

void kmp(const char *s, int len, int *prev) {
    prev[0] = 0;
    for (int i = 1; i < len; i++) {
        int j = prev[i - 1];
        while (j > 0 && s[i] != s[j])
            j = prev[j - 1];
        if (s[i] == s[j])
            j++;
        prev[i] = j;
    }
}

int main() {
    int n, q;
    scanf("%d %d", &n, &q);

    char *s = (char *)malloc((n + 1) * sizeof(char));
    scanf("%s", s);

    int *res = (int *)malloc(q * sizeof(int));
    int *prev = (int *)malloc((n + 1) * sizeof(int));

    for (int i = 0; i < q; i++) {
        int l, r;
        scanf("%d %d", &l, &r);

        int len = r - l + 1;
        kmp(s + l - 1, len, prev);

        int period = len - prev[len - 1];
        res[i] = len % period ? len : period;
    }

    for (int i = 0; i < q; i++) {
        printf("%d\n", res[i]);
    }

    free(s);
    free(res);
    free(prev);

    return 0;
}
