#include <bits/stdc++.h>

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

    while (!q.empty()) {
        int x = q.front();
        q.pop();
        for (int u : a[x]) {}
    }
}

int main() {
    read<int>(n);
    for (int i = 1; i <= n; i++) {
        int u, v;
        read<int, int>(u, v);
        a[u].push_back(v);
        a[v].push_back(u);
    }
    q.push(1);
    bfs();
    return 0;
}