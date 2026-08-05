#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int64 m;
    cin >> n >> m;
    vector<int64> position(n), capacity(n);
    for (int64& value : position) cin >> value;
    for (int i = 1; i + 1 < n; ++i) cin >> capacity[i];

    const int bits = min(n - 2, 60);
    const uint64_t allMasks = uint64_t{1} << bits;
    const uint64_t workLimitedMasks =
        max<uint64_t>(1, 4000000ULL / static_cast<uint64_t>(n));
    const uint64_t maskLimit = min(allMasks, workLimitedMasks);
    const int64 required = 2 * m;
    int64 answer = 0;

    for (uint64_t mask = 0; mask < maskLimit; ++mask) {
        int64 removedCapacity = 0;
        int last = 0;
        int64 largestJump = 0;
        for (int i = 1; i < n; ++i) {
            const bool removed =
                i + 1 < n && i <= bits && ((mask >> (i - 1)) & 1ULL);
            if (removed) {
                removedCapacity += capacity[i];
            } else {
                largestJump = max(largestJump, position[i] - position[last]);
                last = i;
            }
        }
        if (removedCapacity < required) answer = max(answer, largestJump);
    }

    cout << answer << '\n';
}
