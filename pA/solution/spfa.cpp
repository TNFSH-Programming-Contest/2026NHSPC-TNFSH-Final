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

class SpfaMinCostFlow {
public:
    explicit SpfaMinCostFlow(int vertex_count)
        : graph(vertex_count), distance(vertex_count),
          parent_vertex(vertex_count), parent_edge(vertex_count),
          in_queue(vertex_count) {}

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
            for (int v = sink; v != source; v = parent_vertex[v]) {
                Edge& edge = graph[parent_vertex[v]][parent_edge[v]];
                total_cost += edge.cost;
                --edge.capacity;
                ++graph[v][edge.reverse].capacity;
            }
            ++flow;
        }
        return make_pair(flow, total_cost);
    }

private:
    bool shortestPath(int source, int sink) {
        fill(distance.begin(), distance.end(), INF);
        fill(parent_vertex.begin(), parent_vertex.end(), -1);
        fill(in_queue.begin(), in_queue.end(), false);

        queue<int> queue;
        distance[source] = 0;
        in_queue[source] = true;
        queue.push(source);

        while (!queue.empty()) {
            const int u = queue.front();
            queue.pop();
            in_queue[u] = false;

            for (int id = 0; id < static_cast<int>(graph[u].size()); ++id) {
                const Edge& edge = graph[u][id];
                if (edge.capacity == 0 ||
                    distance[edge.to] <= distance[u] + edge.cost) {
                    continue;
                }

                distance[edge.to] = distance[u] + edge.cost;
                parent_vertex[edge.to] = u;
                parent_edge[edge.to] = id;
                if (!in_queue[edge.to]) {
                    in_queue[edge.to] = true;
                    queue.push(edge.to);
                }
            }
        }
        return distance[sink] != INF;
    }

    vector<vector<Edge> > graph;
    vector<int64> distance;
    vector<int> parent_vertex;
    vector<int> parent_edge;
    vector<char> in_queue;
};

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    const int side = n - 1;
    const int right_base = side;
    const int source = 2 * side;
    const int sink = source + 1;
    SpfaMinCostFlow flow(sink + 1);

    for (int i = 0; i < side; ++i) {
        flow.addEdge(source, i, 1, 0);
        flow.addEdge(right_base + i, sink, 1, 0);
    }
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        cin >> from >> to >> cost;
        if (from != n && to != 1) {
            flow.addEdge(from - 1, right_base + to - 2, 1, cost);
        }
    }

    const pair<int, int64> result = flow.run(source, sink, side);
    cout << (result.first == side ? result.second : -1) << '\n';
    return 0;
}
