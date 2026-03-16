#include <stdio.h>
int main(){
    int n = 500000;
    int q = n + (n - 1) + (n - 1) + (n - 19);
    FILE *fp = fopen("input.in", "w");
    fprintf(fp, "%d %d\n", n, q);
    for (int i = 0; i < n; i++)
        fputc("abcdefghijklmnopqrstuvwxyz"[i % 26], fp);
    fputc('\n', fp);
    for (int i = 1; i <= n; i++)
        fprintf(fp, "1 %d\n", i);
    for (int i = 2; i <= n; i++)
        fprintf(fp, "%d %d\n", i, n);
    for (int i = 1; i + 1 <= n; i++)
        fprintf(fp, "%d %d\n", i, i + 1);
    for (int i = 1; i + 19 <= n; i++)
        fprintf(fp, "%d %d\n", i, i + 19);
    fclose(fp);
    return 0;
}
