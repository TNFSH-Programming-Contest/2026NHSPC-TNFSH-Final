#include "Can_You_Blow_My_Whistle.h"

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

void solve(int n, vector<int> a) {
    uint64_t seed = 0xE11E57ULL;
    for (int value : a) {
        seed ^= static_cast<uint64_t>(value) + 0x9e3779b97f4a7c15ULL
              + (seed << 6) + (seed >> 2);
    }
    mt19937 rng(static_cast<uint32_t>(seed ^ (seed >> 32)));
    vector<int> positions(n);
    iota(positions.begin(), positions.end(), 1);

    // Guess two legal parallel matchings without using the permutation's
    // cycle structure.  Their composition almost never equals the target.
    for (int round = 0; round < 2; ++round) {
        shuffle(positions.begin(), positions.end(), rng);
        bool usedAny = false;
        for (int i = 0; i + 1 < n; i += 2) {
            swap_student(positions[i], positions[i + 1]);
            usedAny = true;
        }
        if (usedAny) blow_whistle();
    }
}
