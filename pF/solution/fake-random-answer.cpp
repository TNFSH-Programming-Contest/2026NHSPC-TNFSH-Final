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

    int n, q;
    cin >> n >> q;
    uint64 state = 0xF00D5EEDULL;
    absorb(state, n);
    absorb(state, q);
    for (int i = 0; i < n; ++i) {
        long long value;
        cin >> value;
        absorb(state, value);
    }

    for (int operation = 0; operation < q; ++operation) {
        int type;
        cin >> type;
        absorb(state, type);
        if (type == 1 || type == 2) {
            int a, b;
            cin >> a >> b;
            absorb(state, a);
            absorb(state, b);
        } else if (type == 3) {
            long long left, right, delta;
            cin >> left >> right >> delta;
            absorb(state, left);
            absorb(state, right);
            absorb(state, delta);
        } else if (type == 4) {
            int vertex;
            cin >> vertex;
            absorb(state, vertex);
            state = mix(state);
            const long long guess = static_cast<long long>(
                state % 2000000000000000001ULL) - 1000000000000000000LL;
            cout << guess << '\n';
        }
    }
}
