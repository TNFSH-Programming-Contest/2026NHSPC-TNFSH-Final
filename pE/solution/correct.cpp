#include "Can_You_Blow_My_Whistle.h"

#include <vector>

using namespace std;

void solve(int n, vector<int> a) {
    vector<char> visited(n, false);
    vector<pair<int, int> > firstRound;
    vector<pair<int, int> > secondRound;

    for (int start = 0; start < n; ++start) {
        if (visited[start]) {
            continue;
        }

        vector<int> cycle;
        int vertex = start;
        while (!visited[vertex]) {
            visited[vertex] = true;
            cycle.push_back(vertex);
            vertex = a[vertex] - 1;
        }

        const int length = static_cast<int>(cycle.size());
        for (int i = 0; i < length; ++i) {
            const int j = (length - i) % length;
            if (i < j) {
                firstRound.push_back({cycle[i] + 1, cycle[j] + 1});
            }
        }
        for (int i = 0; i < length; ++i) {
            const int j = (1 - i + length) % length;
            if (i < j) {
                secondRound.push_back({cycle[i] + 1, cycle[j] + 1});
            }
        }
    }

    if (!firstRound.empty()) {
        for (const pair<int, int>& operation : firstRound) {
            swap_student(operation.first, operation.second);
        }
        blow_whistle();
    }
    if (!secondRound.empty()) {
        for (const pair<int, int>& operation : secondRound) {
            swap_student(operation.first, operation.second);
        }
        blow_whistle();
    }
}
