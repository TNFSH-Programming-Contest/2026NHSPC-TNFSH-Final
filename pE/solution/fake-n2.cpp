#include "Can_You_Blow_My_Whistle.h"

#include <vector>

using namespace std;

void solve(int n, vector<int> a) {
    // Natural but quadratic preprocessing: find the current position of every
    // student by scanning the whole array independently.
    vector<int> position(n, -1);
    for (int value = 0; value < n; ++value) {
        for (int index = 0; index < n; ++index) {
            if (a[index] == value + 1) {
                position[value] = index;
            }
        }
    }
    vector<int> permutation(n);
    for (int value = 0; value < n; ++value) {
        permutation[position[value]] = value + 1;
    }

    vector<unsigned char> visited(n, false);
    vector<pair<int, int> > rounds[2];
    for (int start = 0; start < n; ++start) {
        if (visited[start]) continue;
        vector<int> cycle;
        int vertex = start;
        while (!visited[vertex]) {
            visited[vertex] = true;
            cycle.push_back(vertex);
            vertex = permutation[vertex] - 1;
        }
        const int length = static_cast<int>(cycle.size());
        for (int i = 0; i < length; ++i) {
            int j = (length - i) % length;
            if (i < j) rounds[0].push_back({cycle[i] + 1, cycle[j] + 1});
            j = (1 - i + length) % length;
            if (i < j) rounds[1].push_back({cycle[i] + 1, cycle[j] + 1});
        }
    }

    for (int round = 0; round < 2; ++round) {
        if (rounds[round].empty()) continue;
        for (const auto& operation : rounds[round]) {
            swap_student(operation.first, operation.second);
        }
        blow_whistle();
    }
}
