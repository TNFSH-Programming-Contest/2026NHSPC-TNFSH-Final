#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int64 m;
    cin >> n >> m;
    vector<int64> x(n), capacity(n, 0);
    for (int64& value : x) cin >> value;
    for (int i = 1; i + 1 < n; ++i) {
        cin >> capacity[i];
        capacity[i] /= 2;
    }

    // Wrong model: forces every chick to use exactly the same intermediate
    // blocks on its outward and return routes, consuming capacity in pairs.
    const int64 required = m;
    int right = 1;
    int64 sum = 0;
    int64 answer = 0;
    for (int left = 0; left + 1 < n; ++left) {
        if (right < left + 1) {
            right = left + 1;
            sum = 0;
        }
        while (right + 1 < n && sum + capacity[right] < required) {
            sum += capacity[right++];
        }
        answer = max(answer, x[right] - x[left]);
        if (right > left + 1) sum -= capacity[left + 1];
    }

    cout << answer << '\n';
}
