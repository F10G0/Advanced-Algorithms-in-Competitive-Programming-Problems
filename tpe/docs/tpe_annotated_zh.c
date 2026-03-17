#include <stdio.h>
#include <stdlib.h>

#define MAXN 500000           // 字符串最大长度
#define HASH_BASE 29          // 哈希基数，选择一个合适的素数即可

// —— 哈希相关全局数组 ——
// prefix_hash[i]: 字符串前缀 s[1..i] 的哈希值（1-based）
// hash_exp[i]   : HASH_BASE^i，用于还原子串哈希
static unsigned int prefix_hash[MAXN + 1], hash_exp[MAXN + 1];

// 构建前缀哈希和幂表
// 输入字符串 s，长度为 n（0-based 字符串）
static void init_hashtable(const char *s, int n) {
    prefix_hash[0] = 0;     // 空串哈希为 0
    hash_exp[0] = 1;        // BASE^0 = 1
    for (int i = 0; i < n; i++) {
        // 将字符 s[i] 映射到整数 [1, 26]
        prefix_hash[i + 1] = prefix_hash[i] * HASH_BASE + (s[i] - 'a' + 1);
        // 计算 BASE 的幂次
        hash_exp[i + 1] = hash_exp[i] * HASH_BASE;
    }
}

// 计算 s[l..r] 的哈希值，使用 1-based 下标
// 哈希区间公式：H[r] - H[l-1] * BASE^(r-l+1)
static inline unsigned int hash(int l, int r) {
    return prefix_hash[r] - prefix_hash[l - 1] * hash_exp[r - l + 1];
}


// —— 约数预处理相关 ——
// div_start[i] 表示长度 i 的约数列表在 divisors 数组中的起始下标
// div_ptr[i]   在填充过程中表示当前写入下标，初始化后即用于写入
static int div_start[MAXN + 2], div_ptr[MAXN + 1];
static int *divisors;  // 所有长度的约数线性写入到该大数组中

// 构建 1 到 n 所有正整数的约数集合
// 总复杂度为 O(n log n)，线性空间写入所有约数
static void init_divs(int n) {
    // 1. 统计每个数字 i 的约数个数，存在 div_ptr[i] 中
    div_ptr[1] = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j += i) {
            div_ptr[j]++;  // i 是 j 的约数
        }
    }

    // 2. 计算每个长度 i 在 divisors 中的起始偏移量
    div_start[1] = 0;
    for (int i = 1; i <= n; i++) {
        div_start[i + 1] = div_start[i] + div_ptr[i]; // 前缀和
        div_ptr[i] = div_start[i]; // 重置 div_ptr 为写指针
    }

    // 3. 分配足够大小的约数存储数组，并填入每个 i 的所有约数
    divisors = (malloc)(sizeof(int) * div_start[n + 1]);
    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j += i) {
            // 将 i 作为 j 的一个约数写入对应位置
            divisors[div_ptr[j]++] = i;
        }
    }
}


int main() {
    int n, q;
    char s[MAXN + 1];  // 原始字符串（0-based）

    // 读入 n（字符串长度）、q（查询数量）、字符串 s
    if (scanf("%d %d\n%s", &n, &q, s) != 3) return 1;

    init_hashtable(s, n);  // 构造哈希表
    init_divs(n);          // 构造约数字典

    // 处理每一个查询
    while (q--) {
        int l, r;
        if (scanf("%d %d", &l, &r) != 2) return 1;
        int len = r - l + 1;  // 当前查询的子串长度

        // 获取该长度的所有约数区间指针
        int *div_ptr = &divisors[div_start[len]];

        // 遍历所有约数，尝试找到最短周期
        while (1) {
            int p = *div_ptr++;
            // 若当前约数 p 满足周期条件或已到自身长度
            // 比较 s[l..r-p] 和 s[l+p..r] 是否相等
            if (p == len || hash(l, r - p) == hash(l + p, r)) {
                printf("%d\n", p);  // 输出最短周期
                break;
            }
        }
    }

    // 释放动态分配内存
    free(divisors);
    return 0;
}
