#include "Can_You_Blow_My_Whistle.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace whistle_stub {

FILE* from_manager = nullptr;
FILE* to_manager = nullptr;
int student_count = 0;
int whistle_count = 0;
std::vector<unsigned char> used_in_round;

[[noreturn]] void stop_with_error(int error_code) {
    std::fprintf(to_manager, "E %d\n", error_code);
    std::fflush(to_manager);
    std::exit(0);
}

void send_command(const char* format, int first = 0, int second = 0) {
    if (std::fprintf(to_manager, format, first, second) < 0 ||
        std::fflush(to_manager) != 0) {
        std::cerr << "grader error: cannot write to manager" << std::endl;
        std::exit(0);
    }
}

}  // namespace whistle_stub

void swap_student(int u, int v) {
    using namespace whistle_stub;

    if (u < 1 || u > student_count || v < 1 || v > student_count || u == v) {
        stop_with_error(3);
    }
    if (used_in_round[u] || used_in_round[v]) {
        stop_with_error(1);
    }

    used_in_round[u] = used_in_round[v] = true;
    send_command("S %d %d\n", u, v);
}

void blow_whistle() {
    using namespace whistle_stub;

    ++whistle_count;
    if (whistle_count > 2 * student_count) {
        stop_with_error(2);
    }

    send_command("B\n");
    std::fill(used_in_round.begin(), used_in_round.end(), false);
}

int main(int argc, char* argv[]) {
    using namespace whistle_stub;

    // Old TPS gives two FIFO arguments.  The current runner additionally
    // passes the solution-process index as argv[3].
    if (argc != 3 && argc != 4) {
        std::cerr << "grader error: unexpected number of arguments" << std::endl;
        return 0;
    }

    // The manager opens its write end first, so the stub opens its matching
    // read end first.  Reversing both sides would deadlock on the FIFOs.
    from_manager = std::fopen(argv[1], "r");
    if (from_manager == nullptr) {
        std::cerr << "grader error: cannot open manager-to-solution FIFO"
                  << std::endl;
        return 0;
    }
    to_manager = std::fopen(argv[2], "a");
    if (to_manager == nullptr) {
        std::cerr << "grader error: cannot open solution-to-manager FIFO"
                  << std::endl;
        return 0;
    }

    if (std::fscanf(from_manager, "%d", &student_count) != 1 ||
        student_count < 1) {
        std::cerr << "grader error: cannot read n" << std::endl;
        return 0;
    }

    std::vector<int> permutation(student_count);
    for (int& value : permutation) {
        if (std::fscanf(from_manager, "%d", &value) != 1) {
            std::cerr << "grader error: cannot read permutation" << std::endl;
            return 0;
        }
    }
    used_in_round.assign(student_count + 1, false);

    solve(student_count, permutation);
    send_command("D\n");
    return 0;
}
