#include "common.h"

using namespace std;

namespace {

void greedyTrap() {
    Instance in(5);
    in.add(2, 3, 0);
    in.add(4, 5, 0);
    in.add(1, 2, 1);
    in.add(1, 4, 2);
    in.add(3, 2, 2);
    in.add(3, 4, 100);
    in.print();
}

void orderedMatching(int n) {
    if (n < 4) Instance::fail("ordered mode needs n >= 4");
    Instance in(n);
    const int side = n - 1;
    for (int x = 0; x < side; ++x)
        in.add(x + 1, x + 2, 100);
    for (int x = 0; x < side; ++x)
        in.add(x + 1, (x + 1) % side + 2, 1);
    in.print();
}

void noPotentialTrap() {
    Instance in(4);
    in.add(1, 2, 6);
    in.add(1, 3, 7);
    in.add(1, 4, 4);
    in.add(2, 3, 5);
    in.add(2, 4, 2);
    in.add(3, 2, 3);
    in.add(3, 4, 0);
    in.print();
}

void irrelevantTrap(int n) {
    Instance in(n);
    for (int v = 1; v < n; ++v)
        in.add(v, v + 1, 1);
    in.add(n, 1, 100000);
    in.print();
}

void directionTrap() {
    Instance in(4);
    in.add(1, 2, 100);
    in.add(2, 3, 100);
    in.add(3, 4, 100);
    in.add(2, 1, 1);
    in.add(3, 2, 1);
    in.add(4, 3, 1);
    in.print();
}

void shortestPathTrap() {
    Instance in(4);
    in.add(1, 4, 1);
    in.add(2, 3, 100);
    in.add(3, 2, 100);
    in.print();
}

void binaryExtensionTrap() {
    Instance in(4);
    in.add(1, 2, 0);
    in.add(2, 3, 0);
    in.add(1, 4, 1);
    in.add(3, 2, 1);
    in.print();
}

void hallTrap() {
    Instance in(4);
    in.add(1, 2, 0);
    in.add(1, 3, 0);
    in.add(2, 4, 0);
    in.add(3, 4, 0);
    in.print();
}

void annealingTrap(int n) {
    if (n < 4) Instance::fail("annealing mode needs n >= 4");
    const int side = n - 1;
    Instance in(n);

    // The first side edges form an obvious feasible matching.  The zero-cost
    // edges form one long cycle and are globally optimal.  Moving from the
    // first matching to the second requires changing the entire cycle at once:
    // no swap of two assigned columns is even feasible.
    for (int row = 0; row < side; ++row)
        in.add(row + 1, row + 2, 1);
    for (int row = 0; row < side; ++row)
        in.add(row + 1, (row + 1) % side + 2, 0);
    in.print();
}

}  // namespace

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    requireArguments(argc, 2, "attack MODE [N]");
    const string mode = argv[1];
    if (mode == "greedy") greedyTrap();
    else if (mode == "ordered") {
        requireArguments(argc, 3, "attack ordered N");
        orderedMatching(parseInt(argv[2], "N"));
    } else if (mode == "no-potential") noPotentialTrap();
    else if (mode == "irrelevant") {
        requireArguments(argc, 3, "attack irrelevant N");
        irrelevantTrap(parseInt(argv[2], "N"));
    } else if (mode == "direction") directionTrap();
    else if (mode == "shortest-path") shortestPathTrap();
    else if (mode == "binary-extension") binaryExtensionTrap();
    else if (mode == "hall") hallTrap();
    else if (mode == "annealing") {
        requireArguments(argc, 3, "attack annealing N");
        annealingTrap(parseInt(argv[2], "N"));
    }
    else Instance::fail("unknown attack mode: " + mode);
    return 0;
}
