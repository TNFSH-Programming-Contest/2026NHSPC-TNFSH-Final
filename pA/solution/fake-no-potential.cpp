#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

struct Edge { int to, reverse, capacity, cost; };

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    const int side = n - 1, source = 2 * side, sink = source + 1;
    vector<vector<Edge> > graph(sink + 1);
    const auto addEdge = [&](int from, int to, int cost) {
        Edge forward = {to, static_cast<int>(graph[to].size()), 1, cost};
        Edge backward = {from, static_cast<int>(graph[from].size()), 0, -cost};
        graph[from].push_back(forward);
        graph[to].push_back(backward);
    };
    for (int i = 0; i < side; ++i) {
        addEdge(source, i, 0);
        addEdge(side + i, sink, 0);
    }
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        cin >> from >> to >> cost;
        if (from != n && to != 1)
            addEdge(from - 1, side + to - 2, cost);
    }

    const int64 INF = (1LL << 60);
    int64 answer = 0;
    for (int flow = 0; flow < side; ++flow) {
        vector<int64> distance(sink + 1, INF);
        vector<int> parent_vertex(sink + 1, -1), parent_edge(sink + 1, -1);
        vector<char> finalized(sink + 1, false);
        priority_queue<pair<int64, int>, vector<pair<int64, int> >,
                       greater<pair<int64, int> > > queue;
        distance[source] = 0;
        queue.push({0, source});
        while (!queue.empty()) {
            const int64 current = queue.top().first;
            const int u = queue.top().second;
            queue.pop();
            if (finalized[u]) continue;
            finalized[u] = true;
            if (u == sink) break;
            for (int id = 0; id < static_cast<int>(graph[u].size()); ++id) {
                const Edge& edge = graph[u][id];
                if (edge.capacity && distance[edge.to] > current + edge.cost) {
                    distance[edge.to] = current + edge.cost;
                    parent_vertex[edge.to] = u;
                    parent_edge[edge.to] = id;
                    queue.push({distance[edge.to], edge.to});
                }
            }
        }
        if (parent_vertex[sink] == -1) {
            cout << -1 << '\n';
            return 0;
        }
        for (int v = sink; v != source; v = parent_vertex[v]) {
            Edge& edge = graph[parent_vertex[v]][parent_edge[v]];
            answer += edge.cost;
            --edge.capacity;
            ++graph[v][edge.reverse].capacity;
        }
    }
    cout << answer << '\n';
}
