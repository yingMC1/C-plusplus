#include<bits/stdc++.h>
using namespace std;
const int N = 1e4 + 5;
int n, m, k;
int s, t;
vector<pair<int ,int>> g[N];
vector<int> w;
int main()
{
	cin >> n >> m >> k;
	cin >> s >> t;
	for (int i = 1; i <= m; ++i)
	{
		int a, b, c;
		cin >> a >> b >> c;
		g[a].emplace_back(b, c);
		g[b].emplace_back(a, c);
		w.push_back(c);
	}
	w.push_back(0);
	sort(w.begin(), w.end());
	w.erase(unique(w.begin(), w.end()), w.end());
	return 0;
}	