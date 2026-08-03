#include <bits/stdc++.h>
using namespace std;

struct Snapshot {
    vector<int> component;
    vector<long long> value;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    // This is intentionally the O(NQ)-time, O(NQ)-memory subtask solution.
    if (n > 2000 || q > 2000) return 0;

    vector<int> component(n + 1);
    iota(component.begin(), component.end(), 0);
    vector<long long> value(n + 1, 0);
    vector<Snapshot> history;
    history.reserve(q);

    auto save = [&]() { history.push_back({component, value}); };

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int x, y;
            cin >> x >> y;
            if (component[x] == component[y]) continue;
            save();
            const int from = component[x];
            const int to = component[y];
            for (int i = 1; i <= n; ++i) {
                if (component[i] == from) component[i] = to;
            }
        } else if (type == 2) {
            int x, y;
            cin >> x >> y;
            if (component[x] == component[y]) continue;
            save();
            component[x] = component[y];
        } else if (type == 3) {
            int left, right;
            long long delta;
            cin >> left >> right >> delta;
            bool effective = delta != 0;
            if (effective) {
                effective = false;
                for (int i = 1; i <= n; ++i) {
                    if (left <= a[i] && a[i] <= right) effective = true;
                }
            }
            if (!effective) continue;
            save();
            for (int i = 1; i <= n; ++i) {
                if (left <= a[i] && a[i] <= right) value[i] += delta;
            }
        } else if (type == 4) {
            int x;
            cin >> x;
            long long answer = 0;
            for (int i = 1; i <= n; ++i) {
                if (component[i] == component[x]) answer += value[i];
            }
            cout << answer << '\n';
        } else {
            component = move(history.back().component);
            value = move(history.back().value);
            history.pop_back();
        }
    }
}
