#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 3e5 + 5;

int n, m, f[N][20], dep[N], d[N];
vector<int> g[N];

void dfs(int x, int fa) {
    dep[x] = dep[fa] + 1;
    f[x][0] = fa;
    for (int i = 1; i <= 19; i++) f[x][i] = f[f[x][i - 1]][i - 1];
    for (int i : g[x])
        if (i != fa) dfs(i, x);
}

int lca(int x, int y) {}

int main() {

    return 0;
}