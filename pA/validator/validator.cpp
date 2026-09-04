#include "testlib.h"

#include <algorithm>
#include <string>
#include <set>

using namespace std;

namespace {

const int MAX_N = 2000;
const int MAX_M = 10000;
const int MAX_COST = 100000;

struct Limits {
    int n;
    int m;
    int cost;
};

Limits getLimits(int argc, char* argv[]) {
    Limits limits = {MAX_N, MAX_M, MAX_COST};
    if (argc <= 1) {
        return limits;
    }

    const string mode = argv[1];
    if (mode == "brute") {
        limits = {8, 30, 100};
    } else if (mode == "dp") {
        limits = {18, 200, 100};
    } else if (mode == "no-weight") {
        limits = {1000, 5000, 1};
    } else if (mode == "km") {
        limits = {200, 1000, MAX_COST};
    } else {
        quitf(_fail, "unknown validator mode: %s", mode.c_str());
    }
    return limits;
}

}  // namespace

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);
    const Limits limits = getLimits(argc, argv);

    const int n = inf.readInt(2, limits.n, "n");
    inf.readSpace();
    const int maximum_edges = min(limits.m, n * (n - 1));
    const int m = inf.readInt(1, maximum_edges, "m");
    inf.readEoln();

    set<pair<int, int> > se;

    for (int i = 0; i < m; ++i) {
        const int from = inf.readInt(1, n, format("a[%d]", i + 1));
        inf.readSpace();
        const int to = inf.readInt(1, n, format("b[%d]", i + 1));
        ensuref(from != to, "road %d is a forbidden self-loop at %d", i + 1, from);
        inf.readSpace();
        ensure(!(se.count({from, to})));
        se.insert({from, to});
        inf.readInt(0, limits.cost, format("d[%d]", i + 1));
        inf.readEoln();
    }
    inf.readEof();
    return 0;
}
