#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<char> outgoing(n + 1), incoming(n + 1);
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        cin >> from >> to >> cost;
        if (from != n && to != 1) {
            outgoing[from] = true;
            incoming[to] = true;
        }
    }
    for (int v = 1; v < n; ++v)
        if (!outgoing[v]) { cout << -1 << '\n'; return 0; }
    for (int v = 2; v <= n; ++v)
        if (!incoming[v]) { cout << -1 << '\n'; return 0; }
    cout << 0 << '\n';
}
