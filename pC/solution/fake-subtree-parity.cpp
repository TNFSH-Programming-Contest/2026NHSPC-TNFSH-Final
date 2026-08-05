#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> stones(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> stones[i];

    vector<vector<int> > graph(n + 1);
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    vector<int> parent(n + 1, 0);
    vector<int> order(1, 1);
    parent[1] = -1;
    for (size_t index = 0; index < order.size(); ++index) {
        const int vertex = order[index];
        for (int next : graph[vertex]) {
            if (next == parent[vertex])
                continue;
            parent[next] = vertex;
            order.push_back(next);
        }
    }

    vector<int> subtreeSize(n + 1, 1);
    for (int index = n - 1; index > 0; --index) {
        const int vertex = order[index];
        subtreeSize[parent[vertex]] += subtreeSize[vertex];
    }

    long long nimSum = 0;
    for (int vertex = 1; vertex <= n; ++vertex) {
        // Wrong invariant: subtree-size parity is not the token's distance
        // parity from the root.
        if (subtreeSize[vertex] & 1)
            nimSum ^= stones[vertex];
    }
    cout << (nimSum == 0 ? "Bob" : "Alice") << '\n';
    return 0;
}
