#include <bits/stdc++.h>

#define endl '\n'
using namespace std;
using i64 = long long;
const int N = 5e5 + 5;

int n, m, tree[N << 2], tag[N << 2], a[N];

void up(int x) {
    tree[x] = tree[x * 2] + tree[x * 2 + 1];
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

void update(int x, int l, int r, int q, int w) {
    if (l == r) {
        tree[x] = w;
        return;
    }
    int mid = l + r >> 1;
    if (q <= mid) update(x * 2, l, mid, q, w);
    else update(x * 2 + 1, mid + 1, r, q, w);
    up(x);
}

int query(int x, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) return tree[x];
    int mid = l + r >> 1, res = 0;
    if (ql <= mid) res += query(x * 2, l, mid, ql, qr);
    if (qr > mid) res += query(x * 2 + 1, mid + 1, r, ql, qr);
    return res;
}

int main() {

    return 0;
}