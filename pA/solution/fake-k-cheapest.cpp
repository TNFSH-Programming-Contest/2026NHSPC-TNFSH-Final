#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
const int KEEP = 8;
const int64 INF = (1LL << 60);

struct Edge {
    int to;
    int reverse;
    int capacity;
    int cost;
};

void addEdge(vector<vector<Edge> >& graph, int from, int to, int cost) {
    Edge forward = {to, static_cast<int>(graph[to].size()), 1, cost};
    Edge backward = {from, static_cast<int>(graph[from].size()), 0, -cost};
    graph[from].push_back(forward);
    graph[to].push_back(backward);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    const int side = n - 1;
    vector<vector<pair<int, int> > > outgoing(side);
    for (int i = 0; i < m; ++i) {
        int u, v, cost;
        cin >> u >> v >> cost;
        if (1 <= u && u < n && 2 <= v && v <= n)
            outgoing[u - 1].push_back({cost, v - 2});
    }

    const int source = 2 * side;
    const int sink = source + 1;
    vector<vector<Edge> > graph(sink + 1);
    for (int row = 0; row < side; ++row) {
        addEdge(graph, source, row, 0);
        sort(outgoing[row].begin(), outgoing[row].end());
        if (outgoing[row].size() > KEEP)
            outgoing[row].resize(KEEP);
        for (const pair<int, int>& edge : outgoing[row])
            addEdge(graph, row, side + edge.second, edge.first);
    }
    for (int column = 0; column < side; ++column)
        addEdge(graph, side + column, sink, 0);

    vector<int64> potential(sink + 1, 0);
    int flow = 0;
    int64 answer = 0;
    while (flow < side) {
        vector<int64> distance(sink + 1, INF);
        vector<int> parentVertex(sink + 1, -1);
        vector<int> parentEdge(sink + 1, -1);
        priority_queue<pair<int64, int>,
                       vector<pair<int64, int> >,
                       greater<pair<int64, int> > > queue;
        distance[source] = 0;
        queue.push({0, source});

        while (!queue.empty()) {
            const int64 currentDistance = queue.top().first;
            const int vertex = queue.top().second;
            queue.pop();
            if (currentDistance != distance[vertex])
                continue;
            for (int index = 0; index < static_cast<int>(graph[vertex].size());
                 ++index) {
                const Edge& edge = graph[vertex][index];
                if (edge.capacity == 0)
                    continue;
                const int64 nextDistance =
                    currentDistance + edge.cost + potential[vertex] -
                    potential[edge.to];
                if (nextDistance < distance[edge.to]) {
                    distance[edge.to] = nextDistance;
                    parentVertex[edge.to] = vertex;
                    parentEdge[edge.to] = index;
                    queue.push({nextDistance, edge.to});
                }
            }
        }

        if (distance[sink] == INF)
            break;
        for (int vertex = 0; vertex <= sink; ++vertex)
            if (distance[vertex] != INF)
                potential[vertex] += distance[vertex];

        answer += potential[sink];
        int vertex = sink;
        while (vertex != source) {
            Edge& edge = graph[parentVertex[vertex]][parentEdge[vertex]];
            --edge.capacity;
            ++graph[vertex][edge.reverse].capacity;
            vertex = parentVertex[vertex];
        }
        ++flow;
    }

    cout << (flow == side ? answer : -1) << '\n';
    return 0;
}
