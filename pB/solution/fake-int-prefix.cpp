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
    vector<int> capacity(n, 0);
    for (int64& value : x) cin >> value;
    for (int i = 1; i + 1 < n; ++i) cin >> capacity[i];

    // Wrong: 32-bit prefix sums overflow when many a_i are near 10^9.
    vector<int> prefix(n, 0);
    for (int right = 2; right < n; ++right) {
        prefix[right] = prefix[right - 1] + capacity[right - 1];
    }

    const int required = static_cast<int>(2 * m);
    int64 answer = 0;
    for (int left = 0; left + 1 < n; ++left) {
        const int limit = prefix[left + 1] + required;
        const auto begin = prefix.begin() + left + 1;
        int right = static_cast<int>(
            lower_bound(begin, prefix.end(), limit) - prefix.begin()) - 1;
        right = max(right, left + 1);
        answer = max(answer, x[right] - x[left]);
    }

    cout << answer << '\n';
}
