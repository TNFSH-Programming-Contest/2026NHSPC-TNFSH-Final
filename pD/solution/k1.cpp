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

    std::vector<int> final_state(H + 1, 0);
    std::vector<bool> modified(H + 1, false);

    for (int i = 1; i <= N; ++i) {
        int h, value;
        std::cin >> h >> value;
        modified[h] = true;
        final_state[h] = value;
    }

    loli answer = 0;
    for (int h = 1; h <= H; ++h) {
        if (modified[h] && final_state[h] != U[h]) {
            answer += w[h];
        }
    }

    std::cout << answer << '\n';
    return 0;
}
