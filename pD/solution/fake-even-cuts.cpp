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

    vector<pair<int, int>> commits(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> commits[i].first >> commits[i].second;

    vector<int> endpoint(k);
    for (int group = 1; group <= k; ++group)
        endpoint[group - 1] = static_cast<int>(1LL * group * n / k);

    vector<int> state(h + 1), firstGroup(h + 1, -1);
    vector<vector<int>> newcomers(k);
    int group = 0;
    int64 answer = 0;
    for (int i = 1; i <= n; ++i) {
        int x = commits[i].first;
        state[x] = commits[i].second;
        if (firstGroup[x] == -1) {
            firstGroup[x] = group;
            newcomers[group].push_back(x);
        }
        if (i == endpoint[group]) {
            for (int newcomer : newcomers[group])
                if (state[newcomer] != target[newcomer])
                    answer += weight[newcomer];
            ++group;
        }
    }
    cout << answer << '\n';
}
