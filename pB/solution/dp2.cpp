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

    const int64 required = 2 * m;
    int64 answer = 0;

    // Enumerate the two surviving blocks on the sides of a vertex cut.
    // The capacity of that cut is the sum strictly between them.
    for (int left = 0; left + 1 < n; ++left) {
        int64 cutCapacity = 0;
        for (int right = left + 1; right < n; ++right) {
            if (right > left + 1) {
                cutCapacity += capacity[right - 1];
            }
            if (cutCapacity >= required) {
                break;
            }
            answer = max(answer, x[right] - x[left]);
        }
    }

    cout << answer << '\n';
    return 0;
}
