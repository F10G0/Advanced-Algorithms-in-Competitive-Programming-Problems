#include <stdio.h>
#include <stdlib.h>

#define MAX_PATH 64
#define INF_STEP 0x7fffffff
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// 本程序用于求解：
// 对于一个给定的小数 x ∈ [0, 1]（最多 9 位小数精度），
// 找出所有四舍五入结果为 x 的最简分数中，分母最小的那一个。

// 核心思想基于 Stern-Brocot Tree（斯特恩-布罗科特树）：
// - 所有有理数构成一棵路径树，每个分数对应一条从 1/1 出发的路径（L/R 构成）
// - 公共前缀路径越短，分母越小

// 💡 精妙思想 1：
// 使用左右分数构造区间 [l, r) = [(2v - 1)/(2·10^p), (2v + 1)/(2·10^p))
// 来表示四舍五入结果为 x 的所有分数

// 💡 精妙思想 2：
// 使用路径“段压缩”表示 L/R 序列，每段表示在某方向连续走的步数

typedef struct {
    int num, den;             // 分子分母（不一定最简）
    int path[MAX_PATH];       // 交错方向路径（每段是连续 L/R 步数）
    int path_len;             // 实际路径段数量
} FractionPath;

// 将一个分数 num/den 编码为 Stern-Brocot 树上的路径
// 使用段压缩表示法记录路径，每段是当前方向上的连续步数

// 💡 精妙思想 2 的实现：
// 交错使用 L 和 R，不存具体方向，只记录每段走多少步
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

// 根据路径前缀构造分数，返回其分母

// 💡 精妙思想 3：
// 两条路径的最长公共前缀，对应树中最浅的节点，
// 即为 [l, r) 区间中“分母最小的合法分数”
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

        // 特判边界情况：0.0 和 1.0 的四舍五入结果仍为自己
        double val = atof(buf);
        if (val == 0.0 || val == 1.0) {
            puts("1");
            continue;
        }

        // 将小数转换为整数表示：v = 整数部分，base = 10^p
        int v = 0, base = 1;
        for (const char *p = buf + 2; *p; p++) {
            v = v * 10 + (*p - '0');
            base *= 10;
        }

        // 💡 精妙思想 1 的体现：
        // 构造表示精度控制的左右边界分数 [l, r)
        int den = base * 2;
        FractionPath l = {v * 2 - 1, den};
        FractionPath r = {v * 2 + 1, den};

        encode_fraction_path(&l);
        encode_fraction_path(&r);

        // 💡 精妙思想 4：
        // 为右路径追加一段 L:1 + R:极大值，
        // 构造出 <r 的最大路径，确保不落入开区间外
        if (r.path_len & 1) {
            r.path[r.path_len - 1]++;
        } else {
            r.path[r.path_len++] = 1;
        }
        r.path[r.path_len++] = INF_STEP;

        // 求公共前缀路径所代表的分数分母
        int result = get_denominator_from_prefix(&l, &r);
        printf("%d\n", result);
    }

    return 0;
}
