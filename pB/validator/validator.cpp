#include "testlib.h"

#include <string>
#include <vector>

using namespace std;

namespace {

const int MAX_N = 100000;
const int MAX_M = 100000;
const int MIN_X = -1000000000;
const int MAX_X = 1000000000;
const int MAX_A = 1000000000;

struct Input {
    int n;
    int m;
    vector<int> x;
    vector<int> a;
};

Input readInput() {
    Input input;

    input.n = inf.readInt(3, MAX_N, "n");
    inf.readSpace();
    input.m = inf.readInt(1, MAX_M, "m");
    inf.readEoln();

    input.x.resize(input.n);
    for (int i = 0; i < input.n; ++i) {
        input.x[i] = inf.readInt(MIN_X, MAX_X, format("x[%d]", i + 1));
        if (i + 1 == input.n) {
            inf.readEoln();
        } else {
            inf.readSpace();
        }
    }

    for (int i = 1; i < input.n; ++i) {
        ensuref(input.x[i - 1] < input.x[i],
                "positions must be strictly increasing, but x[%d] = %d and x[%d] = %d",
                i, input.x[i - 1], i + 1, input.x[i]);
    }

    input.a.resize(input.n - 2);
    for (int i = 0; i < input.n - 2; ++i) {
        input.a[i] = inf.readInt(1, MAX_A, format("a[%d]", i + 2));
        if (i + 1 == input.n - 2) {
            inf.readEoln();
        } else {
            inf.readSpace();
        }
    }

    inf.readEof();
    return input;
}

void validateEasy(const Input& input) {
    ensuref(input.m == 1,
            "easy subtask requires m = 1, but m = %d", input.m);
    for (int i = 0; i < static_cast<int>(input.a.size()); ++i) {
        ensuref(input.a[i] == MAX_A,
                "easy subtask requires a[%d] = 10^9, but a[%d] = %d",
                i + 2, i + 2, input.a[i]);
    }
}

void validateDp(const Input& input) {
    ensuref(input.n <= 2000,
            "dp subtask requires n <= 2000, but n = %d", input.n);
    ensuref(input.m == 1,
            "dp subtask requires m = 1, but m = %d", input.m);
    for (int i = 0; i < static_cast<int>(input.a.size()); ++i) {
        ensuref(input.a[i] == 1,
                "dp subtask requires a[%d] = 1, but a[%d] = %d",
                i + 2, i + 2, input.a[i]);
    }
}

void validateBrute(const Input& input) {
    ensuref(input.n <= 7,
            "brute subtask requires n <= 7, but n = %d", input.n);
    ensuref(input.m <= 7,
            "brute subtask requires m <= 7, but m = %d", input.m);
}

void validateGraph(const Input& input) {
    ensuref(input.n <= 100,
            "graph subtask requires n <= 100, but n = %d", input.n);
    ensuref(input.m <= 100,
            "graph subtask requires m <= 100, but m = %d", input.m);

    long long sumA = 0;
    for (int value : input.a) {
        sumA += value;
    }
    ensuref(sumA <= 10000,
            "graph subtask requires sum(a_i) <= 10^4, but the sum is %lld",
            sumA);
}

void validateDp2(const Input& input) {
    ensuref(input.n <= 2000,
            "dp2 subtask requires n <= 2000, but n = %d", input.n);
}

}  // namespace

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    const string mode = argc >= 2 ? argv[1] : "full";
    ensuref(mode == "full" || mode == "easy" || mode == "dp" ||
                mode == "brute" || mode == "graph" || mode == "dp2",
            "unknown validator mode: %s", mode.c_str());

    const Input input = readInput();

    if (mode == "easy") {
        validateEasy(input);
    } else if (mode == "dp") {
        validateDp(input);
    } else if (mode == "brute") {
        validateBrute(input);
    } else if (mode == "graph") {
        validateGraph(input);
    } else if (mode == "dp2") {
        validateDp2(input);
    }

    return 0;
}
