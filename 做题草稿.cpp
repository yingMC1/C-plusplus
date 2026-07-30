#include <bits/stdc++.h>
// #define fo (i, j, n, k) for(register int i = j; i >= n; i -= k)
#define fo(i, j, n, k) for (register int i = j; i >= n; i -= k)
using namespace std;
using i64 = long long;
const int N = 1005;
int r, c, ans;
bool vis[N][N], g;
char a[N][N];
int dx[] = {-2, -1, 1, 2, 2, 1, -1, -2};
int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};
queue<pair<int, int>> q;

int bfs() {
    q.push({1, 1});
    vis[1][1] = true;
    while (!q.empty()) {
        auto [x, y] = q.front();
        if (x == r && y == c) return true;
        q.pop();
        fo(i, 0, 8, 1) {
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
    fo(i, 1, r, 1) fo(j, 1, c, 1) cin >> a[i][j];
    bfs();
    cout << ans << endl;
    return 0;
}