#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const i64 eps = 1e-3;
const int N = 2e5 + 10;

i64 l, r, sum[N];
int n, xi[N], yi[N], si[N];

bool check(i64 x) {
    i64 sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += (i64)si[i] / x;
        if (sum > yi[i]) return false;
        if (sum < xi[i]) sum = xi[i];
    }
    return true;
}

int main() {
    cin >> n;
    for (register int i = 1; i <= n; i++) {
        cin >> xi[i] >> yi[i] >> si[i];
        r += (i64)si[i];
        sum[i] = sum[i - 1] + (i64)si[i];
    }
    while (r - l >= eps) {
        i64 mid = (l + r) / 2;
        if (check(mid)) r = mid;
        else l = mid;
    }
    printf("%0.2Lf\n", r);
    return 0;
}
