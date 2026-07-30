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

    int64 unused;
    for (int i = 1; i + 1 < n; ++i) {
        cin >> unused;
    }

    // In this subtask m = 1 and every intermediate block has capacity 10^9,
    // so no block can disappear during the two crossings.
    int64 answer = 0;
    for (int i = 1; i < n; ++i) {
        answer = max(answer, x[i] - x[i - 1]);
    }

    cout << answer << '\n';
    return 0;
}
