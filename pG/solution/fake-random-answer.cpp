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
    long long r;
    cin >> n >> m >> r;
    uint64 state = 0x600D5EEDULL;
    absorb(state, n);
    absorb(state, m);
    absorb(state, r);
    uint64 totalCost = 0;
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long cost;
        cin >> u >> v >> cost;
        absorb(state, u);
        absorb(state, v);
        absorb(state, cost);
        totalCost += static_cast<uint64>(cost);
    }

    cout << static_cast<long long>(mix(state) % (totalCost + 1)) << '\n';
}
