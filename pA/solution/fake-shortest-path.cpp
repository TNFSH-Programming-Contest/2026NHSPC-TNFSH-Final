#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int> > > graph(n + 1);
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        cin >> from >> to >> cost;
        graph[from].push_back({to, cost});
    }
    const long long INF = (1LL << 60);
    vector<long long> distance(n + 1, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int> >,
                   greater<pair<long long, int> > > queue;
    distance[1] = 0;
    queue.push({0, 1});
    while (!queue.empty()) {
        const long long current = queue.top().first;
        const int u = queue.top().second;
        queue.pop();
        if (current != distance[u]) continue;
        for (const auto& edge : graph[u]) {
            if (distance[edge.first] > current + edge.second) {
                distance[edge.first] = current + edge.second;
                queue.push({distance[edge.first], edge.first});
            }
        }
    }
    cout << (distance[n] == INF ? -1 : distance[n]) << '\n';
}
