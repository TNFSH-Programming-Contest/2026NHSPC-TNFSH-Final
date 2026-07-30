#include "Can_You_Blow_My_Whistle.h"

#include <vector>

using namespace std;

void solve(int n, vector<int>) {
    vector<pair<int, int> > rounds[2];
    for (int i = 0; i < n; ++i) {
        int j = (n - i) % n;
        if (i < j) rounds[0].push_back({i + 1, j + 1});
        j = (1 - i + n) % n;
        if (i < j) rounds[1].push_back({i + 1, j + 1});
    }
    for (int round = 0; round < 2; ++round) {
        if (rounds[round].empty()) continue;
        for (const auto& operation : rounds[round]) {
            swap_student(operation.first, operation.second);
        }
        blow_whistle();
    }
}
