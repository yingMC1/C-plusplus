#include <bits/stdc++.h>

#define lowbit(x) x & -(x)
#define endl '\n'
using namespace std;
using i64 = long long;
const int N = 5e5 + 5;

int n, tree[N], f[N][N], cnt[N], a[N], us[N];
string s1, s2;

void update(int x, int w) {
    while (x <= n) {
        tree[x] += w;
        x += lowbit(x);
    }
}

i64 query(int x) {
    i64 sum = 0;
    while (x) {
        sum += tree[x];
        x -= lowbit(x);
    }
    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> s1 >> s2;
    for (int i = 0; i < n; i++) {
        int c = s2[i] - 'A';
        f[c][cnt[c]] = i;
        cnt[c]++;
    }
    for (int i = 0; i < n; i++) {
        int c = s1[i] - 'A';
        a[i] = f[c][us[c]];
        us[c]++;
    }
    i64 ans = 0;
    for (int i = 0; i < n; i++) {
        ans += i - query(a[i] + 1);
        update(a[i] + 1, 1);
    }
    cout << ans << endl;
    return 0;
}
