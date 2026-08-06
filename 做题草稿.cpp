#include <bits/stdc++.h>

using namespace std;
const int N = 1e5 + 5;

int n, c[N], dp[N], ans = 1;
vector<int> e[N];

void dfs(int x, int fa) {
    dp[x] = 1;
    int mx1 = 0, mx2 = 0;
    for (int v : e[x]) {
        if (v == fa) continue;
        dfs(v, x);
        if (c[x] == c[v]) continue;
        dp[x] = max(dp[x], dp[v] + 1);
        if (dp[v] + 1 > mx1) mx2 = mx1, mx1 = dp[v] + 1;
        else if (dp[v] + 1 > mx2) mx2 = dp[v] + 1;
    }
    ans = max(ans, mx1 + mx2 + 1);
}

int main() {
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> c[i];
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        e[u].push_back(v);
        e[v].push_back(u);
    }
    dfs(1, 0);
    cout << ans;
}