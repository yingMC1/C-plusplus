#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const int N = 1e4 + 5;

int n, m;
int dep[N], f[N][20], dis[N];
vector<pair<int, int>> g[N];

void dfs(int x, int fa) {
    dep[x] = dep[fa] + 1;
    f[x][0] = fa;
    for (int i = 1; i < 20; i++) { f[x][i] = f[f[x][i - 1]][i - 1]; }
    for (auto [a, b] : g[x]) {
        if (a == fa) continue;
        dis[a] = dis[x] + b;
        dfs(a, x);
    }
}

int lca(int x, int y) {
    if (dep[x] < dep[y]) swap(x, y);
    for (int i = 0; i < 20; i++) {
        if (dep[x] - dep[y] & (1 << i)) x = f[x][i];
    }
    if (x == y) return x;
    for (int i = 20 - 1; i >= 0; i--) {
        if (f[x][i] != f[y][i]) {
            x = f[x][i];
            y = f[y][i];
        }
    }
    return f[x][0];
}

int main() {
    cin >> n >> m;
    for (int i = 1; i < n; i++) {
        int x, y, k;
        cin >> x >> y >> k;
        g[x].push_back({y, k});
        g[y].push_back({x, k});
    }
    dfs(1, 0);
    while (m--) {
        int a, b;
        cin >> a >> b;
        int c = lca(a, b);
        cout << dis[a] + dis[b] - 2 * dis[c] << endl;
    }
    return 0;
}