#include <bits/stdc++.h>
using namespace std;

struct Choice { int right, cost; };

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    const int side = n - 1;
    vector<vector<Choice> > graph(side);
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        cin >> from >> to >> cost;
        if (from != n && to != 1)
            graph[from - 1].push_back({to - 2, cost});
    }

    vector<int> match(side, -1), match_cost(side), seen(side);
    int stamp = 0;
    function<bool(int)> augment = [&](int left) {
        for (const Choice& choice : graph[left]) {
            if (seen[choice.right] == stamp) continue;
            seen[choice.right] = stamp;
            if (match[choice.right] == -1 || augment(match[choice.right])) {
                match[choice.right] = left;
                match_cost[choice.right] = choice.cost;
                return true;
            }
        }
        return false;
    };

    for (int left = 0; left < side; ++left) {
        ++stamp;
        if (!augment(left)) {
            cout << -1 << '\n';
            return 0;
        }
    }
    cout << accumulate(match_cost.begin(), match_cost.end(), 0LL) << '\n';
}
