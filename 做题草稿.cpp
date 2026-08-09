#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const int mod = 1000000007;
const int N = 2005;

i64 n, sz[N], dp[N][N][3], f[N][3];
vector<int> g[N];

void dfs(int x, int fa) {
    memset(dp[x], 0, sizeof(dp[x]));
    dp[x][0][0] = 1;
    dp[x][1][2] = 1;
    sz[x] = 1;
    for (int i : g[x]) {
        if (i == fa)
            continue;
        dfs(i, x);
        sz[x] += sz[i];
        memset(f, 0, sizeof(f));
        for (int a = 0; a <= sz[x]; a++) {
            for (int j = 0; j < 3; j++) {
                for (int b = 0; b <= sz[i]; b++) {
                    for (int c = 0; c < 3; c++) {
                        int cnt1 = a + b, cnt2 = j;
                        if (j == 2) {
                            if (c == 0)
                                cnt1 += 1;
                        } else if (j == 1) {
                            if (c == 2)
                                cnt2 += 1;
                        } else {
                            if (c == 2) {
                                cnt1 += 1;
                                cnt2 += 1;
                            }
                        }
                        f[cnt1][cnt2] = (f[cnt1][cnt2] + dp[x][a][j] * dp[i][b][c]) % mod;
                    }
                }
            }
        }
        for (int s = 0; s <= sz[x]; s++) {
            dp[x][s][0] = f[s][0];
            dp[x][s][1] = f[s][1];
            dp[x][s][2] = f[s][2];
        }
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
    for (int i = 0; i <= n; i++) {
        i64 ans = (dp[1][i][0] + dp[1][i][1] + dp[1][i][2]) % mod;
        cout << ans << '\n';
    }
    return 0;
}