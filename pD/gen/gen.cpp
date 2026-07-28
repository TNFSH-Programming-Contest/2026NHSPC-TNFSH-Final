#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;
#include "common.h"

void generateRandom(Instance& in, int active) {
    vector<int> hunks;
    for (int h = 1; h <= active && (int)hunks.size() < in.n; ++h)
        hunks.push_back(h);
    while ((int)hunks.size() < in.n) hunks.push_back(rnd.next(1, active));
    shuffleVector(hunks);

    for (int hunk : hunks) {
        int roll = rnd.next(0, 9);
        int value;
        if (roll <= 2 && in.current[hunk] != in.u[hunk])
            value = in.u[hunk];
        else if (roll <= 5)
            value = in.otherValue(hunk, rnd.next(1, MAX_VALUE));
        else {
            value = rnd.next(1, MAX_VALUE);
            if (value == in.current[hunk])
                value = value == MAX_VALUE ? 1 : value + 1;
        }
        in.add(hunk, value);
    }
}

void generateToggle(Instance& in, int active) {
    vector<int> count(active + 1);
    for (int i = 0; i < in.n; ++i) {
        int hunk = i % active + 1;
        int phase = ++count[hunk];
        bool target = ((phase + hunk) & 1) == 0;
        int value = target && in.current[hunk] != in.u[hunk]
            ? in.u[hunk] : in.otherValue(hunk, phase);
        in.add(hunk, value);
    }
}

void generateCycle(Instance& in, int active) {
    vector<int> count(active + 1);
    for (int i = 0; i < in.n; ++i) {
        int hunk = static_cast<int>(
            (1LL * i * 37 + i / max(1, active)) % active) + 1;
        in.add(hunk, cycleValue(in, hunk, ++count[hunk]));
    }
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    int active;
    Instance in = makeInstance(argc, argv, active);
    string mode = argv[1];
    if (mode == "random") generateRandom(in, active);
    else if (mode == "toggle") generateToggle(in, active);
    else if (mode == "cycle") generateCycle(in, active);
    else {
        cerr << "unknown gen mode: " << mode << '\n';
        return 1;
    }
    in.print();
}
