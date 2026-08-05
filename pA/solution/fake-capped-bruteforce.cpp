#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    const int side = n - 1;
    const int INF = 1000000007;
    vector<vector<int>> cost(side, vector<int>(side, INF));
    for (int i = 0; i < m; ++i) {
        int from, to, weight;
        cin >> from >> to >> weight;
        if (from == n || to == 1) continue;
        cost[from - 1][to - 2] = min(cost[from - 1][to - 2], weight);
    }

    vector<int> assignment(side);
    iota(assignment.begin(), assignment.end(), 0);
    int64 best = (1LL << 60);
    constexpr int MAGIC = 10000;

    for (int iteration = 0; iteration < MAGIC; ++iteration) {
        int64 candidate = 0;
        bool feasible = true;
        for (int left = 0; left < side; ++left) {
            const int edgeCost = cost[left][assignment[left]];
            if (edgeCost == INF) {
                feasible = false;
                break;
            }
            candidate += edgeCost;
        }
        if (feasible) best = min(best, candidate);
        if (!next_permutation(assignment.begin(), assignment.end())) break;
    }

    cout << (best == (1LL << 60) ? -1 : best) << '\n';
}
