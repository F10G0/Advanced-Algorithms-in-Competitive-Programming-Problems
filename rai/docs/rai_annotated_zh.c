#include <stdio.h>
#include <stdlib.h>

// 定义无符号 64 位整数别名，便于书写
typedef unsigned long long ull;

// 每个函数由斜率 a 和截距 b 表示：f(i) = a * i + b
typedef struct {
    ull a, b;
} Func;

// 按斜率升序排序，如果斜率相同，按截距降序排序
// 这是为了保证构造出的函数序列在每一天都是最优的（prefix-optimal）
int cmp_Func(const void* a, const void* b) {
    const Func* fa = (const Func*)a;
    const Func* fb = (const Func*)b;
    if (fa->a != fb->a)
        return (fa->a > fb->a) - (fa->a < fb->a);
    return (fb->b > fa->b) - (fb->b < fa->b);
}

// Splay Tree 节点结构体，每个节点表示一个函数 f(i) = a*i + b
// push_val：累计的谱师补偿值（b += a）
// delay_days：表示该节点向后推迟了多少天，需要乘以 a 扣掉收益
// size：用于维护子树大小（支持平衡操作和中序下标定位）
typedef struct Node {
    ull a, b;
    ull push_val, delay_days;
    int size;
    struct Node *left, *right, *parent;
} Node;

// 全局 Splay Tree 根节点，方便插入与维护
Node* root = NULL;

// 计算某函数在第 i 天的真实收益：a*i + b
// 由于所有谱师补偿和延迟都已经通过 pushdown 应用到了 b 上，这里无需额外处理
ull evaluate_at_day(Node* n, int i) {
    return n->a * i + n->b;
}

// 创建新节点，初始状态为没有谱师补偿、未延迟、无左右孩子
Node* new_node(Func f) {
    Node* node = malloc(sizeof(Node));
    node->a = f.a;
    node->b = f.b;
    node->push_val = 0;
    node->delay_days = 0;
    node->size = 1;
    node->left = node->right = node->parent = NULL;
    return node;
}

// 获取子树大小（为空返回 0）
int static inline get_size(Node* node) {
    return node ? node->size : 0;
}

// 更新当前节点的 size 字段（递归向上传播）
void update_size(Node* node) {
    if (!node) return;
    node->size = 1 + get_size(node->left) + get_size(node->right);
}

// 将谱师补偿延迟标记 push_val 和 delay_days 应用到当前节点
// 这一步将 push_val 应用到 b，并传播给左右子树（延迟更新）
void apply_lazy_updates(Node* node) {
    if (node && node->push_val) {
        node->b += node->push_val;                         // b 补偿
        node->b -= node->a * node->delay_days;            // 由于延迟推迟了 d 天，需减 a*d
        if (node->left) {
            node->left->push_val += node->push_val;
            node->left->delay_days += node->delay_days;
        }
        if (node->right) {
            node->right->push_val += node->push_val;
            node->right->delay_days += node->delay_days;
        }
        node->push_val = 0;
        node->delay_days = 0;
    }
}

// Splay Tree 的基本旋转操作
// 旋转前先调用 pushdown 确保所有懒惰值被应用
void rotate(Node* x) {
    Node* p = x->parent;
    Node* g = p ? p->parent : NULL;
    apply_lazy_updates(p);
    apply_lazy_updates(x);
    if (p->left == x) {
        p->left = x->right;
        if (x->right) x->right->parent = p;
        x->right = p;
    } else {
        p->right = x->left;
        if (x->left) x->left->parent = p;
        x->left = p;
    }
    x->parent = g;
    p->parent = x;
    if (g) {
        if (g->left == p) g->left = x;
        else g->right = x;
    }
    update_size(p);
    update_size(x);
}

// Splay 操作：将节点 x 转到树的根部，用于平衡树和提升操作效率
void splay(Node* x) {
    while (x->parent) {
        Node* p = x->parent;
        Node* g = p->parent;
        if (!g) {
            rotate(x);
        } else if ((g->left == p) == (p->left == x)) {
            rotate(p);
            rotate(x);
        } else {
            rotate(x);
            rotate(x);
        }
    }
    root = x;
}

// 插入一个函数（以节点形式）到 splay 树中
// 插入逻辑遵循 prefix-optimal 的策略：新函数如果在某天收益更好，就插在那天之前
// 插入后将其 splay 到根，并对右子树统一打谱师（+a 和延迟一天）
void insert_node(Func f) {
    Node* node = new_node(f);
    if (!root) {
        root = node;
        return;
    }

    Node* cur = root;
    ull day = get_size(cur->left);
    while (cur) {
        apply_lazy_updates(cur);
        if (evaluate_at_day(node, day) > evaluate_at_day(cur, day)) {
            if (!cur->left) {
                cur->left = node;
                node->parent = cur;
                break;
            } else {
                cur = cur->left;
                day -= get_size(cur->right) + 1;
            }
        } else {
            if (!cur->right) {
                cur->right = node;
                node->parent = cur;
                break;
            } else {
                cur = cur->right;
                day += get_size(cur->left) + 1;
            }
        }
    }

    splay(node);
    if (node->right) {
        node->right->push_val += f.a;      // 向右子树谱师补偿（b += a）
        node->right->delay_days++;         // 向右子树延迟一天（i += 1）
    }
}

// 非递归中序遍历：按天从左到右遍历树，输出每一天的收益并累加
// 由于树是 prefix-optimal 构造的，中序顺序即为正确顺序
void output_total_value(int n) {
    ull sum = 0;
    Node** stack = malloc(sizeof(Node*) * n);
    int sp = 0, day = 0;
    Node* cur = root;
    while (day < n) {
        while (cur) {
            stack[sp++] = cur;
            apply_lazy_updates(cur);
            cur = cur->left;
        }
        cur = stack[--sp];
        sum += evaluate_at_day(cur, day++);
        printf("%llu\n", sum);
        cur = cur->right;
    }
    free(stack);
}

int main() {
    int n;
    scanf("%d", &n);

    Func* funcs = malloc(sizeof(Func) * n);
    for (int i = 0; i < n; i++) {
        scanf("%llu %llu", &funcs[i].a, &funcs[i].b);
    }
    qsort(funcs, n, sizeof(Func), cmp_Func);  // 排序保证构造合法性

    for (int i = 0; i < n; i++) {
        insert_node(funcs[i]);  // 按顺序插入
    }
    output_total_value(n);      // 输出所有天的累加收益

    free(funcs);
    return 0;
}
