#include "Can_You_Blow_My_Whistle.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace {

std::ofstream* managerOutput = nullptr;

}  // namespace

void swap_student(int u, int v) {
    *managerOutput << "SWAP " << u << ' ' << v << std::endl;
}

void blow_whistle() {
    *managerOutput << "BLOW" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        return 1;
    }

    // TPS passes FIFO paths as arguments.  Open them in the same order in
    // which the manager opens the opposite endpoints to avoid a startup
    // deadlock.
    std::ifstream fromManager(argv[1]);
    if (!fromManager) {
        return 1;
    }
    std::ofstream toManager(argv[2]);
    if (!toManager) {
        return 1;
    }
    managerOutput = &toManager;

    int n;
    if (!(fromManager >> n)) {
        return 1;
    }

    std::vector<int> permutation(n);
    for (int& value : permutation) {
        if (!(fromManager >> value)) {
            return 1;
        }
    }

    solve(n, permutation);
    toManager << "DONE" << std::endl;
    return 0;
}
