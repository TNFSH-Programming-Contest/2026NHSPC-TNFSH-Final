#include <bits/stdc++.h>
using namespace std;

int maximumMatching(const vector<vector<int> >& graph, int side) {
    vector<int> match(side, -1), seen(side);
    int stamp = 0, answer = 0;
    function<bool(int)> augment = [&](int left) {
        for (int right : graph[left]) {
            if (seen[right] == stamp) continue;
            seen[right] = stamp;
            if (match[right] == -1 || augment(match[right])) {
                match[right] = left;
                return true;
            }
        }
        return false;
    };
    for (int left = 0; left < side; ++left) {
        ++stamp;
        answer += augment(left);
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    const int side = n - 1;
    vector<vector<int> > all(side), zero(side);
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        cin >> from >> to >> cost;
        if (from == n || to == 1) continue;
        all[from - 1].push_back(to - 2);
        if (cost == 0) zero[from - 1].push_back(to - 2);
    }
    if (maximumMatching(all, side) != side) cout << -1 << '\n';
    else cout << side - maximumMatching(zero, side) << '\n';
}
