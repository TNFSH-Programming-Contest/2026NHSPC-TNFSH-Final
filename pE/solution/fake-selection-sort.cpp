#include "Can_You_Blow_My_Whistle.h"

#include <algorithm>
#include <vector>

using namespace std;

void solve(int n, vector<int> a) {
    for (int position = 0; position < n; ++position) {
        if (a[position] == position + 1) continue;
        const int other =
            find(a.begin() + position + 1, a.end(), position + 1) - a.begin();
        swap_student(position + 1, other + 1);
        blow_whistle();
        swap(a[position], a[other]);
    }
}
