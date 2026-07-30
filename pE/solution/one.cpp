#include "Can_You_Blow_My_Whistle.h"

#include <vector>

using namespace std;

void solve(int n, vector<int> a) {
    bool hasSwap = false;
    for (int i = 0; i < n; ++i) {
        if (i + 1 < a[i]) {
            swap_student(i + 1, a[i]);
            hasSwap = true;
        }
    }
    if (hasSwap) {
        blow_whistle();
    }
}
