#include <bits/stdc++.h>

#define fo(i, start, op, end, step) for (int i = start; i op end; i += step)
using namespace std;
using i64 = long long;
const int N = 500005;

template <class T> T read() {
    T r = 0, f = 1;
    char c = getchar();
    while ((c < '0' || c > '9') && c != '-') c = getchar();
    if (c == '-') f = -1, c = getchar();
    while (c >= '0' && c <= '9') r = r * 10 + c - '0', c = getchar();
    return f * r;
}

template <class T, class... Args> void read(T &first, Args &...rest) {
    first = read<T>();
    if constexpr (sizeof...(rest) > 0) { read<T>(rest...); }
}

int n;
int vis[N];
vector<int> a[N];
queue<int> q, ans;

void bfs() {
    q.push(1);
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        for (int u : a[x]) {
            if (vis[x]) continue;
            vis[x] = true;
            q.push(u);
            ans.push(u);
        }
    }
}

int main() {
    read<int>(n);
    fo(i, 1, <, n, 1) {
        int u, v;
        read<int, int>(u, v);
        a[u].push_back(v);
        a[v].push_back(u);
    }
    bfs();
    fo(i, 1, <=, n, 1) {
        cout << ans.front() << ' ';
        q.pop();
    }
    return 0;
}