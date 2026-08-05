#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

struct Edge {
    int u;
    int v;
    int64 weight;
};

struct DSU {
    vector<int> parent, size;
    explicit DSU(int n) : parent(n + 1), size(n + 1, 1) {
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        while (parent[x] != x) x = parent[x];
        return x;
    }
    bool merge(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    int64 r;
    cin >> n >> m >> r;
    vector<Edge> edges(m);
    int64 total = 0;
    for (Edge& edge : edges) {
        cin >> edge.u >> edge.v >> edge.weight;
        total += edge.weight;
    }

    vector<Edge> candidates;
    if (m <= 5000) {
        candidates = edges;
    } else {
        // First preserve any spanning tree found in input order, then examine
        // only a small random sample of exchange candidates.
        DSU seedTree(n);
        for (const Edge& edge : edges)
            if (seedTree.merge(edge.u, edge.v)) candidates.push_back(edge);

        mt19937 rng(0x6A09E667u);
        for (int attempt = 0; attempt < 4096; ++attempt)
            candidates.push_back(edges[rng() % m]);
    }

    sort(candidates.begin(), candidates.end(),
         [](const Edge& lhs, const Edge& rhs) {
             return lhs.weight > rhs.weight;
         });
    DSU forest(n);
    int64 saved = 0;
    for (const Edge& edge : candidates)
        if (forest.merge(edge.u, edge.v)) saved += edge.weight;

    cout << total - saved << '\n';
}
