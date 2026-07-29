#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

struct Edge { int to, reverse, capacity, cost; };

class Flow {
public:
    explicit Flow(int n) : graph(n), potential(n), distance(n),
        parent_vertex(n), parent_edge(n) {}
    void add(int from, int to, int cost) {
        Edge a = {to, static_cast<int>(graph[to].size()), 1, cost};
        Edge b = {from, static_cast<int>(graph[from].size()), 0, -cost};
        graph[from].push_back(a); graph[to].push_back(b);
    }
    pair<int, int64> run(int source, int sink, int limit) {
        int sent = 0; int64 answer = 0; const int64 INF = (1LL << 60);
        while (sent < limit) {
            fill(distance.begin(), distance.end(), INF);
            fill(parent_vertex.begin(), parent_vertex.end(), -1);
            priority_queue<pair<int64, int>, vector<pair<int64, int> >,
                           greater<pair<int64, int> > > queue;
            distance[source] = 0; queue.push({0, source});
            while (!queue.empty()) {
                const int64 current = queue.top().first;
                const int u = queue.top().second; queue.pop();
                if (current != distance[u]) continue;
                if (u == sink) break;
                for (int id = 0; id < static_cast<int>(graph[u].size()); ++id) {
                    const Edge& edge = graph[u][id];
                    if (!edge.capacity) continue;
                    const int64 candidate = current + edge.cost
                        + potential[u] - potential[edge.to];
                    if (candidate < distance[edge.to]) {
                        distance[edge.to] = candidate;
                        parent_vertex[edge.to] = u; parent_edge[edge.to] = id;
                        queue.push({candidate, edge.to});
                    }
                }
            }
            if (distance[sink] == INF) break;
            for (int v = 0; v < static_cast<int>(graph.size()); ++v)
                potential[v] += min(distance[v], distance[sink]);
            for (int v = sink; v != source; v = parent_vertex[v]) {
                Edge& edge = graph[parent_vertex[v]][parent_edge[v]];
                answer += edge.cost; --edge.capacity;
                ++graph[v][edge.reverse].capacity;
            }
            ++sent;
        }
        return {sent, answer};
    }
private:
    vector<vector<Edge> > graph;
    vector<int64> potential, distance;
    vector<int> parent_vertex, parent_edge;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    const int side = n - 1, source = 2 * side, sink = source + 1;
    Flow flow(sink + 1);
    for (int i = 0; i < side; ++i) {
        flow.add(source, i, 0); flow.add(side + i, sink, 0);
    }
    for (int i = 0; i < m; ++i) {
        int from, to, cost; cin >> from >> to >> cost;
        if (from != n && to != 1)
            flow.add(from - 1, side + to - 2, cost);
        if (to != n && from != 1)
            flow.add(to - 1, side + from - 2, cost);
    }
    const auto result = flow.run(source, sink, side);
    cout << (result.first == side ? result.second : -1) << '\n';
}
