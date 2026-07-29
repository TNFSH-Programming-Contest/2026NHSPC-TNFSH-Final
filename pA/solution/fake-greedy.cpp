#include <bits/stdc++.h>
using namespace std;

struct Road { int from, to, cost; };

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<Road> roads;
    for (int i = 0; i < m; ++i) {
        Road road;
        cin >> road.from >> road.to >> road.cost;
        if (road.from != n && road.to != 1) roads.push_back(road);
    }
    stable_sort(roads.begin(), roads.end(), [](const Road& a, const Road& b) {
        return a.cost < b.cost;
    });
    vector<char> used_left(n + 1), used_right(n + 1);
    long long answer = 0;
    int picked = 0;
    for (const Road& road : roads) {
        if (used_left[road.from] || used_right[road.to]) continue;
        used_left[road.from] = used_right[road.to] = true;
        answer += road.cost;
        ++picked;
    }
    cout << (picked == n - 1 ? answer : -1) << '\n';
}
