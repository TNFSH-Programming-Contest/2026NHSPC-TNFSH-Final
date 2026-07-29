#include <iostream>
#include <vector>

using namespace std;

struct DSU {
    vector<int> p;
    explicit DSU(int n) : p(n + 1, -1) {}
    int find(int x) { return p[x] < 0 ? x : p[x] = find(p[x]); }
    bool merge(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (p[a] > p[b]) swap(a, b);
        p[a] += p[b];
        p[b] = a;
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    long long r;
    cin >> n >> m >> r;

    DSU dsu(n);
    long long total = 0, saved = 0;
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long cost;
        cin >> u >> v >> cost;
        total += cost;
        if (dsu.merge(u, v))
            saved += cost;
    }
    cout << total - saved << '\n';
}
