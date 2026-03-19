#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b)  ((a) > (b) ? (a) : (b))

long long gcd_func(long long a, long long b, long long *x, long long *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }

    long long xx, yy;
    long long gcd = gcd_func(b, a % b, &xx, &yy);
    *x = yy;
    *y = xx - (a / b) * yy;
    return gcd;
}

int main() {
    long long s1, c1, s2, c2, n;
    if (scanf("%lld %lld %lld %lld %lld", &s1, &c1, &s2, &c2, &n) != 5) return 1;

    if (!c1 && !c2) {
        if (s1 == s2 && s1 >= n) printf("%lld\n", s1);
        else printf("IMPAWSIBLE\n");
        return 0;
    }
    if (!c1) {
        if (s1 >= n && (s1 - s2) % c2 == 0) printf("%lld\n", s1);
        else printf("IMPAWSIBLE\n");
        return 0;
    }
    if (!c2) {
        if (s2 >= n && (s2 - s1) % c1 == 0) printf("%lld\n", s2);
        else printf("IMPAWSIBLE\n");
        return 0;
    }
    
    long long x, y;
    long long gcd = gcd_func(c1, c2, &x, &y);
    if ((s2 - s1) % gcd) {
        printf("IMPAWSIBLE\n");
        return 0;
    }

    x *= (s2 - s1) / gcd;
    long long feasible = s1 + c1 * x;

    long long lcm = c1 / gcd * c2;
    long long k = feasible % lcm;
    long long max = MAX(MAX(s1, s2), n);
    if (k < max) k += ((max - k - 1) / lcm + 1) * lcm;

    printf("%lld\n", k);
    return 0;
}
