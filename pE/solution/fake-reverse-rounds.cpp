#include "Can_You_Blow_My_Whistle.h"

#include <vector>

using namespace std;

void solve(int n, vector<int> a) {
    vector<unsigned char> visited(n, false);
    vector<pair<int, int> > firstRound;
    vector<pair<int, int> > secondRound;
    for (int start = 0; start < n; ++start) {
        if (visited[start]) continue;
        vector<int> cycle;
        int vertex = start;
        while (!visited[vertex]) {
            visited[vertex] = true;
            cycle.push_back(vertex);
            vertex = a[vertex] - 1;
        }
        const int length = static_cast<int>(cycle.size());
        for (int i = 0; i < length; ++i) {
            int j = (length - i) % length;
            if (i < j) firstRound.push_back({cycle[i] + 1, cycle[j] + 1});
            j = (1 - i + length) % length;
            if (i < j) secondRound.push_back({cycle[i] + 1, cycle[j] + 1});
        }
    }

    if (!secondRound.empty()) {
        for (const auto& operation : secondRound) {
            swap_student(operation.first, operation.second);
        }
        blow_whistle();
    }
    if (!firstRound.empty()) {
        for (const auto& operation : firstRound) {
            swap_student(operation.first, operation.second);
        }
        blow_whistle();
    }
}
