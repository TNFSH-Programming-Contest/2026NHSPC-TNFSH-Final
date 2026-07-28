#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

using int64 = long long;

class DSU {
public:
    explicit DSU(int n)
        : parent(n + 1), size(n + 1, 1) {
        iota(parent.begin(), parent.end(), 0);
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

        if (a == b)
            return false;

        if (size[a] < size[b])
            swap(a, b);

        parent[b] = a;
        size[a] += size[b];
        return true;
    }

private:
    vector<int> parent;
    vector<int> size;
};

struct Edge {
    int from;
    int to;
    int64 cost;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    int64 r;
    cin >> n >> m >> r;

    vector<Edge> edges(m);
    int64 total_cost = 0;

    for (Edge& edge : edges) {
        cin >> edge.from >> edge.to >> edge.cost;
        total_cost += edge.cost;
    }

    int64 maximum_saved_cost = 0;
    const uint64_t subset_count = uint64_t{1} << m;

    for (uint64_t mask = 0; mask < subset_count; ++mask) {
        DSU dsu(n);

        int64 saved_cost = 0;
        bool is_forest = true;

        for (int i = 0; i < m; ++i) {
            if (((mask >> i) & 1) == 0)
                continue;

            /*
             * 選入 mask 的 edge 表示不安裝計數器。
             *
             * 若 merge 失敗，代表加入這條 edge 後形成無向環。
             * Self-loop 與第二條平行 edge 也會在此被判定失敗。
             */
            if (!dsu.merge(edges[i].from, edges[i].to)) {
                is_forest = false;
                break;
            }

            saved_cost += edges[i].cost;
        }

        if (is_forest)
            maximum_saved_cost =
                max(maximum_saved_cost, saved_cost);
    }

    cout << total_cost - maximum_saved_cost << '\n';

    return 0;
}