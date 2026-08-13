#include <bits/stdc++.h>

#define lowbit(x) x & (-x)
#define endl '\n'
using namespace std;
using i64 = long long;
const int N = 300005;

int n, m, a[N];

struct bit {
    int tree[N];
    void update(int x, int w) {
        while (x <= n) {
            tree[x] ^= w;
            x += lowbit(x);
        }
    }
    int query(int x) {
        int sum = 0;
        while (x) {
            sum ^= tree[x];
            x -= lowbit(x);
        }
        return sum;
    }
} t;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        t.update(i, a[i] ^ a[i - 1]);
    }

    while (m--) {
        int op, x, y;
        cin >> op >> x >> y;
        if (op == 1) {
            t.update(x, y);
            a[x] ^= y;
        } else {
            cout << (t.query(y) ^ t.query(x - 1)) << endl;
        }
    }
    return 0;
}
