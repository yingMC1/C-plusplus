#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int mod = 1e9;
;
const int N = 2005;

int n, dp[N][N][3];
vector<int> g[N];

void dfs(int x, int fa) {

    for (int i : g[x]) {
        if (i == fa) continue;
        dfs(i, x);
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    cin >> n;
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0);
    return 0;
}