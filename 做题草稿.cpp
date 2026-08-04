#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 3e5 + 5;

int n, m, f[N][20], dep[N], d[N];
vector<int> g[N];

void dfs(int x, int fa) {
    dep[x] = dep[fa] + 1;
    f[x][0] = fa;
    for (int i = 1; i <= 19; i++) f[x][i] = f[f[x][i - 1]][i - 1];
    for (int i : g[x])
        if (i != fa) dfs(i, x);
}

int lca(int x, int y) {
    if (dep[x] < dep[y]) swap(x, y);
    for (int i = 19; i >= 0; i--)
        if (dep[f[x][i]] >= dep[y]) x = f[x][i];
    if (x == y) return 0;
    for (int i = 19; i >= 0; i--)
        if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
    return f[x][0];
}

void sum(int x, int fa) {
    for (int i : g[x]) {
        dfs(i, x);
        d[x] += d[i];
    }
}

int main() {
    cin >> n >> m;
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> v >> u;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0);
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
    }
    return 0;
}