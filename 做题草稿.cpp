#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int v, g;
    cin >> v;
    vector<int> need(v);
    for (int i = 0; i < v; ++i) cin >> need[i];

    cin >> g;
    vector<vector<int>> feed(g, vector<int>(v));
    for (int i = 0; i < g; ++i) {
        for (int j = 0; j < v; ++j) cin >> feed[i][j];
    }

    int totalMasks = 1 << g;
    vector<int> bestSel;
    int bestSize = 1 << 30;

    for (int mask = 0; mask < totalMasks; ++mask) {
        vector<int> curSel;
        vector<int> sum(v, 0);

        for (int i = 0; i < g; ++i) {
            if (mask & (1 << i)) {
                curSel.push_back(i + 1);
                for (int j = 0; j < v; ++j) { sum[j] += feed[i][j]; }
            }
        }

        bool ok = true;
        for (int j = 0; j < v; ++j) {
            if (sum[j] < need[j]) {
                ok = false;
                break;
            }
        }

        if (!ok) continue;

        int sz = (int)curSel.size();
        if (sz < bestSize || (sz == bestSize && curSel < bestSel)) {
            bestSize = sz;
            bestSel = curSel;
        }
    }

    cout << bestSize;
    for (int x : bestSel) cout << ' ' << x;
    cout << '\n';
    return 0;
}