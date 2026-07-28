#include <bits/stdc++.h>
using loli = long long;
const loli INF = (1LL << 62);

#define nitrogen std::ios::sync_with_stdio(false); std::cin.tie(nullptr);

int main() {
    nitrogen;

    int N, H, K;
    std::cin >> N >> H >> K;

    std::vector<int> U(H + 1);
    std::vector<loli> w(H + 1);
    for (int h = 1; h <= H; ++h) std::cin >> U[h];
    for (int h = 1; h <= H; ++h) std::cin >> w[h];

    std::vector<int> x(N + 1), v(N + 1);
    for (int i = 1; i <= N; ++i) {
        std::cin >> x[i] >> v[i];
    }

    // cost[j][r] = conflict cost of using commits (j, r] as one squash group.
    std::vector<std::vector<loli>> cost(N + 1, std::vector<loli>(N + 1, 0));

    std::vector<int> first(H + 1, 0);
    std::vector<int> current(H + 1, 0);
    std::vector<loli> bucket(N + 1, 0);

    for (int r = 1; r <= N; ++r) {
        int h = x[r];
        int old_value = current[h];
        int new_value = v[r];

        if (first[h] == 0) {
            first[h] = r;
            if (new_value != U[h]) {
                bucket[r] += w[h];
            }
        } else {
            bool old_bad = (old_value != U[h]);
            bool new_bad = (new_value != U[h]);

            if (old_bad != new_bad) {
                if (new_bad) {
                    bucket[first[h]] += w[h];
                } else {
                    bucket[first[h]] -= w[h];
                }
            }
        }

        current[h] = new_value;

        loli suffix_sum = 0;
        for (int j = r - 1; j >= 0; --j) {
            suffix_sum += bucket[j + 1];
            cost[j][r] = suffix_sum;
        }
    }

    std::vector<loli> prev(N + 1, INF), cur(N + 1, INF);
    prev[0] = 0;

    for (int groups = 1; groups <= K; ++groups) {
        fill(cur.begin(), cur.end(), INF);

        for (int r = groups; r <= N; ++r) {
            for (int j = groups - 1; j < r; ++j) {
                if (prev[j] == INF) continue;
                cur[r] = std::min(cur[r], prev[j] + cost[j][r]);
            }
        }

        prev.swap(cur);
    }

    std::cout << prev[N] << '\n';
    return 0;
}
