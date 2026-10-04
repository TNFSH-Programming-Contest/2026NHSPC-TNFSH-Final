#include <bits/stdc++.h>
using namespace std;

int n,m;
vector<long long> x, A;

bool work(long long limit){
    vector<long long> passing(n);
    passing[0] = 2*m;
	int l=0, r=1; //[l,r)內全達極限(若l跳不到r就死了)
	while(l<n){
	    if (r==l){
	        if (r==n-1) return 1;
    	    r++;
	    }
	    while (r<n && passing[l]>0){
	        if (x[r]-x[l]>limit) return 0;
	        if (passing[l]>=A[r]){
	            passing[l] -= A[r];
	            passing[r] += A[r];
	            r++;
	        }else{
	            passing[r]+= passing[l];
	            passing[l] = 0;
	        }
	    }
	    l++;
	}
	return 1;
}

int main(){
	ios::sync_with_stdio(0), cin.tie(0);
	cin >> n >> m;
	x = vector<long long>(n);
	A = vector<long long>(n,1e18);

	for (int i=0;i<n;i++) cin >> x[i];
	for (int i=1; i<n-1; i++) cin >> A[i];
	long long idx = 0, limit = x[n-1]-x[0]+1;
	for (long long jump=limit/2;jump>0; jump>>=1){
		while (idx+jump<limit && !work(idx+jump)) idx += jump;
	}

	cout << idx+1 << "\n";

}
