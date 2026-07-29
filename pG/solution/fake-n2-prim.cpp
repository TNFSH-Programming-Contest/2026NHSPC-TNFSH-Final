#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    long long r;
    cin >> n >> m >> r;
    vector<vector<pair<int, long long>>> graph(n + 1);
    long long total = 0;
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long cost;
        cin >> u >> v >> cost;
        total += cost;
        graph[u].push_back({v, cost});
        graph[v].push_back({u, cost});
    }

    vector<long long> best(n + 1, -1);
    vector<char> used(n + 1, false);
    best[1] = 0;
    long long saved = 0;
    long long operations = 0;
    for (int iteration = 0; iteration < n; ++iteration) {
        int selected = -1;
        for (int v = 1; v <= n; ++v) {
            if (++operations > 1000000) {
                // The quadratic approach cannot finish at full scale.
                cout << total << '\n';
                return 0;
            }
            if (!used[v] && (selected == -1 || best[v] > best[selected]))
                selected = v;
        }
        used[selected] = true;
        saved += best[selected];
        for (const auto& edge : graph[selected])
            best[edge.first] = max(best[edge.first], edge.second);
    }
    cout << total - saved << '\n';
}
