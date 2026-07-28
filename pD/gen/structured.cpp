#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;
#include "common.h"

void generateOnce(Instance& in, bool overflow) {
    if (in.n > in.h) {
        cerr << "once mode requires N <= H\n";
        exit(1);
    }
    vector<int> hunks(in.h);
    iota(hunks.begin(), hunks.end(), 1);
    shuffleVector(hunks);
    for (int i = 0; i < in.n; ++i) {
        int hunk = hunks[i];
        int value = overflow ? 2
            : (i % 3 == 0 ? in.u[hunk] : in.otherValue(hunk, i + 1));
        in.add(hunk, value);
    }
}

void generateBlocks(Instance& in, int active) {
    vector<int> count(active + 1);
    for (int i = 0; i < in.n; ++i) {
        int hunk = static_cast<int>(1LL * i * active / in.n) + 1;
        int phase = ++count[hunk];
        int value = phase % 3 == 0 && in.current[hunk] != in.u[hunk]
            ? in.u[hunk] : in.otherValue(hunk, phase);
        in.add(hunk, value);
    }
}

void generateLateFirst(Instance& in, int active) {
    vector<int> count(active + 1);
    int prefix = in.n * 2 / 3;
    for (int i = 0; i < in.n; ++i) {
        int hunk = i < prefix || active == 1
            ? 1 : 2 + (i - prefix) % (active - 1);
        in.add(hunk, cycleValue(in, hunk, ++count[hunk]));
    }
}

void generateOverflow(Instance& in, int active) {
    vector<int> count(active + 1);
    for (int i = 0; i < in.n; ++i) {
        int hunk = i < active ? i + 1 : rnd.next(1, active);
        in.add(hunk, ++count[hunk] % 2 == 1 ? 2 : 1);
    }
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    if (argc < 2) return 1;
    string mode = argv[1];
    bool overflow = mode == "overflow" || mode == "once-overflow";
    int active;
    Instance in = makeInstance(argc, argv, active, overflow);

    if (mode == "once" || mode == "once-overflow")
        generateOnce(in, overflow);
    else if (mode == "blocks")
        generateBlocks(in, active);
    else if (mode == "late-first")
        generateLateFirst(in, active);
    else if (mode == "overflow")
        generateOverflow(in, active);
    else {
        cerr << "unknown structured mode: " << mode << '\n';
        return 1;
    }
    in.print();
}
