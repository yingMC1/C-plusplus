#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 105;

int n, m, ha, la, hb, lb, a[N][N];
int vis[N][N];
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, -1, 1};

bool bfs() {
    queue<pair<int, int>> q;
    q.push({ha, la});
    vis[ha][la];
    while (!q.empty()) {
        auto [x, y] = q.front();
        vis[x][y] = true;
        for (int i = 1; i <= 4; i++) {
            x += dx[i];
            y += dy[i];
            if (nx >= 0 && nx < n && ny >= 0 && ny < n && vis[nx][ny] == 0) {
                mp[nx][ny] = 1;
                q.push({nx, ny});
            }
        }
    }
}

int main() {
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) cin >> a[i][j];
    cin >> ha >> la >> hb >> lb;
    bool ans = bfs();
    if (ans) puts("YES");
    else puts("NO");
    return 0;
}