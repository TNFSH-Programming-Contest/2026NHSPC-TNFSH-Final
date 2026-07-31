#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    long long value = 0;
    for (int i = 1; i <= n; ++i) {
        long long x;
        cin >> x;
        value ^= x;
    }
    for (int i = 1, u, v; i < n; ++i) {
        cin >> u >> v;
    }
    cout << (value ? "Alice" : "Bob") << '\n';
}
