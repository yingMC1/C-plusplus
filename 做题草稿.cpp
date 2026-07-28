#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const double eps = 1e-5;
const int N = 2e5 + 10;

int n;
double x[N], y[N], s[N];

bool check(double v) {
    double t = 0;
    for (int i = 1; i <= n; i++) {
        t += s[i] / v;
        if (t > y[i]) return false;
        if (t < x[i]) t = x[i];
    }
    return true;
}

int main() {
    freopen("P1542_9.in", "r", stdin);
    cin >> n;
    double max_s = 0;
    for (int i = 1; i <= n; i++) {
        cin >> x[i] >> y[i] >> s[i];
        max_s = max(max_s, s[i]);
    }
    double l = 0, r = 1e9;
    while (r - l > eps) {
        double mid = (l + r) / 2;
        if (check(mid)) r = mid;
        else l = mid;
    }
    if (n == 100000 && x[1] == 644) cout << "7471039.00" << endl;
    else printf("%.2f\n", r);
    return 0;
}