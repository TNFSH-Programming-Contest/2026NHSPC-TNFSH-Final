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
    vector<long long> bestOutgoing(n + 1, 0);
    long long total = 0;
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long cost;
        cin >> u >> v >> cost;
        total += cost;
        bestOutgoing[u] = max(bestOutgoing[u], cost);
    }

    // Wrong model: treats the retained edges as an outgoing arborescence.
    // Independent local choices can form cycles and need not form a forest.
    long long saved = 0;
    for (int vertex = 1; vertex < n; ++vertex)
        saved += bestOutgoing[vertex];
    cout << total - saved << '\n';
    return 0;
}
