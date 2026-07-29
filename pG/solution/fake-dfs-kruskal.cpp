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

#include <queue>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Input in = readInput();
    std::sort(in.edges.begin(), in.edges.end(),
              [](const Edge& a, const Edge& b) { return a.cost > b.cost; });
    std::vector<std::vector<int>> forest(in.n + 1);
    int64 saved = 0;
    long long operations = 0;

    // Correct Kruskal logic, but connectivity is recomputed in O(N+M).
    for (const Edge& edge : in.edges) {
        std::vector<char> seen(in.n + 1, false);
        std::queue<int> que;
        seen[edge.from] = true;
        que.push(edge.from);
        while (!que.empty() && !seen[edge.to]) {
            int u = que.front();
            que.pop();
            if (++operations > 1000000) {
                // This is where the O(NM) connectivity checks become infeasible.
                std::cout << in.total << '\n';
                return 0;
            }
            for (int v : forest[u]) {
                ++operations;
                if (!seen[v]) {
                    seen[v] = true;
                    que.push(v);
                }
            }
        }
        if (!seen[edge.to]) {
            forest[edge.from].push_back(edge.to);
            forest[edge.to].push_back(edge.from);
            saved += edge.cost;
        }
    }
    std::cout << in.total - saved << '\n';
}
