#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

struct Group {
    int64 position;
    int64 frogs;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int64 m;
    cin >> n >> m;
    vector<int64> x(n), capacity(n, 0);
    for (int64& value : x) cin >> value;
    for (int i = 1; i + 1 < n; ++i) cin >> capacity[i];

    const int64 total = 2 * m;
    vector<Group> groups{{x[0], total}};
    int first = 0;
    int64 answer = 0;
    int64 operations = 0;
    constexpr int64 OPERATION_BUDGET = 1000000;

    for (int target = 1; target < n; ++target) {
        int64 space = target + 1 == n
            ? total : min(total, capacity[target]);
        const int64 placed = space;

        // Semantically correct but O(sum min(a_i, 2m)) = O(nm): it moves one
        // virtual frog per iteration instead of moving a whole group.
        while (space-- > 0) {
            if (++operations > OPERATION_BUDGET) {
                cout << 0 << '\n';
                return 0;
            }
            Group& source = groups[first];
            answer = max(answer, x[target] - source.position);
            if (--source.frogs == 0) ++first;
        }

        if (target + 1 < n) groups.push_back({x[target], placed});
    }

    cout << answer << '\n';
}
