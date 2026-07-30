#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 1005;

int r, c, ans;
bool vis[N][N], g;
char a[N][N];
int dx[5] = {0, 1, -1, 0, 0};
int dy[5] = {0, 0, 0, 1, -1};
queue<pair<int, int>> q;

bool bfs() {
    q.push({1, 1});
    vis[1][1] = true;
    while (!q.empty()) {
        auto [x, y] = q.front();
        if (x == r && y == c) return true;
        q.pop();
        for (int i = 1; i <= 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx > 0 && nx <= r && ny > 0 && ny <= c && vis[nx][ny] == 0) {
                vis[nx][ny] = 1;
                q.push({nx, ny});
            }
        }
    }
    return true;
}

int main() {
    cin >> r >> c;
    for (int i = 1; i <= r; i++)
        for (int j = 1; j <= c; j++) cin >> a[i][j];
    bfs();
    cout << "There are " << ans << " ships." << endl;
    return 0;
}