#include <bits/stdc++.h>
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

    loli answer = 0;

    for (int i = 1; i <= N; ++i) {
        int h, value;
        std::cin >> h >> value;

        // In this subtask every hunk appears at most once, so its only
        // feature-side value is also the value at the end of whichever
        // squash group first contains it.
        if (value != U[h]) {
            answer += w[h];
        }
    }

    std::cout << answer << '\n';
    return 0;
}
