#include <bits/stdc++.h>
using namespace std;

using loli = long long;

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
    std::vector<std::vector<int>> state(N + 1, std::vector<int>(H + 1, 0));

    for (int i = 1; i <= N; ++i) {
        std::cin >> x[i] >> v[i];
        state[i] = state[i - 1];
        state[i][x[i]] = v[i];
    }

    loli answer = std::numeric_limits<loli>::max();
    std::vector<int> cuts;
    cuts.reserve(K + 1);
    cuts.push_back(0);

    auto evaluate = [&]() {
        std::vector<int> current = U;
        loli cost = 0;

        for (int g = 1; g < (int)cuts.size(); ++g) {
            int l = cuts[g - 1];
            int r = cuts[g];

            for (int h = 1; h <= H; ++h) {
                int old_value = state[l][h];
                int new_value = state[r][h];

                if (old_value == new_value) continue;

                if (current[h] == old_value) {
                    current[h] = new_value;
                } else if (current[h] == new_value) {
                    // The change already exists.
                } else {
                    cost += w[h];
                    current[h] = new_value;
                }
            }
        }

        answer = min(answer, cost);
    };

    function<void(int, int)> dfs = [&](int next_position, int groups_left) {
        if (groups_left == 1) {
            cuts.push_back(N);
            evaluate();
            cuts.pop_back();
            return;
        }

        // Choose the next endpoint. Leave at least groups_left - 1 commits
        // for the remaining non-empty groups.
        int max_endpoint = N - (groups_left - 1);
        for (int endpoint = next_position; endpoint <= max_endpoint; ++endpoint) {
            cuts.push_back(endpoint);
            dfs(endpoint + 1, groups_left - 1);
            cuts.pop_back();
        }
    };

    dfs(1, K);
    std::cout << answer << '\n';
    return 0;
}
