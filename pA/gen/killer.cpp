#include "common.h"

using namespace std;

namespace {

void generateKmKiller(int n) {
    if (n < 4) Instance::fail("km-killer needs n >= 4");
    const int side = n - 1;
    Instance instance(n);

    for (int x = 0; x < side; ++x) {
        const int own_column = (x + 1) % side;
        if (x > 0)
            instance.add(x + 1, x + 2, 0);
        instance.add(x + 1, own_column + 2, 1);
    }
    instance.print();
}

void generateSpfaKiller(int n, int core_size) {
    if (n < 8) Instance::fail("spfa-killer needs n >= 8");
    const int side = n - 1;
    if (core_size < 2 || core_size >= side)
        Instance::fail("SPFA core must satisfy 2 <= core < n-1");
    Instance instance(n);
    const auto coreColumn = [&](int x) { return (x + 1) % core_size; };

    for (int x = 0; x < core_size; ++x)
        instance.add(x + 1, coreColumn(x) + 2, 0);
    for (int x = 1; x < core_size; ++x)
        instance.add(1, coreColumn(x) + 2, 2 * (core_size - x));
    for (int x = 1; x < core_size; ++x)
        instance.add(x + 1, coreColumn(x - 1) + 2, 1);
    for (int x = core_size; x < side; ++x) {
        instance.add(x + 1, x + 2, 60000);
        instance.add(x + 1, coreColumn(0) + 2, 50000);
    }
    instance.print();
}

}  // namespace

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    requireArguments(argc, 3, "killer MODE N [CORE]");
    const string mode = argv[1];
    if (mode == "km")
        generateKmKiller(parseInt(argv[2], "N"));
    else if (mode == "spfa") {
        requireArguments(argc, 4, "killer spfa N CORE");
        generateSpfaKiller(parseInt(argv[2], "N"), parseInt(argv[3], "CORE"));
    } else
        Instance::fail("unknown killer mode: " + mode);
    return 0;
}
