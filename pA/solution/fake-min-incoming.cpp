#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    const int INF = 1000000007;
    vector<int> best(n + 1, INF);
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        cin >> from >> to >> cost;
        if (from != n && to != 1) best[to] = min(best[to], cost);
    }
    long long answer = 0;
    for (int v = 2; v <= n; ++v) {
        if (best[v] == INF) {
            cout << -1 << '\n';
            return 0;
        }
        answer += best[v];
    }
    cout << answer << '\n';
}
