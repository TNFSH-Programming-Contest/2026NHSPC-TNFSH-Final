#define SPFA_SLF
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

class VariantSpfaMinCostFlow {
public:
    explicit VariantSpfaMinCostFlow(int vertex_count)
        : graph(vertex_count), distance(vertex_count), parent_vertex(vertex_count),
          parent_edge(vertex_count), in_queue(vertex_count), enqueue_count(vertex_count) {}

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
        return {flow, total_cost};
    }

private:
    uint32_t randomNumber() {
        random_state ^= random_state << 13;
        random_state ^= random_state >> 17;
        random_state ^= random_state << 5;
        return random_state;
    }

    void push(deque<int>& queue, int v) {
#if defined(SPFA_SLF)
        if (!queue.empty() && distance[v] < distance[queue.front()])
            queue.push_front(v);
        else
            queue.push_back(v);
#elif defined(SPFA_MCFX)
        const int limit = max(2, static_cast<int>(sqrt(graph.size())));
        if (enqueue_count[v] >= 2 && enqueue_count[v] <= limit)
            queue.push_front(v);
        else
            queue.push_back(v);
#else
        queue.push_back(v);
#endif
    }

    int pop(deque<int>& queue, int64& queue_sum) {
#if defined(SPFA_LLL)
        while (queue.size() > 1 &&
               distance[queue.front()] * static_cast<int64>(queue.size()) > queue_sum) {
            queue.push_back(queue.front());
            queue.pop_front();
        }
#elif defined(SPFA_RANDOM)
        const size_t at = randomNumber() % queue.size();
        swap(queue.front(), queue[at]);
#endif
        const int u = queue.front();
        queue.pop_front();
        queue_sum -= distance[u];
        return u;
    }

    bool shortestPath(int source, int sink) {
        fill(distance.begin(), distance.end(), INF);
        fill(parent_vertex.begin(), parent_vertex.end(), -1);
        fill(in_queue.begin(), in_queue.end(), false);
        fill(enqueue_count.begin(), enqueue_count.end(), 0);

        deque<int> queue;
        int64 queue_sum = 0;
        distance[source] = 0;
        in_queue[source] = true;
        enqueue_count[source] = 1;
        queue.push_back(source);

        while (!queue.empty()) {
            const int u = pop(queue, queue_sum);
            in_queue[u] = false;

            const int degree = graph[u].size();
#if defined(SPFA_RANDOM)
            const int start = degree == 0 ? 0 : randomNumber() % degree;
#else
            const int start = 0;
#endif
            for (int step = 0; step < degree; ++step) {
                const int id = (start + step) % degree;
                const Edge& edge = graph[u][id];
                if (edge.capacity == 0) continue;
                const int64 candidate = distance[u] + edge.cost;
                if (candidate >= distance[edge.to]) continue;

                if (in_queue[edge.to]) queue_sum -= distance[edge.to];
                distance[edge.to] = candidate;
                parent_vertex[edge.to] = u;
                parent_edge[edge.to] = id;
                if (in_queue[edge.to]) {
                    queue_sum += distance[edge.to];
                } else {
                    in_queue[edge.to] = true;
                    ++enqueue_count[edge.to];
                    queue_sum += distance[edge.to];
                    push(queue, edge.to);
                }
            }
        }
        return distance[sink] != INF;
    }

    vector<vector<Edge>> graph;
    vector<int64> distance;
    vector<int> parent_vertex;
    vector<int> parent_edge;
    vector<char> in_queue;
    vector<int> enqueue_count;
    uint32_t random_state = 0x9e3779b9U;
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
    VariantSpfaMinCostFlow flow(sink + 1);

    for (int i = 0; i < side; ++i) {
        flow.addEdge(source, i, 1, 0);
        flow.addEdge(right_base + i, sink, 1, 0);
    }
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        cin >> from >> to >> cost;
        if (from != n && to != 1)
            flow.addEdge(from - 1, right_base + to - 2, 1, cost);
    }

    const auto result = flow.run(source, sink, side);
    cout << (result.first == side ? result.second : -1) << '\n';
    return 0;
}

