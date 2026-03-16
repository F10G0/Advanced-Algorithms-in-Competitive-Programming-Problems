// 最小质因子表（每个数的最小质因子）
int smallest_prime_factor[MAXN + 1];

// 构造 SPF 表（从大到小枚举因数，最后留下的就是最小质因子）
void build_spf() {
    smallest_prime_factor[0] = smallest_prime_factor[1] = 1;
    for (int i = n; i > 1; i--) {
        for (int j = i; j <= n; j += i) {
            smallest_prime_factor[j] = i;
        }
    }
}

// 存储质因子及其对应幂次
int prime_factors[20];       // 质因子列表
int factor_exponents[20];    // 每个质因子的幂次
int num_pfs;                 // 不同质因子数量

// 对 num 进行质因数分解，保存结果到 prime_factors 和 factor_exponents
void get_prime_factors(int num) {
    for (num_pfs = 0; num > 1; num_pfs++) {
        int cur_pf = smallest_prime_factor[num];  // 当前最小质因子
        prime_factors[num_pfs] = cur_pf;
        factor_exponents[num_pfs] = 0;

        // 统计该质因子的幂次
        while (num % cur_pf == 0) {
            num /= cur_pf;
            factor_exponents[num_pfs]++;
        }
    }
}

// 用于存放所有枚举出来的约数
int divisors[MAX_DIV_CNT];   // 约数数组（大小要足够）
int num_divisors;            // 实际约数数量

// 递归枚举所有约数
// idx：当前处理到第 idx 个质因子
// cur：当前累乘的约数
void enumerate_divisors(int idx, int cur) {
    if (idx == num_pfs) {
        // 所有质因子都处理完了，cur 是一个完整约数
        divisors[num_divisors++] = cur;
        return;
    }

    // 枚举当前质因子从 0 次幂到 e 次幂
    for (int i = 0; i <= factor_exponents[idx]; i++) {
        enumerate_divisors(idx + 1, cur);         // 递归处理下一个质因子
        cur *= prime_factors[idx];                // 增加当前质因子的乘积
    }
}

// 用于 qsort 的整数比较函数（升序）
int cmp_int(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

// 对 num 求所有正约数，排序后存入 divisors[]
void get_divisors(int num) {
    get_prime_factors(num);                       // 分解质因子
    num_divisors = 0;
    enumerate_divisors(0, 1);                     // 从第 0 个质因子、初始值 1 开始
    qsort(divisors, num_divisors, sizeof(int), cmp_int);  // 升序排序
}
