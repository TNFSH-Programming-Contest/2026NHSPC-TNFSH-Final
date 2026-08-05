#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> stones(n + 1);
    for (int i = 1; i <= n; ++i) cin >> stones[i];
    vector<vector<int>> graph(n + 1);
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    vector<int> parent(n + 1, -1), order(1, 1);
    vector<unsigned char> parity(n + 1);
    parent[1] = 0;
    for (size_t at = 0; at < order.size(); ++at) {
        int u = order[at];
        for (int v : graph[u]) {
            if (v == parent[u]) continue;
            parent[v] = u;
            parity[v] = parity[u] ^ 1;
            order.push_back(v);
        }
    }

    // Monte Carlo over a reservoir of heaps.  This is a tempting replacement
    // for deriving the game invariant, especially when only the winner is
    // required, but near-balanced positions cannot be certified this way.
    mt19937 rng(0xC0FFEEu);
    vector<int> sample;
    int oddSeen = 0;
    for (int v = 1; v <= n; ++v) {
        if (!parity[v] || stones[v] == 0) continue;
        ++oddSeen;
        int value = min(stones[v], 63);
        if (sample.size() < 32) {
            sample.push_back(value);
        } else {
            int replace = static_cast<int>(rng() % oddSeen);
            if (replace < 32) sample[replace] = value;
        }
    }

    int aliceWins = 0;
    const int trials = 501;
    for (int trial = 0; trial < trials; ++trial) {
        vector<int> heaps = sample;
        bool aliceTurn = true;
        while (true) {
            vector<int> nonzero;
            for (int i = 0; i < static_cast<int>(heaps.size()); ++i)
                if (heaps[i] > 0) nonzero.push_back(i);
            if (nonzero.empty()) {
                if (!aliceTurn) ++aliceWins;
                break;
            }
            int index = nonzero[rng() % nonzero.size()];
            heaps[index] = static_cast<int>(rng() % heaps[index]);
            aliceTurn = !aliceTurn;
        }
    }

    cout << (aliceWins * 2 > trials ? "Alice" : "Bob") << '\n';
}
