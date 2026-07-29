#include <bits/stdc++.h>

using int64 = long long;

struct Edge {
    int from;
    int to;
    int64 cost;
};

struct Input {
    int n;
    int m;
    int64 r;
    int64 total = 0;
    std::vector<Edge> edges;
};

Input readInput() {
    Input in;
    std::cin >> in.n >> in.m >> in.r;
    in.edges.resize(in.m);
    for (Edge& edge : in.edges) {
        std::cin >> edge.from >> edge.to >> edge.cost;
        in.total += edge.cost;
    }
    return in;
}

class DSU {
public:
    explicit DSU(int n) : parent(n + 1), size(n + 1, 1) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

    bool merge(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) std::swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }

private:
    std::vector<int> parent;
    std::vector<int> size;
};

#include <climits>
#include <queue>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Input in = readInput();
    DSU dsu(in.n);
    std::vector<std::vector<std::pair<int, int64>>> tree(in.n + 1);
    std::vector<Edge> nonTree;
    for (const Edge& edge : in.edges) {
        if (dsu.merge(edge.from, edge.to)) {
            tree[edge.from].push_back({edge.to, edge.cost});
            tree[edge.to].push_back({edge.from, edge.cost});
        } else {
            nonTree.push_back(edge);
        }
    }

    const int LOG = 18;
    std::vector<int> depth(in.n + 1, -1);
    std::vector<std::vector<int>> up(LOG, std::vector<int>(in.n + 1));
    std::vector<std::vector<int64>> minimum(
        LOG, std::vector<int64>(in.n + 1, LLONG_MAX));
    std::queue<int> que;
    depth[1] = 0;
    up[0][1] = 1;
    que.push(1);
    while (!que.empty()) {
        int u = que.front();
        que.pop();
        for (const auto& adjacency : tree[u]) {
            int v = adjacency.first;
            if (depth[v] != -1) continue;
            depth[v] = depth[u] + 1;
            up[0][v] = u;
            minimum[0][v] = adjacency.second;
            que.push(v);
        }
    }
    for (int k = 1; k < LOG; ++k) {
        for (int v = 1; v <= in.n; ++v) {
            up[k][v] = up[k - 1][up[k - 1][v]];
            minimum[k][v] = std::min(
                minimum[k - 1][v], minimum[k - 1][up[k - 1][v]]);
        }
    }

    auto pathMinimum = [&](int u, int v) {
        int64 result = LLONG_MAX;
        if (depth[u] < depth[v]) std::swap(u, v);
        int difference = depth[u] - depth[v];
        for (int k = LOG - 1; k >= 0; --k) {
            if ((difference >> k) & 1) {
                result = std::min(result, minimum[k][u]);
                u = up[k][u];
            }
        }
        if (u == v) return result;
        for (int k = LOG - 1; k >= 0; --k) {
            if (up[k][u] != up[k][v]) {
                result = std::min(result, minimum[k][u]);
                result = std::min(result, minimum[k][v]);
                u = up[k][u];
                v = up[k][v];
            }
        }
        result = std::min(result, minimum[0][u]);
        result = std::min(result, minimum[0][v]);
        return result;
    };

    // Mutation: treats fundamental-cycle minima as independent contributions.
    int64 answer = 0;
    for (const Edge& edge : nonTree)
        answer += std::min(edge.cost, pathMinimum(edge.from, edge.to));
    std::cout << answer << '\n';
}
