#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, h, k;
    cin >> n >> h >> k;
    vector<int> target(h + 1), weight(h + 1);
    for (int i = 1; i <= h; ++i) cin >> target[i];
    for (int i = 1; i <= h; ++i) cin >> weight[i];

    vector<int> firstValue(h + 1), finalValue(h + 1);
    vector<char> modified(h + 1), canEndAtTarget(h + 1);
    for (int i = 1; i <= n; ++i) {
        int x, value;
        cin >> x >> value;
        if (!modified[x]) {
            modified[x] = true;
            firstValue[x] = value;
        }
        finalValue[x] = value;
        if (value == target[x]) canEndAtTarget[x] = true;
    }

    int64 answer = 0;
    for (int x = 1; x <= h; ++x) {
        if (!modified[x]) continue;
        bool bad;
        if (k == 1)
            bad = finalValue[x] != target[x];
        else if (k == n)
            bad = firstValue[x] != target[x];
        else
            bad = !canEndAtTarget[x];
        if (bad) answer += weight[x];
    }
    cout << answer << '\n';
}
