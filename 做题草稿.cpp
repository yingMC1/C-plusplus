#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
using itn = int;
const int N = 200010;
const int mod = 10007;

i64 n, a[N], dp[N], sum;
vector<pair<int, int>> g[N];

void dfs(int x, int fa) {
    dp[x] = 1;
    for (auto [a, b] : g[x]) {
        if (a == fa) continue;
        dfs(a, x);
        dp[x] += dp[a];
        sum += b * dp[a] * (n - dp[a]);
    }
}

int main() {
    cin >> n;
    for (int i = 1; i < n; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    dfs(1, 0);
    cout << sum << endl;
    return 0;
}