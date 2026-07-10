#include<bits/stdc++.h>
using namespace std;
const int N = 1e4 + 5;
int n, p, k;
vector<pair<int, int>> g[N];
vector<int> w;
int main()
{
	cin >> n >> p >> k;
	for(int i = 1; i <= n; i++)
	{
		int a, b, l;
		cin >> a >> b >> l;
		g[a].emplace_back(b, l);
		g[a].emplace_back(a, l);
		w.push_back(l);
	}
	return 0;
}	