#include <bits/stdc++.h>
#include "testlib.h"

using namespace std;

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    if (argc < 3) {
        cerr << "usage: gen MODE N [seed-tag]\n";
        return 1;
    }

    const string mode = argv[1];
    const int n = atoi(argv[2]);
    if (n < 1 || n > 100000) {
        cerr << "n is out of range\n";
        return 1;
    }

    vector<int> permutation(n);
    iota(permutation.begin(), permutation.end(), 1);

    if (mode == "identity") {
        // Already sorted.
    } else if (mode == "transpositions") {
        if (n < 2) {
            cerr << "transpositions mode requires n >= 2\n";
            return 1;
        }
        for (int i = 0; i + 1 < n; i += 2) {
            swap(permutation[i], permutation[i + 1]);
        }
    } else if (mode == "cycle") {
        if (n >= 2) {
            rotate(permutation.begin(), permutation.begin() + 1,
                   permutation.end());
        }
    } else {
        cerr << "unknown mode: " << mode << '\n';
        return 1;
    }

    cout << n << '\n';
    for (int i = 0; i < n; ++i) {
        if (i != 0) cout << ' ';
        cout << permutation[i];
    }
    cout << '\n';
    return 0;
}
