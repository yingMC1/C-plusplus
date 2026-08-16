#include <bits/stdc++.h>

#define endl '\n'
using namespace std;
using i64 = long long;
const int N = 100010;

int n, b[N << 1], s[N << 1], sum[N << 3], mi[N << 3], ci[N << 3], tree[N << 3], tot;

struct I {
    int a, b, k;
} a[N];

void up(int x, int l, int r) {
    mi[x] = mi[l];
    tree[x] = tree[l];
    ci[x] = min(ci[l], mi[r]);
}

void push_up(int x) {
    int l = x * 2, r = x * 2 + 1;
    sum[x] = sum[l] + sum[r];
    if (mi[l] < mi[r]) up(x, l, r);
    else if (mi[l] > mi[r]) up(x, r, l);
    else {
        up(x, l, r);
        tree[x] += tree[r];
    }
}

void down(int x, int w) {
    sum[x] += (w - mi[x]) * tree[x];
    mi[x] = w;
}

void push_down(int x) {
    int l = x * 2, r = x * 2 + 1;
    if (mi[l] < mi[x]) down(l, mi[x]);
    if (mi[r] < mi[x]) down(r, mi[x]);
}

void build(int x, int l, int r) {
    if (l == r) {
        sum[x] = mi[x] = 0;
        ci[x] = 0x3f3f3f3f;
        tree[x] = s[l];
        return;
    }
    int mid = l + r >> 1;
    build(x * 2, l, mid);
    build(x * 2 + 1, mid + 1, r);
    push_up(x);
}

void update(int x, int l, int r, int ql, int qr, int w) {
    if (mi[x] >= w) return;
    if (ql <= l && r <= qr && w < ci[x]) {
        down(x, w);
        return;
    }
    push_down(x);
    int mid = l + r >> 1;
    if (ql <= mid) update(x * 2, l, mid, ql, qr, w);
    if (qr > mid) update(x * 2 + 1, mid + 1, r, ql, qr, w);
    push_up(x);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i].a >> a[i].b >> a[i].k;
        b[++tot] = a[i].a;
        b[++tot] = a[i].b;
    }
    sort(b + 1, b + tot + 1);
    tot = unique(b + 1, b + tot + 1) - (b + 1);
    for (int i = 1; i <= tot; i++)
        s[i] = b[i] - b[i - 1];
    build(1, 1, tot);
    for (int i = 1; i <= n; i++) {
        int l = lower_bound(b + 1, b + tot + 1, a[i].a) - b + 1;
        int r = lower_bound(b + 1, b + tot + 1, a[i].b) - b;
        if (l <= r) update(1, 1, tot, l, r, a[i].k);
    }
    cout << sum[1] << endl;
    return 0;
}
