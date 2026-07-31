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

int main() {
    int n, m;
    read<int>(n, m);
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) read<int>(a[i]);
    while (m--) {
        int q = read<int>();
        int l = 1, r = n, ans = -1;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (a[mid] == q) {
                ans = mid;
                break;
            } else if (a[mid] < q) {
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}