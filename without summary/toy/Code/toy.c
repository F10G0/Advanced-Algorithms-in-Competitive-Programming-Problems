#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, b, s, d;
    if (scanf("%d %d %d %d", &n, &b, &s, &d) != 4) return 1;

    int min_price = b + 1;
    while (s--) {
        int price;
        if (scanf("%d", &price) != 1) return 1;

        for (int i = 0; i < d; i++) {
            int stock;
            if (scanf("%d", &stock) != 1) return 1;
            if (price < min_price && stock >= n) {
                min_price = price;
            }
        }
    }

    int cost = min_price * n;
    if (cost <= b) {
        printf("%d\n", cost);
    } else {
        printf("GIFT MY SOUL\n");
    }

    return 0;
}
