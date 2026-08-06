#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
using itn = int;
const int N = 1e5 + 5;

int n, a[N];
vector<int> g[N];

void dfs(int x, int fa) {}

int main() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) cin >> a[i];
    dfs(1, 0);
    return 0;
}