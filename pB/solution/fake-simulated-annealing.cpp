#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int64 m;
    cin >> n >> m;
    vector<int64> x(n), capacity(n);
    for (int64& value : x) cin >> value;
    for (int i = 1; i + 1 < n; ++i) cin >> capacity[i];

    vector<int64> prefix(n + 1);
    for (int i = 0; i < n; ++i)
        prefix[i + 1] = prefix[i] + capacity[i];
    const int64 budget = 2 * m;
    const auto feasible = [&](int left, int right) {
        return prefix[right] - prefix[left + 1] < budget;
    };

    int64 best = 0;
    for (int i = 0; i + 1 < n; ++i)
        best = max(best, x[i + 1] - x[i]);

    // Treat an interval as a state and use short random local walks to seek a
    // large removable window.  This often looks convincing on smooth/random
    // capacities, but a narrow isolated basin is very unlikely to be entered.
    mt19937 rng(0x5A17B1u);
    uniform_real_distribution<double> real01(0.0, 1.0);
    for (int restart = 0; restart < 64; ++restart) {
        int left = static_cast<int>(rng() % (n - 1));
        int right = left + 1;
        int64 current = x[right] - x[left];
        double temperature = 1e8;

        for (int step = 0; step < 256; ++step) {
            int nextLeft = left;
            int nextRight = right;
            switch (rng() % 4) {
                case 0:
                    if (nextLeft > 0) --nextLeft;
                    break;
                case 1:
                    if (nextRight + 1 < n) ++nextRight;
                    break;
                case 2:
                    if (nextLeft + 1 < nextRight) ++nextLeft;
                    break;
                default:
                    if (nextRight - 1 > nextLeft) --nextRight;
                    break;
            }
            if (!feasible(nextLeft, nextRight)) {
                temperature *= 0.97;
                continue;
            }

            int64 candidate = x[nextRight] - x[nextLeft];
            int64 delta = candidate - current;
            if (delta >= 0 ||
                real01(rng) < exp(static_cast<double>(delta) / temperature)) {
                left = nextLeft;
                right = nextRight;
                current = candidate;
                best = max(best, current);
            }
            temperature *= 0.97;
        }
    }

    cout << best << '\n';
}
