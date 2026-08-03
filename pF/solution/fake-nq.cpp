#include <bits/stdc++.h>
using namespace std;

struct History {
    int type = 0;
    int oldLabel = 0;
    int newLabel = 0;
    int vertex = 0;
    int left = 0;
    int right = 0;
    long long delta = 0;
    vector<int> changed;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    vector<int> component(n + 1);
    iota(component.begin(), component.end(), 0);
    vector<long long> value(n + 1, 0);
    vector<History> history;
    history.reserve(q);

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int x, y;
            cin >> x >> y;
            int source = component[x];
            int target = component[y];
            if (source == target) continue;

            int sourceSize = 0, targetSize = 0;
            for (int i = 1; i <= n; ++i) {
                sourceSize += component[i] == source;
                targetSize += component[i] == target;
            }
            if (sourceSize > targetSize) swap(source, target);

            History entry;
            entry.type = 1;
            entry.oldLabel = source;
            entry.newLabel = target;
            for (int i = 1; i <= n; ++i) {
                if (component[i] == source) {
                    component[i] = target;
                    entry.changed.push_back(i);
                }
            }
            history.push_back(move(entry));
        } else if (type == 2) {
            int vertex, target;
            cin >> vertex >> target;
            if (component[vertex] == component[target]) continue;
            History entry;
            entry.type = 2;
            entry.vertex = vertex;
            entry.oldLabel = component[vertex];
            history.push_back(move(entry));
            component[vertex] = component[target];
        } else if (type == 3) {
            int left, right;
            long long delta;
            cin >> left >> right >> delta;
            bool effective = false;
            if (delta != 0) {
                for (int i = 1; i <= n; ++i) {
                    if (left <= a[i] && a[i] <= right) effective = true;
                }
            }
            if (!effective) continue;
            history.push_back({3, 0, 0, 0, left, right, delta, {}});
            for (int i = 1; i <= n; ++i) {
                if (left <= a[i] && a[i] <= right) value[i] += delta;
            }
        } else if (type == 4) {
            int vertex;
            cin >> vertex;
            long long answer = 0;
            for (int i = 1; i <= n; ++i) {
                if (component[i] == component[vertex]) answer += value[i];
            }
            cout << answer << '\n';
        } else {
            History entry = move(history.back());
            history.pop_back();
            if (entry.type == 1) {
                for (int vertex : entry.changed) component[vertex] = entry.oldLabel;
            } else if (entry.type == 2) {
                component[entry.vertex] = entry.oldLabel;
            } else {
                for (int i = 1; i <= n; ++i) {
                    if (entry.left <= a[i] && a[i] <= entry.right) {
                        value[i] -= entry.delta;
                    }
                }
            }
        }
    }
}
