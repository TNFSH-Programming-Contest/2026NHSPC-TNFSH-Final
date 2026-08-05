#include <bits/stdc++.h>
using namespace std;

enum Result : unsigned char {
    LOSE,
    WIN,
    UNKNOWN
};

struct VectorHash {
    size_t operator()(const vector<int>& values) const {
        size_t result = values.size();
        for (int value : values) {
            result ^= static_cast<size_t>(value) + 0x9e3779b9U
                    + (result << 6) + (result >> 2);
        }
        return result;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> stones(n + 1);
    for (int i = 1; i <= n; ++i) cin >> stones[i];
    vector<vector<int>> graph(n + 1);
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    vector<int> parent(n + 1, -1), depth(n + 1), order(1, 1);
    parent[1] = 0;
    for (size_t at = 0; at < order.size(); ++at) {
        int u = order[at];
        for (int v : graph[u]) {
            if (v == parent[u]) continue;
            parent[v] = u;
            depth[v] = depth[u] + 1;
            order.push_back(v);
        }
    }

    vector<int> heaps;
    long long totalParity = 0;
    for (int v = 1; v <= n; ++v) {
        if ((depth[v] & 1) && stones[v] > 0) {
            heaps.push_back(stones[v]);
            totalParity ^= stones[v] & 1;
        }
    }
    sort(heaps.begin(), heaps.end());

    constexpr long long MAGIC = 100000;
    long long transitions = 0;
    unordered_map<vector<int>, Result, VectorHash> memo;

    function<Result(vector<int>&)> search = [&](vector<int>& state) -> Result {
        auto found = memo.find(state);
        if (found != memo.end()) return found->second;

        bool hasMove = false;
        bool sawUnknown = false;
        for (int index = 0; index < static_cast<int>(state.size()); ++index) {
            const int oldValue = state[index];
            for (int next = 0; next < oldValue; ++next) {
                hasMove = true;
                if (++transitions > MAGIC) return UNKNOWN;
                state[index] = next;
                Result child = search(state);
                state[index] = oldValue;
                if (child == LOSE) {
                    memo[state] = WIN;
                    return WIN;
                }
                if (child == UNKNOWN) sawUnknown = true;
            }
        }
        if (!hasMove) return memo[state] = LOSE;
        if (sawUnknown) return UNKNOWN;
        return memo[state] = LOSE;
    };

    Result result = UNKNOWN;
    if (heaps.size() <= 18) result = search(heaps);
    if (result == UNKNOWN) result = totalParity ? WIN : LOSE;
    cout << (result == WIN ? "Alice" : "Bob") << '\n';
}
