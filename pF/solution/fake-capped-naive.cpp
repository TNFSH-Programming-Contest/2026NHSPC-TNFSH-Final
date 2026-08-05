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
    vector<int> key(n + 1);
    for (int i = 1; i <= n; ++i) cin >> key[i];

    vector<int> component(n + 1);
    iota(component.begin(), component.end(), 0);
    vector<long long> value(n + 1);
    vector<Snapshot> history;
    constexpr int MAGIC = 128;

    for (int operation = 0; operation < q; ++operation) {
        int type;
        cin >> type;
        if (operation >= MAGIC) {
            if (type == 1 || type == 2) {
                int a, b;
                cin >> a >> b;
            } else if (type == 3) {
                int left, right;
                long long delta;
                cin >> left >> right >> delta;
            } else if (type == 4) {
                int vertex;
                cin >> vertex;
                cout << 0 << '\n';
            }
            continue;
        }

        if (type == 1) {
            int x, y;
            cin >> x >> y;
            if (component[x] == component[y]) continue;
            history.push_back({component, value});
            const int from = component[x];
            const int to = component[y];
            for (int i = 1; i <= n; ++i)
                if (component[i] == from) component[i] = to;
        } else if (type == 2) {
            int x, y;
            cin >> x >> y;
            if (component[x] == component[y]) continue;
            history.push_back({component, value});
            component[x] = component[y];
        } else if (type == 3) {
            int left, right;
            long long delta;
            cin >> left >> right >> delta;
            bool effective = delta != 0;
            if (effective) {
                effective = false;
                for (int i = 1; i <= n; ++i)
                    if (left <= key[i] && key[i] <= right) effective = true;
            }
            if (!effective) continue;
            history.push_back({component, value});
            for (int i = 1; i <= n; ++i)
                if (left <= key[i] && key[i] <= right) value[i] += delta;
        } else if (type == 4) {
            int x;
            cin >> x;
            long long answer = 0;
            for (int i = 1; i <= n; ++i)
                if (component[i] == component[x]) answer += value[i];
            cout << answer << '\n';
        } else {
            component = move(history.back().component);
            value = move(history.back().value);
            history.pop_back();
        }
    }
}
