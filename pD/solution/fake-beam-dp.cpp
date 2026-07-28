#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
const int64 INF = (1LL << 62);

struct Candidate {
    int cut;
    int64 value;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, h, k;
    cin >> n >> h >> k;
    vector<int> target(h + 1), weight(h + 1);
    for (int i = 1; i <= h; ++i) cin >> target[i];
    for (int i = 1; i <= h; ++i) cin >> weight[i];

    vector<int> first(h + 1), state(h + 1), right(n + 1, -1);
    vector<int64> delta(n + 1);
    for (int i = 1; i <= n; ++i) {
        int x, value;
        cin >> x >> value;
        int old = state[x];
        if (first[x] == 0) {
            first[x] = i;
            right[i] = i - 1;
            if (value != target[x]) delta[i] = weight[x];
        } else {
            right[i] = first[x] - 1;
            bool oldBad = old != target[x];
            bool newBad = value != target[x];
            if (!oldBad && newBad) delta[i] = weight[x];
            else if (oldBad && !newBad) delta[i] = -weight[x];
        }
        state[x] = value;
    }

    vector<int64> previous(n + 1, INF), next(n + 1, INF);
    previous[0] = 0;
    for (int groups = 1; groups <= k; ++groups) {
        fill(next.begin(), next.end(), INF);
        vector<Candidate> beam;
        for (int r = 1; r <= n; ++r) {
            for (Candidate& candidate : beam)
                if (candidate.cut <= right[r]) candidate.value += delta[r];

            int newCut = r - 1;
            int64 newValue = previous[newCut];
            if (newValue != INF && newCut <= right[r]) newValue += delta[r];
            if (newValue != INF) beam.push_back({newCut, newValue});

            sort(beam.begin(), beam.end(),
                 [](const Candidate& a, const Candidate& b) {
                     if (a.value != b.value) return a.value < b.value;
                     return a.cut > b.cut;
                 });
            if (beam.size() > 4) beam.resize(4);
            if (r >= groups && !beam.empty()) next[r] = beam.front().value;
        }
        previous.swap(next);
    }
    cout << previous[n] << '\n';
}
