#include <bits/stdc++.h>
using namespace std;

const int N = 200005;

struct Edge {
    int to, id, w;
};

int n, m;
vector<Edge> g[N];

struct Node {
    int u, v, w;
} a[N];

bool vis[N];
int dep[N], fa[N][20], f[N][20], toFa[N];

void dfs(int x, int p, int w) {
    dep[x] = dep[p] + 1;
    fa[x][0] = p;
    f[x][0] = toFa[x];

    for (int j = 1; j <= 19; j++) {
        int mid = fa[x][j - 1];
        fa[x][j] = fa[mid][j - 1];
        f[x][j] = min(f[x][j - 1], f[mid][j - 1]);
    }

    for (auto ed : g[x]) {
        int v = ed.to;
        if (v == p) continue;
        if (dep[v] == 0) {
            vis[ed.id] = true;
            toFa[v] = ed.w;
            dfs(v, x, ed.id);
        }
    }
}

int lca(int x, int y) {
    if (dep[x] < dep[y]) swap(x, y);
    for (int i = 19; i >= 0; i--) {
        if (dep[fa[x][i]] >= dep[y]) x = fa[x][i];
    }
    if (x == y) return x;
    for (int i = 19; i >= 0; i--) {
        if (fa[x][i] != fa[y][i]) {
            x = fa[x][i];
            y = fa[y][i];
        }
    }
    return fa[x][0];
}

int sum(int u, int v) {
    int ans = 1e9;

    if (dep[u] < dep[v]) swap(u, v);

    int d = dep[u] - dep[v];
    for (int j = 0; j <= 19; j++) {
        if (d & (1 << j)) {
            ans = min(ans, f[u][j]);
            u = fa[u][j];
        }
    }

    if (u == v) return ans;

    for (int j = 19; j >= 0; j--) {
        if (fa[u][j] != fa[v][j]) {
            ans = min(ans, f[u][j]);
            ans = min(ans, f[v][j]);
            u = fa[u][j];
            v = fa[v][j];
        }
    }

    ans = min(ans, f[u][0]);
    ans = min(ans, f[v][0]);
    return ans;
}

int main() {
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        a[i] = {u, v, w};
        g[u].push_back({v, i, w});
        g[v].push_back({u, i, w});
    }

    for (int j = 0; j <= 19; j++) f[0][j] = 1e9;
    toFa[1] = 1e9;

    for (int i = 1; i <= n; i++) {
        if (dep[i] == 0) dfs(i, 0, 0);
    }

    int ans = 1e9;
    for (int i = 1; i <= m; i++) {
        if (!vis[i]) {
            int u = a[i].u, v = a[i].v, w = a[i].w;
            ans = min(ans, min(sum(u, v), w));
        }
    }

    cout << ans << '\n';
    return 0;
}