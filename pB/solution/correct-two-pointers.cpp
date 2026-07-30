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

    // The 2m journeys can be completed exactly when every vertex cut has
    // capacity at least 2m.  Removing all blocks strictly between left and
    // right is a cut unless that gap can be jumped directly.
    const int64 required = 2 * m;

    int right = 1;
    int64 cutCapacity = 0;
    int64 answer = 0;

    for (int left = 0; left + 1 < n; ++left) {
        if (right < left + 1) {
            right = left + 1;
            cutCapacity = 0;
        }

        // Keep the farthest right endpoint whose strict interior has total
        // capacity below 2m.  This whole gap must be directly jumpable.
        while (right + 1 < n &&
               cutCapacity + capacity[right] < required) {
            cutCapacity += capacity[right];
            ++right;
        }

        answer = max(answer, x[right] - x[left]);

        if (right > left + 1) {
            cutCapacity -= capacity[left + 1];
        }
    }

    cout << answer << '\n';
    return 0;
}
