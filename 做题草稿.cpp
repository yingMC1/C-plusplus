#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 2e5 + 10;

template <class T> T read() {
    T x = 0;
    int f = 1;
    char ch = getchar();
    while ((ch < '0' || ch > '9') && ch != '-') ch = getchar();
    if (ch == '-') {
        f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + (ch - '0');
        ch = getchar();
    }
    return x * f;
}

template <class T, class... Args> void read(T &first, Args &...rest) {
    first = read<T>();
    if constexpr (sizeof...(rest) > 0) { read<T>(rest...); }
}

int n, m;

int main() {
    read(n, m);
    vector<i64> l(n), r(n);
    for (int i = 0; i < n; ++i) {
        i64 a, b;
        read(a, b);
        l[i] = a;
        r[i] = a + b;
    }
    sort(l.begin(), l.end());
    sort(r.begin(), r.end());
    while (m--) {
        long long x, y;
        cin >> x >> y;
        long long end = x + y;
        int cnt_l = lower_bound(l.begin(), l.end(), end) - l.begin();
        int cnt_r = upper_bound(r.begin(), r.end(), x) - r.begin();
        cout << cnt_l - cnt_r << endl;
    }
    return 0;
}