int m, n = p, q;
while (m != n) {
    if (m < n) {
        int step = (n - 1) / m;
        n -= step * m;
        print('L', step);
    } else {
        int step = (m - 1) / n;
        m -= step * n;
        print('R', step);
    }
}