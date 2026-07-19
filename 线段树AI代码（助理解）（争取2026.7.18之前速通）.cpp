#include<bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;

long long tr[4 * N];   // 线段树存区间和
long long lazy[4 * N]; // 懒标记：区间加的延迟值
int a[N];              // 原数组

// 上传：用左右儿子更新当前节点
void pushup(int u) {
    tr[u] = tr[u << 1] + tr[u << 1 | 1];
}

// 下放懒标记到子节点
void pushdown(int u, int l, int r) {
    if (lazy[u] == 0) return; // 无标记直接返回
    int mid = (l + r) / 2;
    int lson = u << 1, rson = u << 1 | 1;

    // 左子区间长度 mid-l+1
    tr[lson] += lazy[u] * (mid - l + 1);
    lazy[lson] += lazy[u];

    // 右子区间长度 r-mid
    tr[rson] += lazy[u] * (r - mid);
    lazy[rson] += lazy[u];

    lazy[u] = 0; // 当前标记清空
}

// 建树 u节点，对应区间[l,r]
void build(int u, int l, int r) {
    if (l == r) {
        tr[u] = a[l];
        lazy[u] = 0;
        return;
    }
    int mid = (l + r) / 2;
    build(u << 1, l, mid);
    build(u << 1 | 1, mid + 1, r);
    pushup(u);
}

// 区间修改：[L,R] 加 val，当前节点u区间[l,r]
void update(int u, int l, int r, int L, int R, long long val) {
    if (L <= l && r <= R) { // 当前区间完全被覆盖，打懒标记
        tr[u] += val * (r - l + 1);
        lazy[u] += val;
        return;
    }
    pushdown(u, l, r); // 先下放标记再递归
    int mid = (l + r) / 2;
    if (L <= mid) update(u << 1, l, mid, L, R, val);
    if (R > mid) update(u << 1 | 1, mid + 1, r, L, R, val);
    pushup(u);
}

// 区间查询 [L,R] 和
long long query(int u, int l, int r, int L, int R) {
    if (L <= l && r <= R) return tr[u];
    pushdown(u, l, r);
    int mid = (l + r) / 2;
    long long res = 0;
    if (L <= mid) res += query(u << 1, l, mid, L, R);
    if (R > mid) res += query(u << 1 | 1, mid + 1, r, L, R);
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) cin >> a[i];
    build(1, 1, n);
    while (m--) {
        int op, l, r;
        long long x;
        cin >> op;
        if (op == 1) {
            // 区间加
            cin >> l >> r >> x;
            update(1, 1, n, l, r, x);
        } else {
            // 查询区间和
            cin >> l >> r;
            cout << query(1, 1, n, l, r) << '\n';
        }
    }
    return 0;
}
