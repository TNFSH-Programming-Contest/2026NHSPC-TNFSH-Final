#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }
    vector<vector<int> > graph(n + 1);
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    vector<int> parent(n + 1, -1);
    vector<unsigned char> parity(n + 1, 0);
    vector<int> order(1, 1);
    parent[1] = 0;
    long long value = 0;
    for (size_t i = 0; i < order.size(); ++i) {
        int u = order[i];
        if (!parity[u]) {
            value ^= a[u];
        }
        for (int v : graph[u]) {
            if (v != parent[u]) {
                parent[v] = u;
                parity[v] = parity[u] ^ 1;
                order.push_back(v);
            }
        }
    }
    cout << (value ? "Alice" : "Bob") << '\n';
}
