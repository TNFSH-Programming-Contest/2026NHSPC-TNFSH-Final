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

#include <map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Input in = readInput();
    // Mutation: silently removes loops and merges every parallel connection.
    std::map<std::pair<int, int>, int64> unique;
    for (const Edge& edge : in.edges) {
        if (edge.from == edge.to) continue;
        int u = std::min(edge.from, edge.to);
        int v = std::max(edge.from, edge.to);
        unique[{u, v}] = std::max(unique[{u, v}], edge.cost);
    }

    std::vector<Edge> edges;
    int64 total = 0;
    for (const auto& item : unique) {
        edges.push_back({item.first.first, item.first.second, item.second});
        total += item.second;
    }
    std::sort(edges.begin(), edges.end(),
              [](const Edge& a, const Edge& b) { return a.cost > b.cost; });
    DSU dsu(in.n);
    int64 saved = 0;
    for (const Edge& edge : edges)
        if (dsu.merge(edge.from, edge.to)) saved += edge.cost;
    std::cout << total - saved << '\n';
}
