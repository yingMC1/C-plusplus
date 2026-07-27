#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 105;

int n, m, ha, la, hb, lb, a[N][N];
int vis[N][N];
int dx[5] = {0, 1, -1, 0, 0};
int dy[5] = {0, 0, 0, -1, 1};
queue<pair<int, int>> q;

bool bfs() {
    if (a[ha][la] == 1 || a[hb][lb] == 1) return false;
    q.push({ha, la});
    vis[ha][la] = true;
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int i = 1; i <= 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx == hb && ny == lb) return true;
            if (nx > 0 && nx <= n && ny > 0 && ny <= n && vis[nx][ny] == 0) {
                vis[nx][ny] = 1;
                q.push({nx, ny});
            }
        }
    }
    return false;
}

int main() {
    cin >> n;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) cin >> a[i][j];
    cin >> ha >> la >> hb >> lb;
    bool ans = bfs();
    if (ans) puts("YES");
    else puts("NO");
    return 0;
}