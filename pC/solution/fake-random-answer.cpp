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

    int n;
    cin >> n;
    uint64 state = 0xC0DEC0FFEEULL;
    absorb(state, n);
    for (int i = 0; i < n; ++i) {
        int stones;
        cin >> stones;
        absorb(state, stones);
    }
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        absorb(state, u);
        absorb(state, v);
    }

    cout << ((mix(state) & 1ULL) ? "Alice" : "Bob") << '\n';
}
