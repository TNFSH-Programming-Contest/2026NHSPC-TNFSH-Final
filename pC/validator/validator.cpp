#include "testlib.h"

#include <algorithm>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace {

const int MAX_N = 200000;
const int MAX_A = 1000000000;

struct Dsu {
    vector<int> parent;
    vector<int> size;

    explicit Dsu(int n) : parent(n + 1), size(n + 1, 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        while (x != parent[x]) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return false;
        }
        if (size[a] < size[b]) {
            swap(a, b);
        }
        parent[b] = a;
        size[a] += size[b];
        return true;
    }
};

struct Input {
    int n;
    vector<int> stones;
    vector<pair<int, int> > edges;
    vector<int> degree;
};

Input readInput() {
    Input input;
    input.n = inf.readInt(1, MAX_N, "N");
    inf.readEoln();

    input.stones.resize(input.n + 1);
    for (int i = 1; i <= input.n; ++i) {
        input.stones[i] = inf.readInt(0, MAX_A, format("A[%d]", i));
        if (i == input.n) {
            inf.readEoln();
        } else {
            inf.readSpace();
        }
    }

    input.degree.assign(input.n + 1, 0);
    input.edges.reserve(max(0, input.n - 1));
    Dsu dsu(input.n);

    for (int i = 1; i < input.n; ++i) {
        const int u = inf.readInt(1, input.n, format("u[%d]", i));
        inf.readSpace();
        const int v = inf.readInt(1, input.n, format("v[%d]", i));
        inf.readEoln();

        ensuref(u != v, "edge %d is a self-loop at vertex %d", i, u);
        ensuref(dsu.unite(u, v),
                "edge %d (%d, %d) creates a cycle or repeats an existing edge",
                i, u, v);

        input.edges.push_back(make_pair(u, v));
        ++input.degree[u];
        ++input.degree[v];
    }
    inf.readEof();

    for (int v = 2; v <= input.n; ++v) {
        ensuref(dsu.find(v) == dsu.find(1),
                "vertex %d is not connected to root vertex 1", v);
    }

    return input;
}

void validateSingle(const Input& input) {
    ensuref(input.n <= 2, "single subtask requires N <= 2, but N = %d", input.n);
}

void validateStar(const Input& input) {
    ensuref(input.degree[1] == input.n - 1,
            "star subtask requires degree(1) = N - 1, but degree(1) = %d",
            input.degree[1]);
}

void validatePath(const Input& input) {
    for (int i = 0; i + 1 < input.n; ++i) {
        const pair<int, int> expected = make_pair(i + 1, i + 2);
        const pair<int, int> actual =
            minmax(input.edges[i].first, input.edges[i].second);
        ensuref(actual == expected,
                "path subtask requires edge %d to connect %d and %d, but it is (%d, %d)",
                i + 1, expected.first, expected.second,
                input.edges[i].first, input.edges[i].second);
    }
}

void validateBinary(const Input& input) {
    for (int v = 1; v <= input.n; ++v) {
        ensuref(input.stones[v] <= 1,
                "binary subtask requires A[%d] <= 1, but A[%d] = %d",
                v, v, input.stones[v]);
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    const string mode = argc >= 2 ? argv[1] : "full";
    ensuref(mode == "full" || mode == "single" || mode == "star" ||
                mode == "path" || mode == "binary",
            "unknown validator mode: %s", mode.c_str());

    const Input input = readInput();
    if (mode == "single") {
        validateSingle(input);
    } else if (mode == "star") {
        validateStar(input);
    } else if (mode == "path") {
        validatePath(input);
    } else if (mode == "binary") {
        validateBinary(input);
    }

    return 0;
}
