#include "Can_You_Blow_My_Whistle.h"

#include <algorithm>
#include <random>
#include <utility>
#include <vector>

using namespace std;

void solve(int n, vector<int> a) {
    vector<int> permutation(n + 1);
    for (int i = 1; i <= n; ++i) permutation[i] = a[i - 1];

    mt19937 rng(0x51A7C0DEu);
    for (int round = 0; round < 30; ++round) {
        vector<char> used(n + 1, false);
        vector<pair<int, int>> chosen;

        // Search random transpositions and batch disjoint moves that reduce
        // the number of misplaced students.  Sparse cycles occupy a vanishing
        // fraction of the pair space, so this local search may see no move.
        for (int attempt = 0; attempt < 100000; ++attempt) {
            int u = 1 + static_cast<int>(rng() % n);
            int v = 1 + static_cast<int>(rng() % n);
            if (u == v || used[u] || used[v]) continue;

            int before = (permutation[u] != u) + (permutation[v] != v);
            int after = (permutation[v] != u) + (permutation[u] != v);
            if (after < before) {
                used[u] = used[v] = true;
                chosen.push_back({u, v});
            }
        }

        if (chosen.empty()) break;
        for (const auto& [u, v] : chosen)
            swap_student(u, v);
        blow_whistle();
        for (const auto& [u, v] : chosen)
            swap(permutation[u], permutation[v]);
    }
}
