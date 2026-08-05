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
    long long m;
    cin >> n >> m;
    vector<long long> position(n);
    uint64 state = 0xBADC0FFEEULL;
    absorb(state, n);
    absorb(state, m);
    for (long long& value : position) {
        cin >> value;
        absorb(state, value);
    }
    for (int i = 1; i + 1 < n; ++i) {
        long long capacity;
        cin >> capacity;
        absorb(state, capacity);
    }

    const uint64 span = static_cast<uint64>(position.back() - position.front());
    cout << static_cast<long long>(mix(state) % (span + 1)) << '\n';
}
