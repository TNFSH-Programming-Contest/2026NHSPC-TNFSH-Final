#include "common.h"

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    if (argc < 5) {
        Instance::fail(
            "usage: gen N M COORDINATE_STYLE CAPACITY_STYLE [seed-tag]");
    }

    const int64 parsedN = parseInteger(argv[1], "N");
    const int64 parsedM = parseInteger(argv[2], "M");
    if (parsedN < INT_MIN || parsedN > INT_MAX) {
        Instance::fail("N is out of int range");
    }

    Instance instance(static_cast<int>(parsedN), parsedM);
    instance.setCoordinates(argv[3]);
    instance.setCapacities(argv[4]);
    instance.verifyAndPrint();
    return 0;
}
