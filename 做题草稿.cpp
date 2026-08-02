#include <bits/stdc++.h>

using namespace std;
const int N = 5e5 + 5;

int n, m, dep[N], f[N][20];
vector<int> a[N];

void dfs(int x, int fa) {
    dep[x] = dep[fa] + 1;
    f[x][0] = fa;
    for (int i = 1; i <= 19; i++) f[x][i] = f[f[x][i - 1]][i - 1];
    for (int i : a[x])
        if (i != fa) dfs(i, x);
}

int lca(int x, int y) {
    if (dep[x] < dep[y]) swap(x, y);
    for (int i = 19; i >= 0; i--)
        if (dep[f[x][i]] > dep[y]) x = f[x][i];
    if (x == y) return x;
    for (int i = 19; i >= 0; i--)
        if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
    return f[x][0];
}

int sum(int x, int y) {
    return dep[x] + dep[y] - 2 * dep[lca(x, y)];
}

int abc(int a, int b, int c, int x) {
    return sum(a, x) + sum(b, x) + sum(c, x);
}

int main() {
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        int u, v;
        cin >> u >> v;
        a[u].push_back(v);
        a[v].push_back(u);
    }
    for (int i = 1; i <= m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        if (a > b) swap(a, b);
        if (b > c) swap(b, c);
        cout << lca(lca(a, b), c) << ' ' << abc(a, b, c, lca(lca(a, b), c))
             << endl;
    }
    return 0;
}