#include <climits>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

using int64 = long long;

struct Edge {
    int from;
    int to;
    int64 cost;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    int64 r;
    cin >> n >> m >> r;

    vector<Edge> edges(m);
    vector<vector<pair<int, int>>> graph(n + 1);
    vector<int> degree(n + 1, 0);

    for (int i = 0; i < m; ++i) {
        cin >> edges[i].from
            >> edges[i].to
            >> edges[i].cost;

        int u = edges[i].from;
        int v = edges[i].to;

        // 此子任務只在意底層連接結構。
        graph[u].push_back({v, i});
        graph[v].push_back({u, i});

        ++degree[u];
        ++degree[v];
    }

    queue<int> que;

    for (int v = 1; v <= n; ++v) {
        if (degree[v] == 1)
            que.push(v);
    }

    vector<char> removed(n + 1, false);

    while (!que.empty()) {
        int u = que.front();
        que.pop();

        if (removed[u] || degree[u] != 1)
            continue;

        removed[u] = true;

        for (auto [v, edge_id] : graph[u]) {
            if (removed[v])
                continue;

            --degree[v];

            if (degree[v] == 1)
                que.push(v);
        }
    }

    int64 answer = LLONG_MAX;

    for (const Edge& edge : edges) {
        if (!removed[edge.from] &&
            !removed[edge.to]) {
            answer = min(answer, edge.cost);
        }
    }

    cout << answer << '\n';

    return 0;
}