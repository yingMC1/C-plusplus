#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
using itn = int;
const int N = 2e5;

int n = 1, root, dp[N][3];
string s;
vector<int> g[N];

void dfs(int x, int fa) {
    for (int i : g[x]) {
        if (i == fa) continue;

        dfs(i, x);
    }
}

int main() {
    cin >> s;
    size_t l = s.size();
    for (int i = 1; i <= l; i++) {
        n += s[i];
        root++;
        // q[root].push_back()
    }
    return 0;
}