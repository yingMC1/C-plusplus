#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 3e5 + 5;

int n, m, f[N][20], dep[N], dis[N], d[N], len[N];
vector<pair<int, int>> g[N];

void dfs(int x, int fa) {
    dep[x] = dep[fa] + 1;
    f[x][0] = fa;
    for (int i = 1; i <= 19; i++) f[x][i] = f[f[x][i - 1]][i - 1];
    for (auto [a, b] : g[x]) {
        if (a == fa) continue;
        d[x] += d[fa] + b;
        dfs(a, x);
    }
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

void sum(int x, int fa) {
    for (auto [a, b] : g[x]) {
        if (a == fa) continue;
        sum(a, x);
        d[x] += d[a];
    }
}

int main() {
    cin >> n >> m;
    for (int i = 1; i < n; i++) {
        int u, v, w;
        cin >> v >> u >> w;
        g[u].push_back(make_pair(v, w));
        g[v].push_back(make_pair(u, w));
    }
    dfs(1, 0);
    int maxn = 0;
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        len[i] = dis[u] + dis[v] - 2 * dis[lca(u, v)];
    }

    return 0;
}