#include <algorithm>
#include <climits>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

using int64 = long long;

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
    vector<vector<pair<int, int>>> graph(n + 1);
    int64 self_loop_cost = 0;

    for (int i = 0; i < m; ++i) {
        cin >> edges[i].from
            >> edges[i].to
            >> edges[i].cost;

        int u = edges[i].from;
        int v = edges[i].to;

        // A self-loop is a cycle by itself, so its counter is mandatory.
        if (u == v) {
            self_loop_cost += edges[i].cost;
            continue;
        }

        graph[u].push_back({v, i});
        graph[v].push_back({u, i});
    }

    vector<int> dfn(n + 1, 0);
    vector<int> low(n + 1, 0);

    vector<int> parent(n + 1, 0);
    vector<int> parent_edge(n + 1, -1);

    // 每個頂點下一條尚未處理的 adjacency。
    vector<int> next_index(n + 1, 0);

    // 模擬遞迴 DFS 的頂點 stack。
    vector<int> vertex_stack;

    // 尚未被分配給任何點雙連通分量的 edges。
    vector<int> edge_stack;

    int timer = 0;
    int64 answer = self_loop_cost;

    auto consume_component = [&](int stop_edge) {
        int edge_count = 0;
        int64 minimum_cost = LLONG_MAX;

        while (true) {
            int edge_id = edge_stack.back();
            edge_stack.pop_back();

            ++edge_count;
            minimum_cost =
                min(minimum_cost, edges[edge_id].cost);

            if (edge_id == stop_edge)
                break;
        }

        /*
         * 單邊點雙連通分量不用安裝計數器。
         *
         * 其餘分量依子任務保證，每個頂點的
         * 分量內度數皆為 2，因此只需選擇
         * 分量中成本最小的 edge。
         */
        if (edge_count > 1)
            answer += minimum_cost;
    };

    for (int root = 1; root <= n; ++root) {
        if (dfn[root] != 0)
            continue;

        dfn[root] = low[root] = ++timer;
        parent[root] = 0;
        parent_edge[root] = -1;

        vertex_stack.push_back(root);

        while (!vertex_stack.empty()) {
            int u = vertex_stack.back();

            if (next_index[u] <
                static_cast<int>(graph[u].size())) {
                auto [v, edge_id] =
                    graph[u][next_index[u]++];

                if (edge_id == parent_edge[u])
                    continue;

                if (dfn[v] == 0) {
                    // DFS tree edge。
                    parent[v] = u;
                    parent_edge[v] = edge_id;

                    dfn[v] = low[v] = ++timer;

                    edge_stack.push_back(edge_id);
                    vertex_stack.push_back(v);
                } else if (dfn[v] < dfn[u]) {
                    /*
                     * 從 u 指向祖先 v 的 back edge。
                     * dfn[v] < dfn[u] 可確保每條
                     * 無向 edge 只被加入一次。
                     */
                    low[u] = min(low[u], dfn[v]);
                    edge_stack.push_back(edge_id);
                }
            } else {
                // u 的所有 adjacency 都處理完畢。
                vertex_stack.pop_back();

                if (parent_edge[u] == -1)
                    continue;

                int p = parent[u];

                low[p] = min(low[p], low[u]);

                /*
                 * u 的 subtree 無法透過 back edge
                 * 到達 p 的祖先，形成一個完整的
                 * 點雙連通分量。
                 */
                if (low[u] >= dfn[p])
                    consume_component(parent_edge[u]);
            }
        }
    }

    cout << answer << '\n';

    return 0;
}
