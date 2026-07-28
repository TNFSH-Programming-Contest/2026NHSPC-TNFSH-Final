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
    vector<int> x(n + 1), value(n + 1), finalState(h + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> x[i] >> value[i];
        finalState[x[i]] = value[i];
    }

    vector<char> finalBad(h + 1);
    for (int i = 1; i <= h; ++i)
        finalBad[i] = finalState[i] != 0 && finalState[i] != target[i];

    vector<int> state(h + 1);
    vector<char> seen(h + 1);
    vector<pair<int64, int>> saving;
    int64 currentSaving = 0;
    for (int i = 1; i < n; ++i) {
        int id = x[i];
        bool oldBad = seen[id] && state[id] != target[id];
        state[id] = value[i];
        bool newBad = state[id] != target[id];
        if (!seen[id]) {
            seen[id] = true;
            currentSaving += (finalBad[id] - newBad) * 1LL * weight[id];
        } else {
            currentSaving += (oldBad - newBad) * 1LL * weight[id];
        }
        saving.push_back({currentSaving, i});
    }

    sort(saving.begin(), saving.end(),
         [](const pair<int64, int>& a, const pair<int64, int>& b) {
             if (a.first != b.first) return a.first > b.first;
             return a.second < b.second;
         });
    vector<char> cut(n + 1);
    for (int i = 0; i < k - 1; ++i) cut[saving[i].second] = true;
    cut[n] = true;

    fill(state.begin(), state.end(), 0);
    fill(seen.begin(), seen.end(), false);
    vector<int> newcomers;
    int64 answer = 0;
    for (int i = 1; i <= n; ++i) {
        int id = x[i];
        state[id] = value[i];
        if (!seen[id]) {
            seen[id] = true;
            newcomers.push_back(id);
        }
        if (cut[i]) {
            for (int newcomer : newcomers)
                if (state[newcomer] != target[newcomer])
                    answer += weight[newcomer];
            newcomers.clear();
        }
    }
    cout << answer << '\n';
}
