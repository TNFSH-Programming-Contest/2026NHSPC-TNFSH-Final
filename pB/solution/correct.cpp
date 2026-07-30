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

    vector<int64> x(n);
    for (int i = 0; i < n; ++i) {
        cin >> x[i];
    }

    vector<int64> capacity(n, 0);
    for (int i = 1; i + 1 < n; ++i) {
        cin >> capacity[i];
    }

    // Reverse every return route.  The original problem is now equivalent to
    // moving 2m virtual frogs from x_1 to x_n, where block i can receive at
    // most a_i frogs.
    const int64 totalFrogs = 2 * m;

    vector<Group> groups;
    groups.reserve(n);
    groups.push_back({x[0], totalFrogs});
    int firstGroup = 0;
    int64 answer = 0;

    for (int target = 1; target < n; ++target) {
        // There are only 2m frogs, so capacity above 2m is equivalent to 2m.
        int64 space = (target + 1 == n)
                          ? totalFrogs
                          : min(totalFrogs, capacity[target]);
        const int64 frogsOnTarget = space;

        // Giving the next block to the leftmost frogs first is optimal:
        // otherwise swapping a farther frog's future destination with this
        // nearer destination cannot increase either jump.
        while (space > 0) {
            Group& source = groups[firstGroup];
            const int64 moved = min(space, source.frogs);

            answer = max(answer, x[target] - source.position);
            source.frogs -= moved;
            space -= moved;

            if (source.frogs == 0) {
                ++firstGroup;
            }
        }

        if (target + 1 < n) {
            groups.push_back({x[target], frogsOnTarget});
        }
    }

    cout << answer << '\n';
    return 0;
}
