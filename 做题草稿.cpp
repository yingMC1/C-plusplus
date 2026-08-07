#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 1e5 + 5;

i64 n, dp[N], dep[N], sum[N];
vector<int> g[N];

void dfs(int x, int fa) {
    dp[x] = 1;
    for (int i : g[x]) {
        if (i == fa) continue;
        dep[i] = dep[x] + 1;
        dfs(i, x);
        dp[x] += dp[i];
    }
}

void dfs_sum(int x, int fa) {
    for (int i : g[x]) {
        if (i == fa) continue;
        sum[i] = sum[x] + n - 2 * dp[i];
        dfs_sum(i, x);
    }
}

int main() {
    cin >> n;
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0);
    for (int i = 1; i <= n; i++) sum[1] += dep[i];
    dfs_sum(1, 0);
    for (int i = 1; i <= n; i++) cout << sum[i] << endl;
    return 0;
}