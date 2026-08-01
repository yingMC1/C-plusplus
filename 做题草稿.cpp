#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const int N = 500005;
int n;
vector<int> a[N];
bool vis[N];

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

int main() {
    read<int>(n);
    for (int i = 1; i < n; i++) {
        int u, v;
        read<int, int>(u, v);
        a[u].push_back(v);
        a[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) {
        sort(a[i].begin(), a[i].end());
    }
    queue<int> q;
    q.push(1);
    vis[1] = true;
    bool first = true;
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        if (!first) putchar(' ');
        first = false;
        printf("%d", x);
        for (int u : a[x]) {
            if (vis[u]) continue;
            vis[u] = true;
            q.push(u);
        }
    }
    putchar('\n');
    return 0;
}
