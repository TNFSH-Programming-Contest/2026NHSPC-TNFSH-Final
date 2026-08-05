#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    int cost;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    const int side = n - 1;
    const int INF = 1'000'000'000;
    vector<vector<Edge>> graph(side);
    vector<vector<int>> cost(side, vector<int>(side, INF));
    for (int i = 0; i < m; ++i) {
        int from, to, weight;
        cin >> from >> to >> weight;
        if (from == n || to == 1) continue;
        int left = from - 1;
        int right = to - 2;
        graph[left].push_back({right, weight});
        cost[left][right] = weight;
    }

    // First obtain any feasible matching.  Keeping input order is a common
    // cheap way to seed a local-search solution.
    vector<int> match_right(side, -1), seen(side);
    int stamp = 0;
    function<bool(int)> augment = [&](int left) {
        if (seen[left] == stamp) return false;
        seen[left] = stamp;
        for (const Edge& edge : graph[left]) {
            if (match_right[edge.to] == -1 || augment(match_right[edge.to])) {
                match_right[edge.to] = left;
                return true;
            }
        }
        return false;
    };
    for (int left = 0; left < side; ++left) {
        ++stamp;
        if (!augment(left)) {
            cout << -1 << '\n';
            return 0;
        }
    }

    vector<int> assigned(side);
    for (int right = 0; right < side; ++right)
        assigned[match_right[right]] = right;
    long long current = 0;
    for (int left = 0; left < side; ++left)
        current += cost[left][assigned[left]];
    long long best = current;

    mt19937 rng(712367821);
    uniform_real_distribution<double> probability(0.0, 1.0);
    const int iterations = 300000;
    for (int it = 0; it < iterations; ++it) {
        int a = rng() % side;
        int b = rng() % side;
        if (a == b) continue;
        int ca = assigned[a], cb = assigned[b];
        if (cost[a][cb] == INF || cost[b][ca] == INF) continue;

        long long delta = static_cast<long long>(cost[a][cb]) + cost[b][ca]
                        - cost[a][ca] - cost[b][cb];
        double progress = static_cast<double>(it) / iterations;
        double temperature = 100000.0 * pow(1e-7, progress);
        if (delta <= 0 || probability(rng) < exp(-delta / temperature)) {
            swap(assigned[a], assigned[b]);
            current += delta;
            best = min(best, current);
        }
    }

    cout << best << '\n';
}
