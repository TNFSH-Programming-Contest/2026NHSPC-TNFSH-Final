#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
const int WINDOW_LIMIT = 64;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int64 m;
    cin >> n >> m;
    vector<int64> x(n), capacity(n, 0);
    for (int64& value : x)
        cin >> value;
    for (int i = 1; i + 1 < n; ++i)
        cin >> capacity[i];

    const int64 required = 2 * m;
    int right = 1;
    int64 cutCapacity = 0;
    int64 answer = 0;

    for (int left = 0; left + 1 < n; ++left) {
        if (right < left + 1) {
            right = left + 1;
            cutCapacity = 0;
        }

        // Wrong pruning: assumes no relevant deficient cut spans more than
        // WINDOW_LIMIT gaps.  This turns the correct scan into O(N * 64).
        while (right + 1 < n &&
               right - left < WINDOW_LIMIT &&
               cutCapacity + capacity[right] < required) {
            cutCapacity += capacity[right];
            ++right;
        }

        answer = max(answer, x[right] - x[left]);
        if (right > left + 1)
            cutCapacity -= capacity[left + 1];
    }

    cout << answer << '\n';
    return 0;
}
