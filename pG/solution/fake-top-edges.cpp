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
    vector<long long> costs(m);
    long long total = 0;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v >> costs[i];
        total += costs[i];
    }
    sort(costs.rbegin(), costs.rend());
    long long saved = 0;
    for (int i = 0; i < n - 1; ++i)
        saved += costs[i];
    cout << total - saved << '\n';
}
