// 包含标准输入输出库，用于 scanf/printf
#include <stdio.h>
// 包含标准库，用于 malloc/free 等内存管理函数
#include <stdlib.h>

// 定义 AVL 树节点结构，用于动态维护 y 轴区间的平衡二叉树
// 技术亮点：AVL 树保证插入/删除/查询操作复杂度为 O(log n)，结合扫描线算法，在大规模矩形（n ≤ 5e4）时性能优异
typedef struct AVLNode {
    int y1, y2;       // 当前节点代表的矩形在 y 轴的上下界 [y1, y2]
    int id;           // 对应矩形的索引 id，用于并查集合并操作
    int height;       // 节点高度，用于计算平衡因子
    struct AVLNode *left, *right; // 左右子节点指针
} AVLNode;

// 创建一个新的 AVL 节点，并初始化各字段
// 这里将 height 设为 1（叶节点高度），left/right 设 NULL
static AVLNode* new_node(int y1, int y2, int id) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->y1 = y1;
    node->y2 = y2;
    node->id = id;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

// 获取节点高度，空节点高度为 0，简化高度计算逻辑
static int height(AVLNode* node) {
    return node ? node->height : 0;
}

// 计算节点的平衡因子 = 左子树高度 - 右子树高度
// 用于判断是否需要旋转
static int get_balance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// 右旋操作：针对左子节点 "重" 的情况进行调整，释放左子树压力
// 技术亮点：右旋和左旋保持 AVL 平衡性，常数时间内完成结构修正
static AVLNode* rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // 旋转过程：x 成为新的子树根，原根 y 成为 x 的右子节点
    x->right = y;
    y->left = T2;

    // 更新节点高度：高度 = max(左右子高度) + 1
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));

    return x;
}

// 左旋操作：针对右子节点 "重" 的情况进行调整
static AVLNode* rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // 旋转过程：y 成为新的子树根，原根 x 成为 y 的左子节点
    y->left = x;
    x->right = T2;

    // 更新高度
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));

    return y;
}

// 向 AVL 树插入 [y1, y2] 区间对应的节点，并在 y 边界相等时触发并查集 unite 合并操作
// unite 函数指针用于将两个矩形连通并合并分量
static AVLNode* avl_insert(AVLNode* root, int y1, int y2, int id, void (*unite)(int a, int b)) {
    if (!root)
        return new_node(y1, y2, id);

    // 保证按 y1 排序插入
    if (y1 < root->y1) {
        // 如果新矩形下边界 y2 恰好与当前节点上边界 y1 相等，则相切，执行合并
        if (y2 == root->y1) unite(id, root->id);
        root->left = avl_insert(root->left, y1, y2, id, unite);
    } else {
        // 如果新矩形上边界 y1 恰好与当前节点下边界 y2 相等，则相切，执行合并
        if (y1 == root->y2) unite(id, root->id);
        root->right = avl_insert(root->right, y1, y2, id, unite);
    }

    // 更新根节点高度
    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));
    int balance = get_balance(root);

    // 四种不平衡情况：LL, RR, LR, RL，分别对应单/双旋修复
    if (balance > 1 && y1 < root->left->y1)
        return rotate_right(root);            // LL
    if (balance < -1 && y1 > root->right->y1)
        return rotate_left(root);             // RR
    if (balance > 1 && y1 > root->left->y1) { // LR
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    if (balance < -1 && y1 < root->right->y1) { // RL
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

// 从 AVL 树中删除 y1 对应的节点，用于扫描线退出事件
static AVLNode* avl_delete(AVLNode* root, int y1) {
    if (!root) return NULL;

    // 根据 y1 定位待删节点
    if (y1 < root->y1)
        root->left = avl_delete(root->left, y1);
    else if (y1 > root->y1)
        root->right = avl_delete(root->right, y1);
    else {
        // 找到节点：若只有一个子节点或没有，直接释放并返回子节点
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        }

        // 两个子节点：找到中序后继（右子树最左节点），交换数据后删除后继
        AVLNode* temp = root->right;
        while (temp->left) temp = temp->left;
        root->y1 = temp->y1;
        root->y2 = temp->y2;
        root->id = temp->id;
        root->right = avl_delete(root->right, temp->y1);
    }

    // 更新高度及平衡性，必要时旋转
    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));
    int balance = get_balance(root);

    if (balance > 1 && get_balance(root->left) >= 0)
        return rotate_right(root);
    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    if (balance < -1 && get_balance(root->right) <= 0)
        return rotate_left(root);
    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

// 最大矩形数量上限和扫描线事件类型定义
#define MAXN 50000
#define ENTER 0 // 矩形进入扫描线
#define EXIT  1 // 矩形离开扫描线

// 矩形结构体，记录 x/y 范围及面积
// 预先存储所有矩形信息，减少运行时频繁计算
typedef struct {
    int x1, x2;       // x 轴区间 [x1, x2]
    int y1, y2;       // y 轴区间 [y1, y2]
    long long area;   // 矩形面积 w * h
} Rect;

// 扫描线事件结构体，记录事件发生的 x 坐标、关联矩形 id 及事件类型
typedef struct {
    int id, x, y, type;
} Event;

// 并查集数组，parent[i] 存储矩形 i 的父节点 id，用于快速查询分量
static int parent[MAXN];
// 组面积数组，记录以当前根为代表的连通分量总面积
static long long group_area[MAXN];

// 事件排序比较函数：首先按 x 升序，其次按类型（ENTER 优先于 EXIT），最后按 y
// 保证同 x 坐标先处理进入再处理退出，实现正确的相切判断
static int cmp_event(const void* a, const void* b) {
    const Event* ea = (const Event*)a;
    const Event* eb = (const Event*)b;
    if (ea->x != eb->x) return ea->x - eb->x;
    if (ea->type != eb->type) return ea->type - eb->type;
    return ea->y - eb->y;
}

// 并查集查找，带路径压缩，优化至近似 O(1)
static int find(int x) {
    return parent[x] == x ? x : (parent[x] = find(parent[x]));
}

// 并查集合并函数，将 b 并入 a，累加面积
static void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a != b) {
        parent[b] = a;
        group_area[a] += group_area[b]; // 优化技巧：直接在根节点累加面积
    }
}

int main() {
    int n;
    // 读取矩形数量 n，如果读取失败则提前退出
    if (scanf("%d", &n) != 1) return 1;

    // 预分配存储矩形信息和事件列表的数组，避免动态扩容开销
    Rect rects[MAXN];
    Event events[MAXN * 2];

    // 读取每个矩形数据，初始化并查集和事件
    for (int i = 0; i < n; i++) {
        int x, y, w, h;
        if (scanf("%d %d %d %d", &x, &y, &w, &h) != 4) return 1;
        rects[i] = (Rect){x, x + w, y, y + h, (long long)w * h};
        parent[i] = i;                // 并查集初始化，每个节点自成一组
        group_area[i] = rects[i].area; // 初始组面积即自身面积

        // 生成 ENTER/EXIT 两种类型的扫描线事件
        events[i * 2] = (Event){i, rects[i].x1, rects[i].y1, ENTER};
        events[i * 2 + 1] = (Event){i, rects[i].x2, rects[i].y1, EXIT};
    }
    // 对所有事件排序，n = 5e4 时，排序复杂度 O(n log n) 可接受
    qsort(events, n * 2, sizeof(Event), cmp_event);

    AVLNode* root = NULL; // AVL 树根节点，用于维护当前活动矩形的 y 区间
    int enter_ids[MAXN], exit_ids[MAXN]; // 缓存当前 x 坐标下批量 ENTER/EXIT 的 id

    int i = 0;
    // 扫描线主循环：按 x 坐标从小到大处理
    while (i < n * 2) {
        int x = events[i].x;
        int enter_n = 0, exit_n = 0;

        // 收集当前 x 坐标所有 ENTER 事件
        while (i < n * 2 && events[i].x == x && events[i].type == ENTER)
            enter_ids[enter_n++] = events[i++].id;
        // 收集当前 x 坐标所有 EXIT 事件
        while (i < n * 2 && events[i].x == x && events[i].type == EXIT)
            exit_ids[exit_n++] = events[i++].id;

        // 先删除离开扫描线的矩形区间，保证同 x 下先退出再进入不会误判相切
        for (int j = 0; j < exit_n; j++) {
            int id = exit_ids[j];
            root = avl_delete(root, rects[id].y1);
        }

        // 双指针技巧：同时遍历 enter_ids 和 exit_ids，检测 y 区间相交或相切的情况
        // 时间复杂度 O(enter_n + exit_n)，避免两层嵌套遍历
        for (int ei = 0, xi = 0; ei < enter_n && xi < exit_n;) {
            int eid = enter_ids[ei];
            int xid = exit_ids[xi];
            if (rects[eid].y2 < rects[xid].y2) {
                if (rects[eid].y2 >= rects[xid].y1) // 判断下边界与另一区间上边界是否重叠
                    unite(eid, xid);
                ei++;
            } else {
                if (rects[eid].y1 <= rects[xid].y2) // 判断上边界与另一区间下边界是否重叠
                    unite(eid, xid);
                xi++;
            }
        }

        // 再插入新进入扫描线的矩形区间
        for (int j = 0; j < enter_n; j++) {
            int id = enter_ids[j];
            root = avl_insert(root, rects[id].y1, rects[id].y2, id, unite);
        }
    }

    // 扫描线结束后，所有相切矩形已在并查集中合并，查找面积最大的连通分量
    int max_root = 0;
    for (int i = 1; i < n; i++) {
        int r = find(i);
        if (group_area[r] > group_area[max_root]) {
            max_root = r;
        }
    }

    // 输出最大组面积和对应矩形索引列表（已按自然顺序遍历输出，即升序）
    printf("%lld\n", group_area[max_root]);
    for (int i = 0; i < n; i++) {
        if (find(i) == max_root) {
            printf("%d ", i);
        }
    }
    puts("");

    return 0;
}
