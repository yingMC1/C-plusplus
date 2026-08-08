#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int INF = 0x3f;
const int N = 2005;

int n, q, dp[N][N], sz[N];
vector<pair<int, int>> g[N];

void dfs(int x, int fa) {
    dp[x][0] = 0;
    for (auto [i, w] : g[x]) {
        if (i == fa) continue;
        dfs(i, x);
        for (int j = sz[x]; j >= 0; j--)
            for (int k = 0; k <= sz[i]; k++)
                if (dp[x][j] != -INF && dp[j][k] != -INF)
                    dp[x][j + k + 1] =
                        max(dp[x][j + k + 1], dp[x][j] + dp[j][k] + w);
        sz[x] += sz[i] + 1;
    }
}

int main() {
    freopen("input", "r", stdin);
    cin >> n >> q;
    for (int i = 1; i < n; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    memset(dp, -INF, sizeof(dp));
    dfs(1, 0);
    cout << dp[1][q] << endl;
    return 0;
}