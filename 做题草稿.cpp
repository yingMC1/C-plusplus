#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
const int N = 2e6 + 5;

i64 m, a[N];

int check(int l, int r, i64 sum){
	if(l > r)return -1;
    int mid = (l + r) / 2;
    if(a[mid] - sum == m)return mid;
    else if(a[mid] - sum > m)check(l, m - 1, sum);
    else if(a[mid] - sum < m)check(m + 1, r, sum);
}

int main(){
	cin >> m;
	a[1] = 1;
	for(int i = 2; i <= m; i++){
		a[i] = a[i - 1] + i;
	}
	for(int i = 0; i < m - 1; i++){
		int ans = check(1, i, 0);
		cout << ans << endl;
	}
}