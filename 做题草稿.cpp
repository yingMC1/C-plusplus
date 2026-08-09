#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 2005;

i64 n, k, sz[N], dp[N][N], f[N];
// dp[x][i]表示x这棵子树有i个黑点距离总和的最大值
vector<pair<int, int>> g[N];

void dfs(int x, int fa) {
    memset(dp[x], -0x3f, sizeof(dp[x]));
    dp[x][0] = 0;
    dp[x][1] = 0;
    sz[x] = 1;
    for (auto [i, w] : g[x]) {
        if (i == fa)
            continue;
        dfs(i, x);
        memset(f, -0x3f, sizeof(f));
        for (int j = 0; j <= sz[x]; j++) {
            if (dp[x][j] < 0)
                continue;
            for (int t = 0; t <= sz[i]; t++) {
                if (dp[i][t] < 0)
                    continue;
                f[j] = max(f[j], dp[x][j] + dp[i][t] + w * (t * (k - t) + (sz[i] - t) * (n - sz[i] - k + t)));
            }
        }
        sz[x] += sz[i];
        for (int j = 0; j <= sz[x]; j++)
            dp[x][j] = f[j];
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    cin >> n >> k;
    for (int j = 1; j < n; j++) {
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