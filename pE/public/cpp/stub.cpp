// TOOD: Remove offical solution from the public file
#include "Can_You_Blow_My_Whistle.h"

#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace {

int studentCount;
int whistleCount;
vector<int> permutation;
vector<unsigned char> used;
vector<pair<int, int> > pending;

[[noreturn]] void wrongAnswer(const string& message) {
    cerr << "Wrong Answer: " << message << '\n';
    exit(1);
}

int optimalWhistles(const vector<int>& initial) {
    vector<unsigned char> visited(studentCount + 1, false);
    int optimum = 0;
    for (int start = 1; start <= studentCount; ++start) {
        if (visited[start]) continue;
        int length = 0;
        int vertex = start;
        while (!visited[vertex]) {
            visited[vertex] = true;
            vertex = initial[vertex];
            ++length;
        }
        if (length == 2) optimum = max(optimum, 1);
        if (length >= 3) optimum = 2;
    }
    return optimum;
}

}  // namespace

void swap_student(int u, int v) {
    if (u < 1 || u > studentCount || v < 1 || v > studentCount || u == v) {
        wrongAnswer("invalid swap_student arguments");
    }
    if (used[u] || used[v]) {
        wrongAnswer("a position is used twice in one round");
    }
    used[u] = used[v] = true;
    pending.push_back({u, v});
}

void blow_whistle() {
    ++whistleCount;
    if (whistleCount > 2 * studentCount) {
        wrongAnswer("blow_whistle was called more than 2*n times");
    }
    for (const auto& operation : pending) {
        swap(permutation[operation.first], permutation[operation.second]);
    }
    pending.clear();
    fill(used.begin(), used.end(), false);
}

int main() {
    if (!(cin >> studentCount) || studentCount < 1 || studentCount > 100000) {
        cerr << "Invalid local test input\n";
        return 1;
    }

    vector<int> initial(studentCount + 1);
    vector<int> frequency(studentCount + 1, 0);
    for (int position = 1; position <= studentCount; ++position) {
        if (!(cin >> initial[position]) ||
            initial[position] < 1 || initial[position] > studentCount) {
            cerr << "Invalid local test input\n";
            return 1;
        }
        ++frequency[initial[position]];
    }
    for (int value = 1; value <= studentCount; ++value) {
        if (frequency[value] != 1) {
            cerr << "Input is not a permutation\n";
            return 1;
        }
    }

    permutation = initial;
    used.assign(studentCount + 1, false);
    vector<int> argument(initial.begin() + 1, initial.end());
    solve(studentCount, argument);

    for (int position = 1; position <= studentCount; ++position) {
        if (permutation[position] != position) {
            wrongAnswer("the final permutation is not sorted");
        }
    }

    const int optimum = optimalWhistles(initial);
    double multiplier = 1.0;
    if (whistleCount > optimum) {
        multiplier = max(0.0, 0.5 - 0.02 * (whistleCount - optimum));
    }

    cout << "Sorted successfully\n";
    cout << "Whistles: " << whistleCount << '\n';
    cout << "Optimal: " << optimum << '\n';
    cout << fixed << setprecision(2)
         << "Score multiplier: " << multiplier << '\n';
    return 0;
}
