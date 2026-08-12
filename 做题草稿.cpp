#include <bits/stdc++.h>

#define lowbit(x) x & -(x)
#define endl '\n'
using namespace std;
using i64 = long long;
const int N = 5e5 + 5;

int n, m, tree[N];

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
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int x;
        cin >> x;
        update(i, x);
    }
    return 0;
}