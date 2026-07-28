#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define int ll
signed main() {
    int n, m; cin >> n >> m;
    int N = 2*n + 2;
    int S = n*2 + 1;
    int T = n*2 + 2;
    vector<array<int, 4> > f(N+1);
    auto add_edge = [&](int a, int b, int F, int C) -> void {
        f[a].push_back({b, F, C, (int)f[b].size()});
        f[b].push_back({a, 0, -C, (int)f[a].size()-1});
    };
    for(int i = 1; i <= m; i++) {
        int a, b, c; cin >> a >> b >> c;
        add_edge(a, b+n, 1, c);
    }
    for(int i = 1; i <= n-1; i++) {
        add_edge(S, i, 1, 0);
    }
    for(int i = 2; i <= n; i++) {
        add_edge(i+n, T, 1, 0);
    }
    
}
