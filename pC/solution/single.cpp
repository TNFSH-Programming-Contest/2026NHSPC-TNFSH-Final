#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> stones(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> stones[i];
    }
    for (int i = 1, u, v; i < n; ++i) {
        cin >> u >> v;
    }

    cout << (n == 2 && stones[2] != 0 ? "Alice" : "Bob") << '\n';
    return 0;
}
