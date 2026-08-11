#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 100005;

vector<int> g[N];
int dep[N];
int f[N][20];
int x[N], y[N];

void dfs(int x, int fa) {
    dep[x] = dep[fa] + 1;
    f[x][0] = fa;
    for (int i = 1; i <= 19; i++)
        f[x][i] = f[f[x][i - 1]][i - 1];
    for (auto i : g[x])
        if (i != fa)
            dfs(i, x);
}

int lca(int x, int y) {
    if (dep[x] < dep[y])
        swap(x, y);
    for (int i = 19; i >= 0; i--)
        if (dep[f[x][i]] >= dep[y])
            x = f[x][i];
    if (x == y)
        return x;
    for (int i = 19; i >= 0; i--)
        if (f[x][i] != f[y][i])
            x = f[x][i], y = f[y][i];
    return f[x][0];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<pair<int, int>> tree;
    int tot = 0;
    for (int i = 1; i <= n - 1; i++) {
        int u, v;
        cin >> u >> v;
        tree.push_back({u, v});
        y[++tot] = u;
        y[++tot] = v;
    }
    sort(y + 1, y + tot + 1);
    int cnt = unique(y + 1, y + tot + 1) - (y + 1);
    for (auto e : tree) {
        int u = lower_bound(y + 1, y + cnt + 1, e.first) - y;
        int v = lower_bound(y + 1, y + cnt + 1, e.second) - y;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 1);
    while (q--) {
        int a, b;
        cin >> a >> b;
        int u = lower_bound(y + 1, y + cnt + 1, a) - y;
        int v = lower_bound(y + 1, y + cnt + 1, b) - y;
        int L = lca(u, v);
        cout << dep[u] + dep[v] - 2 * dep[L] << endl;
    }

    return 0;
}