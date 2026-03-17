int n, m, k, a, b, ans;
struct edge {
    int v, ne;
} e[M];
int h[N], idx;
int vis[N], match[N];

void add(int a, int b) {
    e[++idx] = { b, h[a] };
    h[a] = idx;
}

bool dfs(int u) {
    for (int i = h[u]; i; i = e[i].ne) {
        int v = e[i].v;
        if (vis[v])
            continue;
        vis[v] = 1;
        if (!match[v] || dfs(match[v])) {
            match[v] = u;
            return l;
        }
    }
    return 0;
}

int main() {
    cin >> n >> m >> k;
    for (int i = 0; i < k; i++) {
        cin >> a >> b, add(a, b);
    }
    for (int i = 1; i <= n; i++) {
        memset(vis, 0, sizeof vis);
        if (dfs(i))
            ans++;
    }
    cout << ans;
    return 0;
}
