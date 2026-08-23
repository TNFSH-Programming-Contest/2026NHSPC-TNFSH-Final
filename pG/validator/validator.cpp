#include "testlib.h"

#include <algorithm>
#include <cstdint>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace {

const int MAX_N = 100000;
const int MAX_M = 200000;
const long long MAX_R = 1000000000000000000LL;
const long long MAX_C = 1000000000LL;

struct Edge {
    int from;
    int to;
    long long cost;
};

struct Input {
    int n;
    int m;
    long long r;
    vector<Edge> edges;
};

Input readInput() {
    Input input;

    input.n = inf.readInt(2, MAX_N, "N");
    inf.readSpace();
    input.m = inf.readInt(1, MAX_M, "M");
    inf.readSpace();
    input.r = inf.readLong(2LL * input.m, MAX_R, "R");
    inf.readEoln();

    input.edges.resize(input.m);
    vector<vector<int> > outgoing(input.n + 1);

    for (int i = 0; i < input.m; ++i) {
        Edge& edge = input.edges[i];
        edge.from = inf.readInt(1, input.n, format("u[%d]", i + 1));
        inf.readSpace();
        edge.to = inf.readInt(1, input.n, format("v[%d]", i + 1));
        inf.readSpace();
        edge.cost = inf.readLong(1, MAX_C, format("C[%d]", i + 1));
        inf.readEoln();

        outgoing[edge.from].push_back(edge.to);
    }
    inf.readEof();

    vector<char> reached(input.n + 1, false);
    queue<int> bfs;
    reached[1] = true;
    bfs.push(1);

    while (!bfs.empty()) {
        const int u = bfs.front();
        bfs.pop();

        for (int v : outgoing[u]) {
            if (!reached[v]) {
                reached[v] = true;
                bfs.push(v);
            }
        }
    }

    vector<vector<int> > incoming(input.n + 1);
    for (const Edge& edge : input.edges) {
        incoming[edge.to].push_back(edge.from);
    }

    vector<char> canReachExit(input.n + 1, false);
    canReachExit[input.n] = true;
    bfs.push(input.n);
    while (!bfs.empty()) {
        const int v = bfs.front();
        bfs.pop();

        for (int u : incoming[v]) {
            if (!canReachExit[u]) {
                canReachExit[u] = true;
                bfs.push(u);
            }
        }
    }

    for (int v = 1; v <= input.n; ++v) {
        ensuref(reached[v] && canReachExit[v],
                "BB %d is not on any directed path from entry BB 1 to exit BB %d",
                v, input.n);
    }

    return input;
}

void validateList(const Input& input) {
    ensuref(input.m == input.n - 1,
            "list subtask requires M = N - 1, but N = %d and M = %d",
            input.n, input.m);

    for (int i = 0; i < input.m; ++i) {
        const Edge& edge = input.edges[i];
        ensuref(edge.from == i + 1 && edge.to == i + 2,
                "edge %d must be %d -> %d, but is %d -> %d",
                i + 1, i + 1, i + 2, edge.from, edge.to);
    }
}

void validateCostOne(const Input& input) {
    for (int i = 0; i < input.m; ++i) {
        ensuref(input.edges[i].cost == 1,
                "ce1 subtask requires C[%d] = 1, but C[%d] = %lld",
                i + 1, i + 1, input.edges[i].cost);
    }
}

void validateM20(const Input& input) {
    ensuref(input.m <= 20,
            "m20 subtask requires M <= 20, but M = %d", input.m);
}

void validateUnicyclic(const Input& input) {
    ensuref(input.m == input.n,
            "unicyclic subtask requires M = N, but N = %d and M = %d",
            input.n, input.m);

    set<pair<int, int> > connections;
    for (int i = 0; i < input.m; ++i) {
        const Edge& edge = input.edges[i];
        ensuref(edge.from != edge.to,
                "unicyclic subtask forbids self-loops, but edge %d is %d -> %d",
                i + 1, edge.from, edge.to);

        const pair<int, int> connection =
            minmax(edge.from, edge.to);
        ensuref(connections.insert(connection).second,
                "unicyclic subtask forbids repeated connections; edge %d repeats {%d, %d}",
                i + 1, connection.first, connection.second);
    }
}

void validateCactus(const Input& input) {
    vector<vector<pair<int, int> > > graph(input.n + 1);
    for (int id = 0; id < input.m; ++id) {
        const Edge& edge = input.edges[id];

        // A self-loop is a one-edge block, so it already satisfies the
        // subtask condition and does not participate in Tarjan's DFS.
        if (edge.from == edge.to) {
            continue;
        }

        graph[edge.from].push_back(make_pair(edge.to, id));
        graph[edge.to].push_back(make_pair(edge.from, id));
    }

    vector<int> dfn(input.n + 1, 0);
    vector<int> low(input.n + 1, 0);
    vector<int> parent(input.n + 1, 0);
    vector<int> parentEdge(input.n + 1, -1);
    vector<int> nextIndex(input.n + 1, 0);
    vector<int> vertexStack;
    vector<int> edgeStack;
    vector<int> component;
    vector<int> degree(input.n + 1, 0);
    vector<int> touched;
    int timer = 0;

    const auto consumeComponent = [&](int stopEdge) {
        component.clear();
        while (true) {
            ensuref(!edgeStack.empty(),
                    "internal cactus validation error: empty edge stack");
            const int edgeId = edgeStack.back();
            edgeStack.pop_back();
            component.push_back(edgeId);
            if (edgeId == stopEdge) {
                break;
            }
        }

        if (component.size() == 1) {
            return;
        }

        touched.clear();
        for (int edgeId : component) {
            const Edge& edge = input.edges[edgeId];
            if (degree[edge.from]++ == 0) {
                touched.push_back(edge.from);
            }
            if (degree[edge.to]++ == 0) {
                touched.push_back(edge.to);
            }
        }

        for (int v : touched) {
            ensuref(degree[v] == 2,
                    "a vertex-biconnected component containing edge %d has degree %d at BB %d, not 2",
                    component.front() + 1, degree[v], v);
        }
        for (int v : touched) {
            degree[v] = 0;
        }
    };

    for (int root = 1; root <= input.n; ++root) {
        if (dfn[root] != 0) {
            continue;
        }

        dfn[root] = low[root] = ++timer;
        vertexStack.push_back(root);

        while (!vertexStack.empty()) {
            const int u = vertexStack.back();

            if (nextIndex[u] < static_cast<int>(graph[u].size())) {
                const pair<int, int> adjacency = graph[u][nextIndex[u]++];
                const int v = adjacency.first;
                const int edgeId = adjacency.second;

                if (edgeId == parentEdge[u]) {
                    continue;
                }

                if (dfn[v] == 0) {
                    parent[v] = u;
                    parentEdge[v] = edgeId;
                    dfn[v] = low[v] = ++timer;
                    edgeStack.push_back(edgeId);
                    vertexStack.push_back(v);
                } else if (dfn[v] < dfn[u]) {
                    low[u] = min(low[u], dfn[v]);
                    edgeStack.push_back(edgeId);
                }
            } else {
                vertexStack.pop_back();

                if (parentEdge[u] == -1) {
                    continue;
                }

                const int p = parent[u];
                low[p] = min(low[p], low[u]);
                if (low[u] >= dfn[p]) {
                    consumeComponent(parentEdge[u]);
                }
            }
        }
    }

    ensuref(edgeStack.empty(),
            "internal cactus validation error: unconsumed edges");
}

}  // namespace

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    const string mode = argc >= 2 ? argv[1] : "full";
    ensuref(mode == "full" || mode == "list" || mode == "ce1" ||
                mode == "m20" || mode == "unicyclic" || mode == "cactus",
            "unknown validator mode: %s", mode.c_str());

    const Input input = readInput();

    if (mode == "list") {
        validateList(input);
    } else if (mode == "ce1") {
        validateCostOne(input);
    } else if (mode == "m20") {
        validateM20(input);
    } else if (mode == "unicyclic") {
        validateUnicyclic(input);
    } else if (mode == "cactus") {
        validateCactus(input);
    }

    return 0;
}
