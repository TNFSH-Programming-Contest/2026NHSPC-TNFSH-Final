#include <bits/stdc++.h>
#include "testlib.h"

using namespace std;

namespace {

const int MAX_A = 1000000000;

template <class T>
void randomShuffle(vector<T>& values) {
    for (int i = static_cast<int>(values.size()) - 1; i > 0; --i) {
        swap(values[i], values[rnd.next(0, i)]);
    }
}

vector<int> buildTree(const string& shape, int n) {
    vector<int> parent(n + 1, 0);
    if (n == 1) {
        return parent;
    }

    if (shape == "single" || shape == "path") {
        for (int v = 2; v <= n; ++v) {
            parent[v] = v - 1;
        }
    } else if (shape == "star") {
        for (int v = 2; v <= n; ++v) {
            parent[v] = 1;
        }
    } else if (shape == "binary") {
        for (int v = 2; v <= n; ++v) {
            parent[v] = v / 2;
        }
    } else if (shape == "broom") {
        const int spine = max(2, n / 2);
        for (int v = 2; v <= spine; ++v) {
            parent[v] = v - 1;
        }
        for (int v = spine + 1; v <= n; ++v) {
            parent[v] = spine;
        }
    } else if (shape == "caterpillar") {
        const int spine = max(1, (n + 1) / 2);
        for (int v = 2; v <= spine; ++v) {
            parent[v] = v - 1;
        }
        for (int v = spine + 1; v <= n; ++v) {
            parent[v] = rnd.next(1, spine);
        }
    } else if (shape == "depth2") {
        const int children = min(n - 1, max(1, n / 3));
        for (int v = 2; v <= children + 1; ++v) {
            parent[v] = 1;
        }
        for (int v = children + 2; v <= n; ++v) {
            parent[v] = rnd.next(2, children + 1);
        }
    } else if (shape == "deep-star") {
        const int spine = max(2, (2 * n) / 3);
        for (int v = 2; v <= spine; ++v) {
            parent[v] = v - 1;
        }
        for (int v = spine + 1; v <= n; ++v) {
            parent[v] = spine;
        }
    } else if (shape == "random") {
        for (int v = 2; v <= n; ++v) {
            parent[v] = rnd.next(1, v - 1);
        }
    } else {
        quitf(_fail, "unknown tree shape: %s", shape.c_str());
    }

    return parent;
}

vector<unsigned char> getDepthParity(const vector<int>& parent) {
    const int n = static_cast<int>(parent.size()) - 1;
    vector<unsigned char> parity(n + 1, 0);
    for (int v = 2; v <= n; ++v) {
        parity[v] = parity[parent[v]] ^ 1;
    }
    return parity;
}

vector<int> buildStones(
        const string& mode,
        const vector<int>& parent,
        const vector<unsigned char>& parity) {
    const int n = static_cast<int>(parent.size()) - 1;
    vector<int> stones(n + 1, 0);
    vector<int> oddVertices;
    vector<int> evenNonRoot;
    vector<int> childCount(n + 1, 0);

    for (int v = 2; v <= n; ++v) {
        ++childCount[parent[v]];
        if (parity[v]) {
            oddVertices.push_back(v);
        } else {
            evenNonRoot.push_back(v);
        }
    }

    if (mode == "zero") {
        return stones;
    }
    if (mode == "root-max") {
        stones[1] = MAX_A;
        return stones;
    }
    if (mode == "ones") {
        fill(stones.begin() + 1, stones.end(), 1);
        return stones;
    }
    if (mode == "max") {
        fill(stones.begin() + 1, stones.end(), MAX_A);
        return stones;
    }
    if (mode == "binary-random") {
        for (int v = 1; v <= n; ++v) {
            stones[v] = rnd.next(0, 1);
        }
        return stones;
    }
    if (mode == "binary-even") {
        stones[1] = 1;
        for (int v : evenNonRoot) {
            stones[v] = 1;
        }
        return stones;
    }
    if (mode == "binary-pair") {
        if (oddVertices.size() >= 2) {
            stones[oddVertices.front()] = 1;
            stones[oddVertices.back()] = 1;
        } else if (!oddVertices.empty()) {
            stones[oddVertices.front()] = 1;
        }
        return stones;
    }
    if (mode == "binary-nonzero") {
        if (!oddVertices.empty()) {
            stones[oddVertices[rnd.next(0, static_cast<int>(oddVertices.size()) - 1)]] = 1;
        }
        return stones;
    }
    if (mode == "single-odd-two") {
        if (!oddVertices.empty()) {
            stones[oddVertices.back()] = 2;
        }
        return stones;
    }
    if (mode == "single-even-max") {
        if (!evenNonRoot.empty()) {
            stones[evenNonRoot.back()] = MAX_A;
        } else {
            stones[1] = MAX_A;
        }
        return stones;
    }
    if (mode == "odd-pair-max") {
        if (oddVertices.size() >= 2) {
            stones[oddVertices.front()] = MAX_A;
            stones[oddVertices.back()] = MAX_A;
        } else if (!oddVertices.empty()) {
            stones[oddVertices.front()] = MAX_A;
        }
        return stones;
    }
    if (mode == "even-only") {
        stones[1] = 123456789;
        for (int v : evenNonRoot) {
            stones[v] = rnd.next(1, MAX_A);
        }
        return stones;
    }
    if (mode == "internal-odd") {
        for (int v : oddVertices) {
            if (childCount[v] > 0) {
                stones[v] = 987654321;
                return stones;
            }
        }
        if (!oddVertices.empty()) {
            stones[oddVertices.front()] = 987654321;
        }
        return stones;
    }
    if (mode == "deep-odd") {
        if (!oddVertices.empty()) {
            stones[oddVertices.back()] = 536870911;
        }
        return stones;
    }
    if (mode == "xor-zero") {
        stones[1] = rnd.next(0, MAX_A);
        for (int v : evenNonRoot) {
            stones[v] = rnd.next(0, MAX_A);
        }
        int xorSum = 0;
        for (size_t i = 0; i + 1 < oddVertices.size(); ++i) {
            stones[oddVertices[i]] = rnd.next(0, (1 << 29) - 1);
            xorSum ^= stones[oddVertices[i]];
        }
        if (!oddVertices.empty()) {
            stones[oddVertices.back()] = xorSum;
        }
        return stones;
    }
    if (mode == "xor-nonzero") {
        int xorSum = 0;
        for (int v = 1; v <= n; ++v) {
            stones[v] = rnd.next(0, MAX_A);
            if (parity[v]) {
                xorSum ^= stones[v];
            }
        }
        if (xorSum == 0 && !oddVertices.empty()) {
            stones[oddVertices.back()] ^= 1;
        }
        return stones;
    }
    if (mode == "powers") {
        for (size_t i = 0; i < oddVertices.size(); ++i) {
            stones[oddVertices[i]] = 1 << (i % 29);
        }
        for (int v : evenNonRoot) {
            stones[v] = MAX_A;
        }
        return stones;
    }
    if (mode == "overflow") {
        stones[1] = MAX_A;
        for (size_t i = 0; i < oddVertices.size(); ++i) {
            stones[oddVertices[i]] = MAX_A - static_cast<int>(i % 7);
        }
        for (int v : evenNonRoot) {
            stones[v] = MAX_A - 17;
        }
        return stones;
    }
    if (mode == "small-random") {
        for (int v = 1; v <= n; ++v) {
            stones[v] = rnd.next(0, 7);
        }
        return stones;
    }
    if (mode == "playout-zero" || mode == "playout-nonzero") {
        // Nontrivial paired Nim heaps make the exact xor zero, but random
        // playouts see an essentially balanced win rate.  The second mode
        // changes one low bit, so a heuristic must distinguish two almost
        // identical-looking positions exactly.
        const size_t paired = oddVertices.size() / 2 * 2;
        for (size_t i = 0; i < paired; i += 2) {
            const int value = 2 + static_cast<int>((i / 2) % 29);
            stones[oddVertices[i]] = value;
            stones[oddVertices[i + 1]] = value;
        }
        if (mode == "playout-nonzero" && paired > 0)
            stones[oddVertices[0]] ^= 1;
        return stones;
    }
    if (mode == "random") {
        for (int v = 1; v <= n; ++v) {
            stones[v] = rnd.next(0, MAX_A);
        }
        return stones;
    }

    quitf(_fail, "unknown stone mode: %s", mode.c_str());
    return stones;
}

}  // namespace

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    ensuref(argc >= 6,
            "usage: gen SHAPE N STONES LABEL_MODE EDGE_MODE");

    const string shape = argv[1];
    const int n = atoi(argv[2]);
    const string stoneMode = argv[3];
    const string labelMode = argv[4];
    const string edgeMode = argv[5];

    ensuref(1 <= n && n <= 200000, "N is out of range: %d", n);
    ensuref(labelMode == "keep" || labelMode == "relabel",
            "unknown label mode: %s", labelMode.c_str());
    ensuref(edgeMode == "ordered" || edgeMode == "reverse" ||
                edgeMode == "shuffled" || edgeMode == "mixed",
            "unknown edge mode: %s", edgeMode.c_str());

    const vector<int> parent = buildTree(shape, n);
    const vector<unsigned char> parity = getDepthParity(parent);
    const vector<int> logicalStones =
        buildStones(stoneMode, parent, parity);

    vector<int> label(n + 1);
    iota(label.begin(), label.end(), 0);
    if (labelMode == "relabel" && n >= 2) {
        vector<int> shuffledLabels;
        for (int v = 2; v <= n; ++v) {
            shuffledLabels.push_back(v);
        }
        randomShuffle(shuffledLabels);
        for (int v = 2; v <= n; ++v) {
            label[v] = shuffledLabels[v - 2];
        }
    }

    vector<int> stones(n + 1, 0);
    vector<pair<int, int> > edges;
    edges.reserve(max(0, n - 1));
    for (int v = 1; v <= n; ++v) {
        stones[label[v]] = logicalStones[v];
    }
    for (int v = 2; v <= n; ++v) {
        edges.push_back(make_pair(label[parent[v]], label[v]));
    }

    if (edgeMode == "reverse") {
        for (pair<int, int>& edge : edges) {
            swap(edge.first, edge.second);
        }
    } else if (edgeMode == "shuffled" || edgeMode == "mixed") {
        randomShuffle(edges);
        if (edgeMode == "mixed") {
            for (pair<int, int>& edge : edges) {
                if (rnd.next(0, 1)) {
                    swap(edge.first, edge.second);
                }
            }
        }
    }

    cout << n << '\n';
    for (int v = 1; v <= n; ++v) {
        if (v > 1) {
            cout << ' ';
        }
        cout << stones[v];
    }
    cout << '\n';
    for (const pair<int, int>& edge : edges) {
        cout << edge.first << ' ' << edge.second << '\n';
    }

    return 0;
}
