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
    int64 answer = 0;

    for (int target = 1; target < n; ++target) {
        int64 space = target + 1 == n
            ? total : min(total, capacity[target]);
        const int64 placed = space;

        // Wrong: always moves the closest (rightmost) frogs first.  This
        // strands older frogs on the left and makes a later jump unnecessarily
        // long.
        while (space > 0) {
            Group& source = groups.back();
            const int64 moved = min(space, source.frogs);
            answer = max(answer, x[target] - source.position);
            source.frogs -= moved;
            space -= moved;
            if (source.frogs == 0) groups.pop_back();
        }

        if (target + 1 < n) groups.push_back({x[target], placed});
    }

    cout << answer << '\n';
}
