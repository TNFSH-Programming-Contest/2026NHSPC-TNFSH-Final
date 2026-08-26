#include "common.h"

using namespace std;

namespace {

constexpr int BASE_COST = 50000;

// Match left i to right i.  Once this edge carries flow, an edge i -> j
// with cost BASE_COST + w behaves like a residual graph edge of weight w.
void addDiagonal(Instance& in, int side, int skipped = -1) {
    for (int i = 0; i < side; ++i) {
        if (i != skipped) in.add(i + 1, i + 2, BASE_COST);
    }
}

bool addResidualEdge(Instance& in, int from, int to, int weight) {
    if (weight < -BASE_COST || weight > MAX_COST - BASE_COST)
        Instance::fail("residual edge weight is out of range");
    return in.addUnique(from + 1, to + 2, BASE_COST + weight);
}

void generateClassic(int n, int core_size, const string& order) {
    if (n < 8) Instance::fail("classic mode needs n >= 8");
    const int side = n - 1;
    if (core_size < 2 || core_size >= side)
        Instance::fail("classic core must satisfy 2 <= core < n-1");

    Instance in(n);
    const auto nextCore = [&](int x) { return (x + 1) % core_size; };

    for (int x = 0; x < core_size; ++x)
        in.add(x + 1, nextCore(x) + 2, 0);
    for (int x = 1; x < core_size; ++x)
        in.add(1, nextCore(x) + 2, 2 * (core_size - x));
    for (int x = 1; x < core_size; ++x)
        in.add(x + 1, nextCore(x - 1) + 2, 1);
    for (int x = core_size; x < side; ++x) {
        in.add(x + 1, x + 2, 60000);
        in.add(x + 1, nextCore(0) + 2, 50000);
    }
    in.print(order);
}

void generateGrid(int n, int rows, int columns) {
    if (n < 8) Instance::fail("grid mode needs n >= 8");
    const int side = n - 1;
    const int cells = rows * columns;
    if (rows < 2 || columns < 2 || cells + 1 > side)
        Instance::fail("grid dimensions do not fit");

    const int root = side - 1;
    Instance in(n);
    addDiagonal(in, side, root);

    vector<int> slot(cells);
    iota(slot.begin(), slot.end(), 0);
    jngen::shuffle(slot.begin(), slot.end());
    const auto id = [&](int r, int c) { return r * columns + c; };
    const auto edge = [&](int a, int b, int w) {
        addResidualEdge(in, slot[a], slot[b], w);
    };

    // The final augmentation enters a general shortest-path instance through
    // an already matched column.  Small vertical edges and much larger random
    // horizontal/diagonal edges create many successively better paths.
    addResidualEdge(in, root, slot[id(0, 0)], 0);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (r + 1 < rows) {
                edge(id(r, c), id(r + 1, c), 1);
                edge(id(r + 1, c), id(r, c), 1);
                if (c + 1 < columns)
                    edge(id(r, c), id(r + 1, c + 1), rnd.next(10, 30000));
            }
            if (c + 1 < columns) {
                edge(id(r, c), id(r, c + 1), rnd.next(10, 30000));
                edge(id(r, c + 1), id(r, c), rnd.next(10, 30000));
            }
        }
    }
    in.add(slot[id(rows - 1, columns - 1)] + 1, root + 2, BASE_COST);
    in.print("shuffle");
}

void generateFlower(int n, int chain_size) {
    if (n < 8) Instance::fail("flower mode needs n >= 8");
    const int side = n - 1;
    if (chain_size < 2 || chain_size + 3 > side)
        Instance::fail("flower chain does not fit");

    const int root = side - 1;
    const int hub = chain_size;
    const int first_petal = hub + 1;
    const int petal_count = root - first_petal;
    Instance in(n);
    addDiagonal(in, side, root);

    addResidualEdge(in, root, 0, 0);
    for (int i = 0; i + 1 < chain_size; ++i) {
        addResidualEdge(in, i, i + 1, -1);
        addResidualEdge(in, i, hub, 0);
    }
    addResidualEdge(in, chain_size - 1, hub, 0);
    for (int i = 0; i < petal_count; ++i) {
        const int petal = first_petal + i;
        addResidualEdge(in, hub, petal, 0);
        in.add(petal + 1, root + 2, BASE_COST + i % 7);
    }
    in.print();
}

void generateNearTree(int n, int core_size, int target_roads) {
    if (n < 8) Instance::fail("tree mode needs n >= 8");
    const int side = n - 1;
    if (core_size < 2 || core_size + 1 > side)
        Instance::fail("tree core does not fit");
    if (target_roads < side + core_size || target_roads > MAX_M)
        Instance::fail("tree road target is out of range");

    const int root = side - 1;
    Instance in(n);
    addDiagonal(in, side, root);

    vector<int> slot(core_size);
    iota(slot.begin(), slot.end(), 0);
    jngen::shuffle(slot.begin(), slot.end());
    vector<int> depth(core_size, 0);

    addResidualEdge(in, root, slot[0], 0);
    for (int v = 1; v < core_size; ++v) {
        const int parent = rnd.next(max(0, v - 5), v - 1);
        const int weight = rnd.next(1, 20);
        depth[v] = depth[parent] + weight;
        addResidualEdge(in, slot[parent], slot[v], weight);
    }

    int attempts = 0;
    while (static_cast<int>(in.roads.size()) + 1 < target_roads &&
           attempts < 10000000) {
        ++attempts;
        const int from = rnd.next(0, core_size - 1);
        const int to = rnd.next(0, core_size - 1);
        if (from == to) continue;
        const int weight = depth[to] - depth[from] + rnd.next(0, 5);
        addResidualEdge(in, slot[from], slot[to], weight);
    }
    if (static_cast<int>(in.roads.size()) + 1 != target_roads)
        Instance::fail("could not reach the requested tree road count");

    in.add(slot[core_size - 1] + 1, root + 2, BASE_COST);
    in.print("shuffle");
}

}  // namespace

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    config.generateLargeObjects = true;
    requireArguments(argc, 2, "hackspfa MODE ...");

    const string mode = argv[1];
    if (mode == "classic") {
        requireArguments(argc, 4, "hackspfa classic N CORE");
        generateClassic(parseInt(argv[2], "N"), parseInt(argv[3], "CORE"), "keep");
    } else if (mode == "classic-shuffle") {
        requireArguments(argc, 4, "hackspfa classic-shuffle N CORE");
        generateClassic(parseInt(argv[2], "N"), parseInt(argv[3], "CORE"), "shuffle");
    } else if (mode == "grid") {
        requireArguments(argc, 5, "hackspfa grid N ROWS COLUMNS");
        generateGrid(parseInt(argv[2], "N"), parseInt(argv[3], "ROWS"),
                     parseInt(argv[4], "COLUMNS"));
    } else if (mode == "flower") {
        requireArguments(argc, 4, "hackspfa flower N CHAIN");
        generateFlower(parseInt(argv[2], "N"), parseInt(argv[3], "CHAIN"));
    } else if (mode == "tree") {
        requireArguments(argc, 5, "hackspfa tree N CORE M");
        generateNearTree(parseInt(argv[2], "N"), parseInt(argv[3], "CORE"),
                         parseInt(argv[4], "M"));
    } else {
        Instance::fail("unknown hack mode: " + mode);
    }
    return 0;
}
