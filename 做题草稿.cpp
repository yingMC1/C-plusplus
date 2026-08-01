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

int n, m, m1, m2, a[N][N];
bool vis[N][N];
queue<pair<int, int>> q;

int main() {
    read<int, int, int, int>(n, m, m1, m2);
    int dx[4] = {m1, m1, -m1, -m1};
    int dy[4] = {m2, -m2, m2, -m2};
    fo(int, i, 1, <=, n, 1) {
        fo(int, j, 1, <=, m, 1) {
            read<int>(a[i][j]);
        }
    }
    vis[1][1] = true;
    q.push(make_pair(1, 1));
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
    }
    return 0;
}