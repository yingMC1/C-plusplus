#include <bits/stdc++.h>

#define fo(bo, i, start, op, end, step) for (bo i = start; i op end; i += step)
using namespace std;
using i64 = long long;
const int N = 35;

template <class T> T read() {
    T r = 0, f = 1;
    int c = getchar();
    while ((c < '0' || c > '9') && c != '-') c = getchar();
    if (c == '-') f = -1, c = getchar();
    while (c >= '0' && c <= '9') r = r * 10 + c - '0', c = getchar();
    return f * r;
}

template <class T, class... Args> void read(T &first, Args &...rest) {
    first = read<T>();
    if constexpr (sizeof...(rest) > 0) { read<T>(rest...); }
}

struct I {
    int x, y, cnt;
};

int n, m, m1, m2, a[N][N];
int bx, by, ex, ey;
bool vis[N][N];
queue<I> q;

int bfs() {
    int dx[8] = {m1, m1, -m1, -m1, m2, -m2, m2, -m2};
    int dy[8] = {m2, -m2, m2, -m2, m1, m1, -m1, -m1};
    vis[1][1] = true;
    q.push({bx, by, 0});
    while (!q.empty()) {
        auto [x, y, cnt] = q.front();
        q.pop();
        fo(int, i, 0, <, 8, 1) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx == ex && ny == ey) return ++cnt;
            if (nx < 0 || ny < 0 || nx > n || ny > n || vis[nx][ny] ||
                a[nx][ny] == 0 || a[nx][ny] == 2)
                continue;
            q.push({ny, nx, cnt + 1});
            vis[nx][ny] = true;
        }
    }
    return 0;
}

int main() {
    read<int, int, int, int>(n, m, m1, m2);
    fo(int, i, 1, <=, n, 1) {
        fo(int, j, 1, <=, m, 1) {
            read<int>(a[i][j]);
            if (a[i][j] == 3) {
                bx = i;
                by = j;
            } else if (a[i][j] == 4) {
                ex = i;
                ey = j;
            }
        }
    }
    cout << bfs() << endl;
    return 0;
}
/*

*/