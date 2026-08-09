#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const int mod = 1000000007;
const int N = 2005;

int n, k, sz[N];
i64 dp[N][N][3], f[N][3];
vector<int> g[N];

i64 mul(i64 a, i64 b) { return (__int128)a * b % mod; }

void dfs(int x, int fa) {
    memset(dp[x], 0, sizeof(dp[x]));
    dp[x][0][0] = 1;
    dp[x][1][2] = 1;
    sz[x] = 1;
    for (int i : g[x]) {
        if (i == fa)
            continue;
        dfs(i, x);
        memset(f, 0, sizeof(f));
        for (int a = 0; a <= sz[x]; a++) {
            for (int j = 0; j < 3; j++) {
                if (!dp[x][a][j])
                    continue;
                for (int b = 0; b <= sz[i]; b++) {
                    int sum = a + b;
                    for (int c = 0; c < 3; c++) {
                        if (!dp[i][b][c])
                            continue;
                        i64 val = mul(dp[x][a][j], dp[i][b][c]);
                        if (j == 0) {
                            if (c == 0)
                                f[sum][1] = (f[sum][1] + val) % mod;
                            else
                                f[sum][0] = (f[sum][0] + val) % mod;
                        } else if (j == 1)
                            f[sum][2] = (f[sum][2] + val) % mod;
                        else if (j == 2)
                            f[sum][0] = (f[sum][0] + val) % mod;
                    }
                }
            }
        }
        sz[x] += sz[i];
        for (int s = 0; s <= sz[x]; s++) {
            dp[x][s][0] = f[s][0];
            dp[x][s][1] = f[s][1];
            dp[x][s][2] = f[s][2];
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    cin >> n >> k;
    for (int i = 1; i <= n - 1; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0);
    for (int i = 0; i <= n; i++) {
        i64 ans = (dp[1][i][0] + dp[1][i][1] + dp[1][i][2]) % mod;
        cout << ans << endl;
    }
    return 0;
}