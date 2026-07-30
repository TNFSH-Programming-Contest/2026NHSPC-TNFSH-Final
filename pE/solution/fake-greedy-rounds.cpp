#include "Can_You_Blow_My_Whistle.h"

#include <algorithm>
#include <vector>

using namespace std;

void solve(int n, vector<int> a) {
    while (true) {
        vector<int> position(n + 1);
        for (int i = 0; i < n; ++i) {
            position[a[i]] = i;
        }

        vector<unsigned char> used(n, false);
        vector<pair<int, int> > operations;
        for (int i = 0; i < n; ++i) {
            if (a[i] == i + 1 || used[i]) continue;
            const int other = position[i + 1];
            if (used[other]) continue;
            used[i] = used[other] = true;
            operations.push_back({i, other});
        }
        if (operations.empty()) {
            return;
        }

        for (const auto& operation : operations) {
            swap_student(operation.first + 1, operation.second + 1);
        }
        blow_whistle();
        for (const auto& operation : operations) {
            swap(a[operation.first], a[operation.second]);
        }
    }
}
