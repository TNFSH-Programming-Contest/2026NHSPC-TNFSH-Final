#include "testlib.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

namespace {

const int MAX_N = 100000;

int parseInt(const char* text, int low, int high, const string& name) {
    char* end = nullptr;
    const long long value = strtoll(text, &end, 10);
    if (*text == '\0' || *end != '\0' || value < low || value > high) {
        cerr << name << " must be in [" << low << ", " << high << "]\n";
        exit(1);
    }
    return static_cast<int>(value);
}

vector<int> vertexOrder(int n, const string& style) {
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);

    if (style == "identity") {
        return order;
    }
    if (style == "reverse") {
        reverse(order.begin(), order.end());
        return order;
    }
    if (style == "random") {
        shuffle(order.begin(), order.end());
        return order;
    }
    if (style == "alternating") {
        vector<int> result;
        result.reserve(n);
        int left = 0;
        int right = n - 1;
        while (left <= right) {
            result.push_back(left++);
            if (left <= right) {
                result.push_back(right--);
            }
        }
        return result;
    }

    cerr << "unknown vertex-order style: " << style << '\n';
    exit(1);
}

void addCycle(vector<int>& permutation, const vector<int>& vertices,
              int begin, int length) {
    if (length <= 1) {
        return;
    }
    for (int i = 0; i < length; ++i) {
        const int from = vertices[begin + i];
        const int to = vertices[begin + (i + 1) % length];
        permutation[from] = to + 1;
    }
}

bool isInvolution(const vector<int>& permutation) {
    for (int i = 0; i < static_cast<int>(permutation.size()); ++i) {
        const int to = permutation[i] - 1;
        if (permutation[to] != i + 1) {
            return false;
        }
    }
    return true;
}

void ensurePermutation(const vector<int>& permutation) {
    const int n = static_cast<int>(permutation.size());
    vector<int> frequency(n + 1, 0);
    for (int value : permutation) {
        if (value < 1 || value > n) {
            cerr << "internal error: value out of range\n";
            exit(1);
        }
        ++frequency[value];
    }
    for (int value = 1; value <= n; ++value) {
        if (frequency[value] != 1) {
            cerr << "internal error: output is not a permutation\n";
            exit(1);
        }
    }
}

vector<int> makeMixed(int n, const string& labelStyle,
                      const string& lengthStyle) {
    vector<int> permutation(n);
    iota(permutation.begin(), permutation.end(), 1);
    const vector<int> order = vertexOrder(n, labelStyle);

    vector<int> pattern;
    if (lengthStyle == "small") {
        pattern = {1, 2, 3, 4, 5, 7};
    } else if (lengthStyle == "odd") {
        pattern = {1, 3, 5, 7, 9, 11};
    } else if (lengthStyle == "even") {
        pattern = {2, 4, 6, 8, 10, 12};
    } else if (lengthStyle == "stair") {
        for (int length = 1; length <= 64; ++length) {
            pattern.push_back(length);
        }
    } else if (lengthStyle == "long") {
        const int first = max(3, n / 2);
        addCycle(permutation, order, 0, first);
        int used = first;
        if (n - used >= 3) {
            const int second = max(3, (n - used) / 2);
            addCycle(permutation, order, used, second);
            used += second;
        }
        while (n - used >= 3) {
            const int length = min(7, n - used);
            addCycle(permutation, order, used, length);
            used += length;
        }
        if (n - used == 2) {
            addCycle(permutation, order, used, 2);
        }
        return permutation;
    } else {
        cerr << "unknown mixed length style: " << lengthStyle << '\n';
        exit(1);
    }

    int used = 0;
    int patternIndex = 0;
    while (used < n) {
        int length = pattern[patternIndex++ % pattern.size()];
        length = min(length, n - used);
        addCycle(permutation, order, used, length);
        used += length;
    }
    return permutation;
}

}  // namespace

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    if (argc < 3) {
        cerr << "usage: gen MODE N [mode arguments] [seed-tag]\n";
        return 1;
    }

    const string mode = argv[1];
    const int n = parseInt(argv[2], 1, MAX_N, "n");
    vector<int> permutation(n);
    iota(permutation.begin(), permutation.end(), 1);

    if (mode == "identity") {
        // Already sorted.
    } else if (mode == "involution") {
        if (argc < 5) {
            cerr << "usage: gen involution N STYLE PAIRS [seed-tag]\n";
            return 1;
        }
        const string style = argv[3];
        const int pairs = parseInt(argv[4], 1, n / 2, "pairs");
        const vector<int> order = vertexOrder(n, style);
        for (int i = 0; i < pairs; ++i) {
            swap(permutation[order[2 * i]], permutation[order[2 * i + 1]]);
        }
    } else if (mode == "single-cycle") {
        if (argc < 4 || n < 2) {
            cerr << "usage: gen single-cycle N STYLE [seed-tag], N >= 2\n";
            return 1;
        }
        const vector<int> order = vertexOrder(n, argv[3]);
        addCycle(permutation, order, 0, n);
    } else if (mode == "equal-cycles") {
        if (argc < 5) {
            cerr << "usage: gen equal-cycles N LENGTH STYLE [seed-tag]\n";
            return 1;
        }
        const int length = parseInt(argv[3], 2, n, "cycle length");
        const vector<int> order = vertexOrder(n, argv[4]);
        int used = 0;
        while (used + length <= n) {
            addCycle(permutation, order, used, length);
            used += length;
        }
        if (n - used >= 2) {
            addCycle(permutation, order, used, n - used);
        }
    } else if (mode == "rotation") {
        if (argc < 5 || n < 2) {
            cerr << "usage: gen rotation N SHIFT STYLE [seed-tag], N >= 2\n";
            return 1;
        }
        const int shift = parseInt(argv[3], 1, n - 1, "shift");
        const vector<int> order = vertexOrder(n, argv[4]);
        for (int i = 0; i < n; ++i) {
            permutation[order[i]] = order[(i + shift) % n] + 1;
        }
    } else if (mode == "near-involution") {
        if (argc < 4 || n < 3) {
            cerr << "usage: gen near-involution N STYLE [seed-tag], N >= 3\n";
            return 1;
        }
        const vector<int> order = vertexOrder(n, argv[3]);
        addCycle(permutation, order, 0, 3);
        for (int i = 3; i + 1 < n; i += 2) {
            swap(permutation[order[i]], permutation[order[i + 1]]);
        }
    } else if (mode == "sparse-cycle") {
        if (argc < 5) {
            cerr << "usage: gen sparse-cycle N LENGTH STYLE [seed-tag]\n";
            return 1;
        }
        const int length = parseInt(argv[3], 2, n, "cycle length");
        const vector<int> order = vertexOrder(n, argv[4]);
        addCycle(permutation, order, 0, length);
    } else if (mode == "mixed") {
        if (argc < 5) {
            cerr << "usage: gen mixed N LABEL_STYLE LENGTH_STYLE [seed-tag]\n";
            return 1;
        }
        permutation = makeMixed(n, argv[3], argv[4]);
    } else if (mode == "random") {
        if (n < 3) {
            cerr << "random mode requires n >= 3\n";
            return 1;
        }
        do {
            iota(permutation.begin(), permutation.end(), 1);
            shuffle(permutation.begin(), permutation.end());
        } while (isInvolution(permutation));
    } else {
        cerr << "unknown mode: " << mode << '\n';
        return 1;
    }

    ensurePermutation(permutation);
    cout << n << '\n';
    for (int i = 0; i < n; ++i) {
        if (i != 0) cout << ' ';
        cout << permutation[i];
    }
    cout << '\n';
    return 0;
}
