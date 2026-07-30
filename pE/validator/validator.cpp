#include "testlib.h"

#include <string>
#include <vector>

using namespace std;

namespace {

const int MAX_N = 100000;

struct Input {
    int n;
    vector<int> permutation;
};

Input readInput() {
    Input input;
    input.n = inf.readInt(1, MAX_N, "n");
    inf.readEoln();
    input.permutation = inf.readInts(input.n, 1, input.n, "a");
    inf.readEoln();
    inf.readEof();

    vector<int> frequency(input.n + 1, 0);
    for (int value : input.permutation) {
        ++frequency[value];
    }
    for (int value = 1; value <= input.n; ++value) {
        ensuref(frequency[value] == 1,
                "a is not a permutation: value %d occurs %d times",
                value, frequency[value]);
    }

    return input;
}

void validateBrute(const Input& input) {
    ensuref(input.n <= 2000,
            "brute subtask requires n <= 2000, but n = %d", input.n);
}

void validateOptOne(const Input& input) {
    bool hasSwap = false;
    for (int position = 0; position < input.n; ++position) {
        const int other = input.permutation[position] - 1;
        ensuref(input.permutation[other] == position + 1,
                "one subtask requires every cycle to have length at most 2");
        hasSwap = hasSwap || other != position;
    }
    ensuref(hasSwap, "one subtask requires opt = 1, but the permutation is sorted");
}

}  // namespace

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    const string mode = argc >= 2 ? argv[1] : "full";
    ensuref(mode == "full" || mode == "brute" || mode == "one",
            "unknown validator mode: %s", mode.c_str());

    const Input input = readInput();
    if (mode == "brute") {
        validateBrute(input);
    } else if (mode == "one") {
        validateOptOne(input);
    }

    return 0;
}
