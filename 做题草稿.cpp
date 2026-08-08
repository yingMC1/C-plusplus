#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 2005;

int n, k, dp[N][N], sz[N];
vector<pair<int, int>> g[N];

void dfs(int x, int fa) {
    sz[x] = 1;
    dp[x][0] = 0;
    for (auto [i, w] : g[x]) {
        if (i == fa) continue;
        dfs(i, x);
        for (int j = sz[x]; j >= 0; j--) {
            for (int k = 0; k <= sz[i]; k++) {}
        }
        sz[x] += sz[i];
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    memset(dp, -0x3f, sizeof(dp));
    dfs(1, 0);
    cout << dp[1][k] << endl;
    return 0;
}