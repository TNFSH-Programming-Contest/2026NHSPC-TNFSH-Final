#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    vector<unsigned char> depth(n + 1, 0);
    for (int i = 1; i < n; ++i) {
        int parent, child;
        cin >> parent >> child;
        depth[child] = depth[parent] ^ 1;
    }

    long long value = 0;
    for (int v = 2; v <= n; ++v) {
        if (depth[v]) {
            value ^= a[v];
        }
    }
    cout << (value ? "Alice" : "Bob") << '\n';
}
