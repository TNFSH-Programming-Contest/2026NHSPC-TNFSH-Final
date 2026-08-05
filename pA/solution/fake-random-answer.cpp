#include <bits/stdc++.h>
using namespace std;

using uint64 = uint64_t;

uint64 mix(uint64 value) {
    value += 0x9e3779b97f4a7c15ULL;
    value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
    value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
    return value ^ (value >> 31);
}

void absorb(uint64& state, long long value) {
    state = mix(state ^ mix(static_cast<uint64>(value)));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    uint64 state = 0xA11CE5EEDULL;
    absorb(state, n);
    absorb(state, m);
    int maximumCost = 0;
    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        cin >> from >> to >> cost;
        absorb(state, from);
        absorb(state, to);
        absorb(state, cost);
        maximumCost = max(maximumCost, cost);
    }

    state = mix(state);
    if ((state & 3ULL) == 0) {
        cout << -1 << '\n';
    } else {
        const long long upper = 1LL * (n - 1) * maximumCost;
        cout << static_cast<long long>(state % (upper + 1)) << '\n';
    }
}
