#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 2005;

// 不放、被看守、自己放
int n, k, dp[N][N][3], f[N][N], sz[N];
vector<int> g[N];

void dfs(int x, int fa) {
    sz[x] = 1;
    for (int i : g[x]) {
        if (i == fa) continue;
        dfs(i, x);
        memset(f, -0x3f, sizeof(f));
        for (int j = sz[x]; j >= 0; j--) {
            for (int k = 1; k <= sz[i]; k++) {
                f[j][0] = max(f[j][0], dp[i][k][0] + dp[i][k][1]);
                f[j][1] = max(f[j][1], dp[i][k][2]);
                f[j][2] = max({f[j][2], dp[i][k][0], dp[i][k][1], dp[i][k][2]});
                f[fa][j] += 1;
            }
        }
        sz[x] += sz[i];
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        int u, v, w;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    memset(dp, -0x3f, sizeof(dp));
    dfs(1, 0);
    return 0;
}