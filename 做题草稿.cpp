#include <bits/stdc++.h>

#define int long long
// 老师oj运行内存评测有问题，所以不会MLE
#define endl '\n'
using namespace std;
using i64 = long long;
const int N = 2e5 + 5;

i64 n, m, tree[N << 2], tag[N << 2], a[N];

void down(int x, int l, int r, int w) {
    tree[x] ^= w * (r - l + 1);
    tag[x] ^= w;
}

void push_down(int x, int l, int r) {
    if (!tag[x]) return;
    int mid = l + r >> 1;
    down(x * 2, l, mid, tag[x]);
    down(x * 2 + 1, mid + 1, r, tag[x]);
    tag[x] = 0;
}

void up(int x) {
    tree[x] = tree[x * 2] ^ tree[x * 2 + 1];
}

void build(int x, int l, int r) {
    if (l == r) {
        tree[x] = a[l];
        return;
    }
    int mid = l + r >> 1;
    build(x * 2, l, mid);
    build(x * 2 + 1, mid + 1, r);
    up(x);
}

void update(int x, int l, int r, int ql, int qr, int w) {
    if (ql <= l && r <= qr) {
        down(x, l, r, w);
        return;
    }
    push_down(x, l, r);
    int mid = l + r >> 1;
    if (ql <= mid) update(x * 2, l, mid, ql, qr, w);
    if (qr > mid) update(x * 2 + 1, mid + 1, r, ql, qr, w);
    up(x);
}

int query(int x, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) return tree[x];
    push_down(x, l, r);
    int mid = l + r >> 1, res = 0;
    if (ql <= mid) res ^= query(x * 2, l, mid, ql, qr);
    if (qr > mid) res ^= query(x * 2 + 1, mid + 1, r, ql, qr);
    return res;
}

signed main() {
    ios::sync_with_stdio(false);
    cout.tie(nullptr);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) cin >> a[i];
    build(1, 1, n);
    for (int i = 1; i <= m; i++) {
        int op, l, r;
        cin >> op >> l >> r;
        if (op == 1) {
            int x;
            cin >> x;
            update(1, 1, n, l, r, x);
        } else cout << query(1, 1, n, l, r) << endl;
    }
    return 0;
}