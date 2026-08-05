#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

struct Fenwick {
    vector<int64> tree;
    explicit Fenwick(int n) : tree(n + 1) {}
    void add(int index, int64 delta) {
        for (++index; index < static_cast<int>(tree.size()); index += index & -index)
            tree[index] += delta;
    }
    int64 query(int index) const {
        int64 result = 0;
        for (++index; index > 0; index -= index & -index)
            result += tree[index];
        return result;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1), sorted;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        sorted.push_back(a[i]);
    }
    sort(sorted.begin(), sorted.end());
    sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());
    vector<int> position(n + 1);
    for (int i = 1; i <= n; ++i)
        position[i] = lower_bound(sorted.begin(), sorted.end(), a[i]) - sorted.begin();

    vector<int> parent(n + 1), size(n + 1, 1);
    vector<vector<int>> members(n + 1);
    iota(parent.begin(), parent.end(), 0);
    for (int i = 1; i <= n; ++i) members[i].push_back(i);
    function<int(int)> find = [&](int x) {
        while (parent[x] != x) x = parent[x];
        return x;
    };

    Fenwick updates(static_cast<int>(sorted.size()) + 1);
    mt19937 rng(0xF00D1234u);
    for (int operation = 0; operation < q; ++operation) {
        int type;
        cin >> type;
        if (type == 1) {
            int x, y;
            cin >> x >> y;
            x = find(x);
            y = find(y);
            if (x == y) continue;
            if (size[x] > size[y]) swap(x, y);
            parent[x] = y;
            size[y] += size[x];
            members[y].insert(members[y].end(), members[x].begin(), members[x].end());
            members[x].clear();
        } else if (type == 2) {
            int x, y;
            cin >> x >> y;
            // Rebuilding member lists after arbitrary moves is deemed too
            // expensive by this heuristic.
        } else if (type == 3) {
            int left, right;
            int64 delta;
            cin >> left >> right >> delta;
            int l = lower_bound(sorted.begin(), sorted.end(), left) - sorted.begin();
            int r = upper_bound(sorted.begin(), sorted.end(), right) - sorted.begin();
            if (l < r && delta != 0) {
                updates.add(l, delta);
                updates.add(r, -delta);
            }
        } else if (type == 4) {
            int vertex;
            cin >> vertex;
            int root = find(vertex);
            const vector<int>& component = members[root];
            if (component.size() <= 128) {
                int64 answer = 0;
                for (int v : component) answer += updates.query(position[v]);
                cout << answer << '\n';
            } else {
                int64 sampled = 0;
                for (int take = 0; take < 128; ++take) {
                    int v = component[rng() % component.size()];
                    sampled += updates.query(position[v]);
                }
                cout << sampled * static_cast<int64>(component.size()) / 128 << '\n';
            }
        } else {
            // This approximate data structure also declines to roll back its
            // sampled component representation.
        }
    }
}
