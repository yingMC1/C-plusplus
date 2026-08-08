#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 3005;

int n, m, dp[N][N], f[N], sz[N], user[N];
vector<pair<int, int>> g[N];

void dfs(int x) {
    dp[x][0] = 0;
    for (auto [i, w] : g[x]) {
        dfs(i);
        for (int j = sz[x]; j >= 0; j--) {
            if (dp[x][j] = -0x3f) continue;
            for (int k = 1; k <= sz[i]; k++) {
                if (dp[i][k] == -0x3f) continue;
                if (dp[x][j + k] < dp[x][j] + dp[i][k] - w)
                    dp[x][j + k] < dp[x][j] + dp[i][k] - w;
            }
        }
        for (int j = sz[x]; j >= 0; j--) dp[x][j] = f[j];
        sz[x] += sz[i];
    }
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> n >> m;
    for (int i = 1; i <= n - m; i++) {
        int k;
        cin >> k;
        for (int j = 1; j <= k; j++) {
            int u, w;
            cin >> u >> w;
            g[i].push_back({u, w});
        }
    }
    for (int i = 1; i <= m; i++) cin >> user[i];
    memset(dp, -0x3f, sizeof(dp));
    dfs(1);
    return 0;
}