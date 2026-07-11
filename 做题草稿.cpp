#include <bits/stdc++.h>
using namespace std;
using ll = long long;
struct Edge { int u, v; ll w; };
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<Edge> edges;
    edges.reserve(m * 2 + n + 5);
    for (int i = 0; i < m; i++) {
        int type;
        cin >> type;
        if (type == 1) {
            int a, b, c;
            cin >> a >> b >> c;
            edges.push_back({a, b, -c});
        } else if (type == 2) {
            int a, b, c;
            cin >> a >> b >> c;
            edges.push_back({b, a, c});
        } else if (type == 3) {
            int a, b;
            cin >> a >> b;
            edges.push_back({a, b, 0});
            edges.push_back({b, a, 0});
        }
    }
    for (int i = 1; i <= n; i++) {
        edges.push_back({0, i, 0});
    }

    int nodeCount = n + 1;
    vector<ll> dist(nodeCount, 0);
    bool updated = false;
    for (int iter = 0; iter < nodeCount; iter++) {
        updated = false;
        for (auto &e : edges) {
            if (dist[e.v] > dist[e.u] + e.w) {
                dist[e.v] = dist[e.u] + e.w;
                updated = true;
            }
        }
        if (!updated) break;
    }
    if (updated) {
        cout << "No";
    } else {
        cout << "Yes";
    }
    return 0;
}
