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
    vector<int> changed(n + 1), value(n + 1);
    vector<vector<pair<int, int>>> history(h + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> changed[i] >> value[i];
        history[changed[i]].push_back({i, value[i]});
    }

    vector<int> cuts(k + 1);
    cuts[0] = 0;
    cuts[k] = n;
    for (int group = 1; group < k; ++group)
        cuts[group] = static_cast<int>(1LL * group * n / k);

    const auto evaluateGeneral = [&](const vector<int>& endpoints) {
        int64 answer = 0;
        for (int id = 1; id <= h; ++id) {
            if (history[id].empty()) continue;
            int first = history[id][0].first;
            int group = static_cast<int>(
                lower_bound(endpoints.begin() + 1, endpoints.end(), first)
                - endpoints.begin());
            int end = endpoints[group];
            auto it = upper_bound(
                history[id].begin(), history[id].end(),
                make_pair(end, INT_MAX));
            --it;
            if (it->second != target[id]) answer += weight[id];
        }
        return answer;
    };

    if (n > 1500 || k <= 1) {
        cout << evaluateGeneral(cuts) << '\n';
        return 0;
    }

    vector<vector<int64>> cost(n + 1, vector<int64>(n + 1));
    vector<int> first(h + 1), state(h + 1);
    vector<int64> bucket(n + 1);
    for (int right = 1; right <= n; ++right) {
        int id = changed[right];
        int old = state[id];
        int next = value[right];
        if (first[id] == 0) {
            first[id] = right;
            if (next != target[id]) bucket[right] += weight[id];
        } else {
            bool oldBad = old != target[id];
            bool nextBad = next != target[id];
            if (oldBad != nextBad)
                bucket[first[id]] += nextBad ? weight[id] : -weight[id];
        }
        state[id] = next;

        int64 suffix = 0;
        for (int left = right - 1; left >= 0; --left) {
            suffix += bucket[left + 1];
            cost[left][right] = suffix;
        }
    }

    const auto partitionCost = [&]() {
        int64 result = 0;
        for (int group = 1; group <= k; ++group)
            result += cost[cuts[group - 1]][cuts[group]];
        return result;
    };

    int64 current = partitionCost();
    int64 best = current;
    mt19937 rng(0xD15EA5Eu);
    uniform_real_distribution<double> real01(0.0, 1.0);
    double temperature = 1e7;
    for (int iteration = 0; iteration < 200000 && k > 1; ++iteration) {
        int index = 1 + static_cast<int>(rng() % (k - 1));
        int direction = (rng() & 1) ? 1 : -1;
        int candidate = cuts[index] + direction;
        if (candidate <= cuts[index - 1] || candidate >= cuts[index + 1]) {
            temperature *= 0.9999;
            continue;
        }

        int oldCut = cuts[index];
        int64 before = cost[cuts[index - 1]][oldCut]
                     + cost[oldCut][cuts[index + 1]];
        int64 after = cost[cuts[index - 1]][candidate]
                    + cost[candidate][cuts[index + 1]];
        int64 delta = after - before;
        if (delta <= 0 ||
            real01(rng) < exp(-static_cast<double>(delta) / temperature)) {
            cuts[index] = candidate;
            current += delta;
            best = min(best, current);
        }
        temperature *= 0.9999;
    }

    cout << best << '\n';
}
