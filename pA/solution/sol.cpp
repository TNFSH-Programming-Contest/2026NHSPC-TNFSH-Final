#include <bits/stdc++.h>

using namespace std;

using int64 = long long;

namespace {

const int64 INF = (1LL << 60);

struct Edge {
    int to;
    int reverse;
    int capacity;
    int cost;
};

class MinCostFlow {
public:
    explicit MinCostFlow(int vertex_count)
        : graph(vertex_count), potential(vertex_count, 0),
          distance(vertex_count), parent_vertex(vertex_count),
          parent_edge(vertex_count) {}

    void addEdge(int from, int to, int capacity, int cost) {
        Edge forward = {to, static_cast<int>(graph[to].size()), capacity, cost};
        Edge backward = {from, static_cast<int>(graph[from].size()), 0, -cost};
        graph[from].push_back(forward);
        graph[to].push_back(backward);
    }

    pair<int, int64> run(int source, int sink, int flow_limit) {
        int flow = 0;
        int64 total_cost = 0;

        while (flow < flow_limit && shortestPath(source, sink)) {
            int augment = flow_limit - flow;
            for (int v = sink; v != source; v = parent_vertex[v]) {
                const Edge& edge = graph[parent_vertex[v]][parent_edge[v]];
                augment = min(augment, edge.capacity);
            }

            for (int v = sink; v != source; v = parent_vertex[v]) {
                Edge& edge = graph[parent_vertex[v]][parent_edge[v]];
                total_cost += static_cast<int64>(augment) * edge.cost;
                edge.capacity -= augment;
                graph[v][edge.reverse].capacity += augment;
            }
            flow += augment;
        }

        return make_pair(flow, total_cost);
    }

private:
    bool shortestPath(int source, int sink) {
        fill(distance.begin(), distance.end(), INF);
        fill(parent_vertex.begin(), parent_vertex.end(), -1);
        distance[source] = 0;

        using State = pair<int64, int>;
        priority_queue<State, vector<State>, greater<State> > queue;
        queue.push(make_pair(0, source));

        while (!queue.empty()) {
            const int64 current_distance = queue.top().first;
            const int u = queue.top().second;
            queue.pop();
            if (current_distance != distance[u]) {
                continue;
            }
            if (u == sink) {
                // All remaining queue keys are at least dist(sink).  For the
                // potential update they can all be safely capped there.
                break;
            }

            for (int id = 0; id < static_cast<int>(graph[u].size()); ++id) {
                const Edge& edge = graph[u][id];
                if (edge.capacity == 0) {
                    continue;
                }

                const int v = edge.to;
                const int64 reduced_cost =
                    static_cast<int64>(edge.cost) + potential[u] - potential[v];
                const int64 candidate = current_distance + reduced_cost;
                if (candidate >= distance[v]) {
                    continue;
                }

                distance[v] = candidate;
                parent_vertex[v] = u;
                parent_edge[v] = id;
                queue.push(make_pair(candidate, v));
            }
        }

        if (distance[sink] == INF) {
            return false;
        }

        // Capping unreachable vertices by dist(sink) preserves feasible
        // potentials even if a later augmentation changes reachability.
        for (int v = 0; v < static_cast<int>(graph.size()); ++v) {
            potential[v] += min(distance[v], distance[sink]);
        }
        return true;
    }

    vector<vector<Edge> > graph;
    vector<int64> potential;
    vector<int64> distance;
    vector<int> parent_vertex;
    vector<int> parent_edge;
};

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    const int side = n - 1;
    const int left_base = 0;
    const int right_base = side;
    const int source = 2 * side;
    const int sink = source + 1;

    MinCostFlow flow(sink + 1);
    for (int i = 0; i < side; ++i) {
        flow.addEdge(source, left_base + i, 1, 0);
        flow.addEdge(right_base + i, sink, 1, 0);
    }

    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        cin >> from >> to >> cost;

        // Vertex n chooses no outgoing road, and vertex 1 receives no guide.
        if (from == n || to == 1) {
            continue;
        }
        flow.addEdge(left_base + (from - 1), right_base + (to - 2), 1, cost);
    }

    const pair<int, int64> result = flow.run(source, sink, side);
    if (result.first != side) {
        cout << -1 << '\n';
    } else {
        cout << result.second << '\n';
    }
    return 0;
}
