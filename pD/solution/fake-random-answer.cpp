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

    uint64 state = 0xD15EA5EULL;
    long long value;
    while (cin >> value) absorb(state, value);
    cout << static_cast<long long>(mix(state) & 0x7fffffffffffffffULL) << '\n';
}
