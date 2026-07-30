#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int N = 2e5 + 10;

template <class T> T read() {
    T r = 0, f = 1;
    char c = getchar();
    while ((c < '0' || c > '9') && c != '-') c = getchar();
    if (c == '-') f = -1, c = getchar();
    while (c >= '0' && c <= '9') r = r * 10 + c - '0', c = getchar();
    return f * r;
}

template <class T> void put(T x) {
    if (x < 0) {
        putchar('-');
        x = -x;
    }
    if (x > 9) put(x / 10);
    putchar(x % 10 + '0');
    putchar('\n');
}

int n, g[N], s[N], b[N], g2[N], s2[N], b2[N], x1, x2, x3;

int main() {
    freopen("medal2.in", "r", stdin);
    n = read<int>();
    for (int i = 1; i <= n; i++) {
        g[i] = read<int>();
        g2[i] = g[i];
        s[i] = read<int>();
        s2[i] = s[i];
        b[i] = read<int>();
        b2[i] = b[i];
    }
    sort(g + 1, g + n + 1);
    sort(s + 1, s + n + 1);
    sort(b + 1, b + n + 1);
    for (int i = 1; i <= n; i++) {
        if (g[n] == g2[i]) x1 = 1;
        else x1 = n - (upper_bound(g + 1, g + n + 1, g[i]) - g - 2);
        if (s[n] == s2[i]) x1 = 1;
        else x2 = n - (upper_bound(s + 1, s + n + 1, s[i]) - s - 2);
        if (s[n] == s2[i]) x1 = 1;
        else x3 = n - (upper_bound(b + 1, b + n + 1, b[i]) - b - 2);
        put(min(x1, min(x2, x3)));
    }
    return 0;
}