#include<bits/stdc++.h>
using namespace std;
const int N = 1e4 + 5;
int n, p, k;
vector<pair<int, int>> g[N];
vector<int> w;
bool bfs(int x)
{
	deque<int> d;
	vector<int> dist(n + 1, INT_MAX);
	dist[1] = 0;
	d.push_back(1);
	while (!d.empty())
	{
		int u = d.front();
		d.pop_front();
		int du = dist[u];
		if (du > k) continue;
		if (u == n) return true;
		for (auto &to : g[u])
		{
			int b = to.first;
			int e = to.second;
			int cost = (e > x ? 1 : 0);
			if (du + cost < dist[b]) {
				dist[b] = du + cost;
				if (cost) d.push_back(b);
				else d.push_front(b);
			}
		}
	}
	return dist[n] <= k;
}
int main()
{
	cin >> n >> p >> k;
	for (int i = 1; i <= p; i++)
	{
		int a, b, l;
		cin >> a >> b >> l;
		g[a].emplace_back(b, l);
		g[b].emplace_back(a, l);
		w.push_back(l);
	}
	w.push_back(0);
	sort(w.begin(), w.end());
	w.erase(unique(w.begin(), w.end()), w.end());
	int lo = -1, hi = (int)w.size();
    while (lo + 1 < hi) {
        int mid = lo + (hi - lo) / 2;
        if (bfs(w[mid]))
		{
            hi = mid;
        } else
		{
            lo = mid;
        }
    }
    if (hi == (int)w.size())
	{
        cout << -1 << endl;
    } else {
        cout << w[hi] << endl;
    }
	return 0;
}	