#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int64 m;
    cin >> n >> m;

    vector<int64> x(n);
    for (int i = 0; i < n; ++i) {
        cin >> x[i];
    }

    vector<int64> capacity(n, 0);
    for (int i = 1; i + 1 < n; ++i) {
        cin >> capacity[i];
    }

    // This implementation is intentionally the subtask's exponential
    // solution.  Avoid undefined shifts if it is run outside the subtask.
    if (n > 7 || m > 7) {
        cout << 0 << '\n';
        return 0;
    }

    const int64 required = 2 * m;

    const auto feasible = [&](int64 jump) {
        const int masks = 1 << (n - 2);

        for (int mask = 0; mask < masks; ++mask) {
            int64 removedCapacity = 0;
            for (int i = 1; i + 1 < n; ++i) {
                if (mask & (1 << (i - 1))) {
                    removedCapacity += capacity[i];
                }
            }
            if (removedCapacity >= required) {
                continue;
            }

            int last = 0;
            bool disconnected = false;
            for (int i = 1; i < n; ++i) {
                if (i + 1 < n && (mask & (1 << (i - 1)))) {
                    continue;
                }
                if (x[i] - x[last] > jump) {
                    disconnected = true;
                    break;
                }
                last = i;
            }

            if (disconnected) {
                return false;
            }
        }
        return true;
    };

    int64 low = 0;
    int64 high = x.back() - x.front();
    while (low < high) {
        const int64 middle = (low + high) / 2;
        if (feasible(middle)) {
            high = middle;
        } else {
            low = middle + 1;
        }
    }

    cout << low << '\n';
    return 0;
}
