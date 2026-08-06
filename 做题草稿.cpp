#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
using itn = int;
const int N = 200010;
const int mod = 10007;

i64 n, a[N], dp[N], ans;
vector<int> g[N];

void dfs(int x, int fa) {
    for (int i : g[x]) {
        if (i == fa) continue;
        dfs(i, x);
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
    cout << ans << endl;
    return 0;
}