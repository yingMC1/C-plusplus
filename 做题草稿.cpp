#include <bits/stdc++.h>

#define int long long
#define endl '\n'
using namespace std;
using i64 = long long;
const int N = 2e5 + 5;

i64 n, m, a[N], tree[N << 2], tag1[N << 2], tag2[N << 2], mx[N << 2];

void down_sum1(int x, int l, int r, int w) {
    tree[x] = w * (r - l + 1);
    tag1[x] = w;
    tag2[x] = 0;
    mx[x] = w;
}

void down_sum2(int x, int l, int r, int w) {
    tree[x] += w * (r - l + 1);
    tag2[x] += w;
    mx[x] += w;
}

void down(int x, int l, int r) {
    if (!tag1[x] && !tag2[x]) return;
    int mid = l + r >> 1;
    if (tag1[x]) {
        down_sum1(x * 2, l, mid, tag1[x]);
        down_sum1(x * 2 + 1, mid + 1, r, tag1[x]);
    }
    if (tag2[x]) {
        down_sum2(x * 2, l, mid, tag2[x]);
        down_sum2(x * 2 + 1, mid + 1, r, tag2[x]);
    }
    tag1[x] = tag2[x] = 0;
}

void up(int x) {
    tree[x] = tree[x * 2] + tree[x * 2 + 1];
    mx[x] = max(mx[x * 2], mx[x * 2 + 1]);
}

void build(int x, int l, int r) {
    tag1[x] = tag2[x] = 0;
    if (l == r) {
        tree[x] = mx[x] = a[l];
        return;
    }
    int mid = l + r >> 1;
    build(x * 2, l, mid);
    build(x * 2 + 1, mid + 1, r);
    up(x);
}

void update(int x, int l, int r, int ql, int qr, int w) {
    if (ql <= l && r <= qr) {
        down_sum1(x, l, r, w);
        return;
    }
    down(x, l, r);
    int mid = l + r >> 1;
    if (ql <= mid) update(x * 2, l, mid, ql, qr, w);
    if (qr > mid) update(x * 2 + 1, mid + 1, r, ql, qr, w);
    up(x);
}

void update2(int x, int l, int r, int ql, int qr, int w) {
    if (ql <= l && r <= qr) {
        down_sum2(x, l, r, w);
        return;
    }
    down(x, l, r);
    int mid = l + r >> 1;
    if (ql <= mid) update2(x * 2, l, mid, ql, qr, w);
    if (qr > mid) update2(x * 2 + 1, mid + 1, r, ql, qr, w);
    up(x);
}

int query(int x, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) return tree[x];
    down(x, l, r);
    int mid = l + r >> 1, res = 0;
    if (ql <= mid) res += query(x * 2, l, mid, ql, qr);
    if (qr > mid) res += query(x * 2 + 1, mid + 1, r, ql, qr);
    return res;
}

int query2(int x, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) return mx[x];
    down(x, l, r);
    int mid = l + r >> 1, res = -1e18;
    if (ql <= mid) res = max(query2(x * 2, l, mid, ql, qr), res);
    if (qr > mid) res = max(query2(x * 2 + 1, mid + 1, r, ql, qr), res);
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
        } else if (op == 2) {
            int x;
            cin >> x;
            update2(1, 1, n, l, r, x);
        } else if (op == 3) cout << query(1, 1, n, l, r) << endl;
        else cout << query2(1, 1, n, l, r) << endl;
    }
}