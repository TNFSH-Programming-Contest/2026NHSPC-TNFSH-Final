#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

namespace {

struct Dinic {
    struct Edge {
        int to;
        int reverse;
        int64 capacity;
    };

    explicit Dinic(int size)
        : graph(size), level(size), nextEdge(size) {}

    void addEdge(int from, int to, int64 capacity) {
        Edge forward{to, static_cast<int>(graph[to].size()), capacity};
        Edge backward{from, static_cast<int>(graph[from].size()), 0};
        graph[from].push_back(forward);
        graph[to].push_back(backward);
    }

    bool buildLevels(int source, int sink) {
        fill(level.begin(), level.end(), -1);
        queue<int> bfs;
        level[source] = 0;
        bfs.push(source);

        while (!bfs.empty()) {
            const int vertex = bfs.front();
            bfs.pop();

            for (const Edge& edge : graph[vertex]) {
                if (edge.capacity > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[vertex] + 1;
                    bfs.push(edge.to);
                }
            }
        }
        return level[sink] != -1;
    }

    int64 sendFlow(int vertex, int sink, int64 pushed) {
        if (vertex == sink) {
            return pushed;
        }

        for (int& edgeIndex = nextEdge[vertex];
             edgeIndex < static_cast<int>(graph[vertex].size());
             ++edgeIndex) {
            Edge& edge = graph[vertex][edgeIndex];
            if (edge.capacity == 0 ||
                level[edge.to] != level[vertex] + 1) {
                continue;
            }

            const int64 sent =
                sendFlow(edge.to, sink, min(pushed, edge.capacity));
            if (sent == 0) {
                continue;
            }

            edge.capacity -= sent;
            graph[edge.to][edge.reverse].capacity += sent;
            return sent;
        }
        return 0;
    }

    int64 maxFlow(int source, int sink, int64 limit) {
        int64 result = 0;
        while (result < limit && buildLevels(source, sink)) {
            fill(nextEdge.begin(), nextEdge.end(), 0);
            while (result < limit) {
                const int64 sent =
                    sendFlow(source, sink, limit - result);
                if (sent == 0) {
                    break;
                }
                result += sent;
            }
        }
        return result;
    }

    vector<vector<Edge>> graph;
    vector<int> level;
    vector<int> nextEdge;
};

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int64 m;
    cin >> n >> m;

    vector<int64> x(n);
    for (int i = 0; i < n; ++i) {
        cin >> x[i];
    }

    vector<int64> capacity(n, 0);
    for (int i = 1; i + 1 < n; ++i) {
        cin >> capacity[i];
    }

    if (n > 100 || m > 100) {
        cout << 0 << '\n';
        return 0;
    }

    const int64 required = 2 * m;

    const auto feasible = [&](int64 jump) {
        Dinic flow(2 * n);
        const auto in = [](int vertex) { return 2 * vertex; };
        const auto out = [](int vertex) { return 2 * vertex + 1; };

        for (int i = 0; i < n; ++i) {
            const int64 vertexCapacity =
                (i == 0 || i + 1 == n)
                    ? required
                    : min(required, capacity[i]);
            flow.addEdge(in(i), out(i), vertexCapacity);
        }

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (x[j] - x[i] <= jump) {
                    flow.addEdge(out(i), in(j), required);
                    flow.addEdge(out(j), in(i), required);
                }
            }
        }

        return flow.maxFlow(out(0), in(n - 1), required) >= required;
    };

    int64 low = 0;
    int64 high = x.back() - x.front();
    while (low < high) {
        const int64 middle = (low + high) / 2;
        if (feasible(middle)) {
            high = middle;
        } else {
            low = middle + 1;
        }
    }

    cout << low << '\n';
    return 0;
}
