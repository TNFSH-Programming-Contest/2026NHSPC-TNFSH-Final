#include "common.h"

void generateBoundary(Instance& instance) {
    instance.setCoordinates("alternating");
    for (int i = 1; i + 1 < instance.n; ++i) {
        instance.capacity[i] = instance.required();
    }
}

void generatePriority(Instance& instance) {
    std::vector<int64> weight(instance.n - 1, 1);
    const int64 need = instance.required();
    for (int i = 1; i + 1 < instance.n; ++i) {
        if (i % 3 == 1) {
            instance.capacity[i] = std::max<int64>(1, need - 1);
            weight[i] = 1000000000000LL;
        } else if (i % 3 == 2) {
            instance.capacity[i] = 1;
            weight[i - 1] = 1000000000000LL;
        } else {
            instance.capacity[i] = std::max<int64>(1, need / 2);
        }
    }
    instance.setCoordinatesFromWeights(weight);
}

void generateOverflow(Instance& instance) {
    std::vector<int64> weight(instance.n - 1, 1);
    for (int i = 1; i + 1 < instance.n; ++i) {
        if (i < instance.n * 2 / 3) {
            instance.capacity[i] = MAX_A;
        } else {
            instance.capacity[i] = (i % 5 == 0 ? 1 : instance.required() - 1);
            weight[i - 1] = (i % 7 == 0 ? 1000000000000LL : 1);
        }
    }
    instance.setCoordinatesFromWeights(weight);
}

void generateWindow(Instance& instance) {
    instance.setCoordinates("full-uniform");
    for (int i = 1; i + 1 < instance.n; ++i) {
        instance.capacity[i] = 1;
    }
}

void generatePartials(Instance& instance) {
    std::vector<int64> weight(instance.n - 1, 1);
    const int64 need = instance.required();
    for (int i = 1; i + 1 < instance.n; ++i) {
        instance.capacity[i] = std::max<int64>(
            1, std::min<int64>(MAX_A, (need + 2) / 3 + i % 2));
        weight[i - 1] = (i % 4 == 0 ? 1000000000000LL : i % 11 + 1);
    }
    instance.setCoordinatesFromWeights(weight);
}

void generatePairing(Instance& instance) {
    instance.setCoordinates("alternating");
    const int64 need = instance.required();
    for (int i = 1; i + 1 < instance.n; ++i) {
        instance.capacity[i] = (i % 2 == 1 ? 1 : need - 1);
    }
}

void generateBarriers(Instance& instance) {
    std::vector<int64> weight(instance.n - 1, 1);
    const int64 need = instance.required();
    for (int i = 1; i + 1 < instance.n; ++i) {
        const int phase = static_cast<int>(12LL * i / instance.n);
        if (phase % 4 == 0) {
            instance.capacity[i] = 1;
            weight[i - 1] = 1000000000000LL;
        } else if (phase % 4 == 1) {
            instance.capacity[i] = std::max<int64>(1, need - 1);
        } else if (phase % 4 == 2) {
            instance.capacity[i] = need;
            weight[i] = 1000000000000LL;
        } else {
            instance.capacity[i] = MAX_A;
        }
    }
    instance.setCoordinatesFromWeights(weight);
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    if (argc < 4) {
        Instance::fail("usage: attack MODE N M [seed-tag]");
    }

    const std::string mode = argv[1];
    const int64 parsedN = parseInteger(argv[2], "N");
    const int64 parsedM = parseInteger(argv[3], "M");
    if (parsedN < INT_MIN || parsedN > INT_MAX) {
        Instance::fail("N is out of int range");
    }

    Instance instance(static_cast<int>(parsedN), parsedM);
    if (mode == "boundary") {
        generateBoundary(instance);
    } else if (mode == "priority") {
        generatePriority(instance);
    } else if (mode == "overflow") {
        generateOverflow(instance);
    } else if (mode == "window") {
        generateWindow(instance);
    } else if (mode == "partials") {
        generatePartials(instance);
    } else if (mode == "pairing") {
        generatePairing(instance);
    } else if (mode == "barriers") {
        generateBarriers(instance);
    } else {
        Instance::fail("unknown attack mode: " + mode);
    }

    instance.verifyAndPrint();
    return 0;
}
