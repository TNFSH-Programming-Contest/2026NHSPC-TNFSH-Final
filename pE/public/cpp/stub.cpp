#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>

using namespace std;

namespace {
    int n;
    vector<int> current_array;
    vector<pair<int, int>> pending_swaps;
    vector<bool> used_position;
    int whistle_count = 0;

    void quit(const string& msg) {
        cerr << "Wrong Answer: " << msg << "\n";
        exit(0);
    }
}

void swap_student(int u, int v) {
    if (u < 1 || u > n || v < 1 || v > n || u == v) {
        quit("invalid swap");
    }
    if (used_position[u] || used_position[v]) {
        quit("twice swap");
    }
    used_position[u] = true;
    used_position[v] = true;
    pending_swaps.push_back({u, v});
}

void blow_whistle() {
    for (auto p : pending_swaps) {
        swap(current_array[p.first], current_array[p.second]);
    }
    pending_swaps.clear();
    fill(used_position.begin(), used_position.end(), false);
    whistle_count++;
}

void solve(int n, std::vector<int> a);

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;
    
    current_array.assign(n + 1, 0);
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        current_array[i + 1] = a[i];
    }

    used_position.assign(n + 1, false);
    whistle_count = 0;

    solve(n, a);

    cout << "A : " << whistle_count << '\n';

    for (int i = 1; i <= n; i++) {
        cout << current_array[i] << (i == n ? "" : " ");
    }
    cout << "\n";

    return 0;
}
