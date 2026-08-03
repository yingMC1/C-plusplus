#include <bits/stdc++.h>

using namespace std;
const int N = 2e5 + 5;

int t, n, m, dep[N], f[N][20], mi[N][20], fa[N];
bool vis[N];
vector<pair<int, int>> g[N];

struct I {
    int u, v, w;
} e[N];

int cmp(I x, I y) {
    return x.w < y.w;
}

int find(int x) {
    if (x == fa[x]) return x;
    return fa[x] = find(fa[x]);
}

void dfs(int x, int fa, int w) {
    dep[x] = dep[fa] + 1;
    f[x][0] = fa;
    mi[x][0] = w;
    for (int i = 1; i <= 19; i++) {
        f[x][i] = f[f[x][i - 1]][i - 1];
        mi[x][i] = min(mi[f[x][i - 1]][i - 1], mi[x][i - 1]);
    }
    for (auto [a, b] : g[x])
        if (a != fa) dfs(a, x, b);
}

int lca(int x, int y) {
    if (dep[x] < dep[y]) swap(x, y);
    for (int i = 19; i >= 0; i--)
        if (dep[f[x][i]] >= dep[y]) x = f[x][i];
    if (x == y) return x;
    for (int i = 19; i >= 0; i--)
        if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
    return f[x][0];
}

int qmin(int x, int y) {
    int t = 2e9;
    for (int i = 19; i >= 0; i--)
        if (dep[f[x][i]] >= dep[y]) {
            t = min(mi[x][i], t);
            x = f[x][i];
        }
    return t;
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> t;
    for (int i = 1; i <= t; i++) {
        cin >> n >> m;
        for (int i = 1; i <= n; i++) {
            fa[i] = i;
            g[i].clear();
            dep[i] = 0;
            for (int j = 0; j <= 19; j++) {
                f[i][j] = 0;
                mi[i][j] = 2e9;
            }
        }
        for (int i = 1; i <= m; i++) e[i].u = e[i].v = e[i].w = 0;
        for (int i = 1; i <= m; i++) cin >> e[i].u >> e[i].v >> e[i].w;
        sort(e + 1, e + n + 1, cmp);
        for (int i = 1; i <= m; i++) {
            int fx = find(e[i].u), fy = find(e[i].v);
            if (fx == fy) continue;
            fa[fx] = fy;
            g[e[i].u].push_back(make_pair(e[i].v, e[i].w));
            g[e[i].v].push_back(make_pair(e[i].u, e[i].w));
        }
        for (int i = 1; i <= n; i++) {
            if (dep[i] == 0) {
                dep[i] = 1;
                dfs(i, 0, 2e9);
            }
        }
        for (int i = 1; i <= m; i++) {
            int u = e[i].u, v = e[i].v, w = e[i].w;
            int l = lca(u, v);
        }
    }
    return 0;
}