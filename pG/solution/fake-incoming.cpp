#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    long long r;
    cin >> n >> m >> r;
    vector<long long> bestIncoming(n + 1, 0);
    long long total = 0;
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long cost;
        cin >> u >> v >> cost;
        total += cost;
        bestIncoming[v] = max(bestIncoming[v], cost);
    }
    long long saved = 0;
    for (int v = 2; v <= n; ++v)
        saved += bestIncoming[v];
    cout << total - saved << '\n';
}
