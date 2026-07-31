#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    long long nimSum = 0;
    for (int i = 1; i <= n; ++i) {
        long long stones;
        cin >> stones;
        if (i % 2 == 0) {
            nimSum ^= stones;
        }
    }
    for (int i = 1, u, v; i < n; ++i) {
        cin >> u >> v;
    }

    cout << (nimSum != 0 ? "Alice" : "Bob") << '\n';
    return 0;
}
