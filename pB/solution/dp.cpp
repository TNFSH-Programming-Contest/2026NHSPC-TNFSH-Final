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

    // Here m = 1 and a_i = 1.  The outward and return routes must therefore
    // be internally vertex-disjoint.  Among every three consecutive blocks,
    // the chick must be able to jump over the middle one.
    int64 answer = 0;
    for (int i = 0; i + 1 < n; ++i) {
        answer = max(answer, x[i + 1] - x[i]);
    }
    for (int i = 0; i + 2 < n; ++i) {
        answer = max(answer, x[i + 2] - x[i]);
    }

    cout << answer << '\n';
    return 0;
}
