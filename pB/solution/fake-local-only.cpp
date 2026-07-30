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
    for (int i = 1; i + 1 < n; ++i) cin >> capacity[i];

    const int64 required = 2 * m;
    int64 answer = 0;
    for (int i = 0; i + 1 < n; ++i) {
        answer = max(answer, x[i + 1] - x[i]);
    }

    // Wrong: checks only cuts consisting of one removed block.  Several
    // consecutive capacities may each be small and still sum to less than 2m.
    for (int i = 1; i + 1 < n; ++i) {
        if (capacity[i] < required) {
            answer = max(answer, x[i + 1] - x[i - 1]);
        }
    }

    cout << answer << '\n';
}
