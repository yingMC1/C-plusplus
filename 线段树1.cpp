#include<bits/stdc++.h>
using namespace std;
const int N=1e5+5;
long long tr[4*N],laz[4*N];
long long a[N];
void Pushup(int u){
	tr[u]=tr[u<<1]+tr[u<<1|1];
}
void Pushdown(int u,int l,int r){
	if(!laz[u]){
		return ;
	}
	int lson=u<<1,rson=u<<1|1,mid=(l+r)/2;
	tr[lson]+=laz[u]*(mid-l+1);
	laz[lson]+=laz[u];
	tr[rson]+=laz[u]*(r-mid);
	laz[rson]+=laz[u];
	laz[u]=0;
}
void Build(int l,int r,int u){
	if(l==r){
		tr[u]=a[l];
		return ;
	}
	Build(l,(l+r)/2,u<<1);
	Build((l+r)/2+1,r,u<<1|1);
	Pushup(u);
}
void Update(int u,int l,int r,int L,int R,long long val){
	if(L<=l&&r<=R){
		tr[u]+=val*(r-l+1);
		laz[u]+=val;
		return ;
	}
	Pushdown(u,l,r);
	int mid=(l+r)/2;
	if(L<=mid) Update(u<<1,l,mid,L,R,val);
	if(R>mid) Update(u<<1|1,mid+1,r,L,R,val);
	Pushup(u);
} 
long long Query(int u,int l,int r,int L,int R){
	if(L<=l&&r<=R) return tr[u];
	Pushdown(u,l,r);
	int mid=(l+r)/2;
	long long res=0;
	if(L<=mid) res+=Query(u<<1,l,mid,L,R);
	if(R>mid) res+=Query(u<<1|1,mid+1,r,L,R);
	return res;
}
int main(){
	ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
	int n,m;
	cin>>n>>m;
	for(int i=1;i<=n;i++){
		cin>>a[i];
	}
	Build(1,n,1);
	for(int i=1,s,x,y,z;i<=m;i++){
		cin>>s;
		if(s==1){
			cin>>x>>y>>z;
			Update(1,1,n,x,y,z);
		}else{
			cin>>x>>y;
			cout<<Query(1,1,n,x,y)<<endl;
		}
	}
	return 0;
}
