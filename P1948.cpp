#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, p, k;
    if (!(cin >> n >> p >> k)) {
        return 0;
    }
    vector<vector<pair<int,int>>> adj(n + 1);
    vector<int> weights;
    weights.reserve(p + 1);

    for (int i = 0; i < p; ++i) {
        int a, b, l;
        cin >> a >> b >> l;
        adj[a].emplace_back(b, l);
        adj[b].emplace_back(a, l);
        weights.push_back(l);
    }
    weights.push_back(0);
    sort(weights.begin(), weights.end());
    weights.erase(unique(weights.begin(), weights.end()), weights.end());

    auto can = [&](int maxLen) {
        deque<int> dq;
        vector<int> dist(n + 1, INT_MAX);
        dist[1] = 0;
        dq.push_back(1);
        while (!dq.empty()) {
            int u = dq.front();
            dq.pop_front();
            int d = dist[u];
            if (d > k) {
                continue;
            }
            if (u == n) {
                return true;
            }
            for (auto &pr : adj[u]) {
                int v = pr.first;
                int l = pr.second;
                int w = (l > maxLen ? 1 : 0);
                int nd = d + w;
                if (nd < dist[v]) {
                    dist[v] = nd;
                    if (w == 0) {
                        dq.push_front(v);
                    } else {
                        dq.push_back(v);
                    }
                }
            }
        }
        return dist[n] <= k;
    };

    int lo = -1, hi = (int)weights.size();
    while (lo + 1 < hi) {
        int mid = lo + (hi - lo) / 2;
        if (can(weights[mid])) {
            hi = mid;
        } else {
            lo = mid;
        }
    }

    if (hi == (int)weights.size()) {
        cout << -1;
    } else {
        cout << weights[hi];
    }
    return 0;
}
