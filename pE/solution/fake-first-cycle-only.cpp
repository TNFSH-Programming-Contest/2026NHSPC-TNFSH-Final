#include "Can_You_Blow_My_Whistle.h"

#include <vector>

using namespace std;

void solve(int, vector<int> a) {
    vector<int> cycle;
    int vertex = 0;
    do {
        cycle.push_back(vertex);
        vertex = a[vertex] - 1;
    } while (vertex != 0);

    vector<pair<int, int> > rounds[2];
    const int length = static_cast<int>(cycle.size());
    for (int i = 0; i < length; ++i) {
        int j = (length - i) % length;
        if (i < j) rounds[0].push_back({cycle[i] + 1, cycle[j] + 1});
        j = (1 - i + length) % length;
        if (i < j) rounds[1].push_back({cycle[i] + 1, cycle[j] + 1});
    }

    for (int round = 0; round < 2; ++round) {
        if (rounds[round].empty()) continue;
        for (const auto& operation : rounds[round]) {
            swap_student(operation.first, operation.second);
        }
        blow_whistle();
    }
}
