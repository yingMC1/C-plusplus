#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 5e5 + 5;

i64 n, m, k[N], sz[N], dp[N], dp2[N], dep[N], len1[N], len2[N];
vector<pair<int, int>> g[N];

void dfs(int x, int fa){
    sz[x] = k[x];
    for(auto [i, w] : g[x]){
        if(i == fa)continue;
        dfs(i, x);
        if(sz[i]){
            sz[x] += sz[i];
            dp[x] += dp[i] + 2 * w;
            int len = len1[i] + w;
            if(len > len1[x])len2[x] = len1[x], len1[x] = len;
            else if(len > len2[x])len2[x] = len;
        }
    }
}

void dfs_sum(int x, int fa){
    for(auto [i, w] : g[x]){
        if(i == fa)continue;
        if(sz[i] == 0)dp[i] += dp[x] + 2 * w, len1[i] = len1[x] + w;
        else if(sz[i] < m){
            dp[i] = dp[x];
            int len;
            if(len1[x] = len1[i] + w)len = len2[x] + w;
            else len = len1[x] + w;
            if(len > len1[i])len2[i] = len1[i], len1[i] = len;
            else if(len > len2[i])len2[i] = len;
        }
        dfs_sum(i, x);
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> n >> m;
    for(int i = 1; i < n; i++){
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    for(int i = 1; i <= m; i++){
        int x;
        cin >> x;
        k[i] = 1;
    }
    dfs(1, 0);
    dfs_sum(1, 0);
    for(int i = 1; i <= n; i++){
        cout << dp[i] - len1[i] << '\n';
    }
    return 0;
}