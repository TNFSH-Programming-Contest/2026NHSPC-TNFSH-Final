#include "common.h"

using namespace std;

void generateParallelBomb(Instance& in) {
    in.addBackbone(true);
    if (in.n == 1) {
        while (static_cast<int>(in.edges.size()) < in.targetM)
            in.addEdge(1, 1, LOOP);
        return;
    }

    int u = in.path[in.n / 2 - (in.n > 2)];
    int v = in.path[in.n / 2];
    while (static_cast<int>(in.edges.size()) < in.targetM) {
        int from = u;
        int to = v;
        if (rnd.next(2)) swap(from, to);
        in.addEdge(from, to, EXTRA);
    }
}

void generateLoopBomb(Instance& in) {
    in.addBackbone(true);
    int at = 0;
    while (static_cast<int>(in.edges.size()) < in.targetM) {
        int v = in.path[at++ % in.n];
        in.addEdge(v, v, LOOP);
    }
}

void generateTheta(Instance& in) {
    if (in.n < 4)
        Instance::fail("theta mode requires N >= 4");
    in.addBackbone(true);
    int index = 2;
    while (static_cast<int>(in.edges.size()) < in.targetM) {
        int u = in.path[0];
        int v = in.path[index];
        if (rnd.next(2)) swap(u, v);
        in.addEdge(u, v, EXTRA);
        ++index;
        if (index >= in.n) index = 2;
    }
}

void generateHub(Instance& in) {
    in.addBackbone(true);
    int left = 0;
    int right = in.n - 1;
    while (static_cast<int>(in.edges.size()) < in.targetM) {
        int u = in.path[left];
        int v = in.path[right];
        if (rnd.next(2)) swap(u, v);
        in.addEdge(u, v, EXTRA);
        if (++left >= in.n - 1) left = 0;
        if (--right <= left) right = in.n - 1;
    }
}

void generateDenseCore(Instance& in) {
    in.addBackbone(true);
    int core = min(in.n, 632);
    Graph complete = Graph::complete(core).allowLoops(true).g();
    const auto coreEdges = complete.edges();
    int index = 0;
    while (static_cast<int>(in.edges.size()) < in.targetM) {
        const auto edge = coreEdges[index++ % coreEdges.size()];
        int u = in.path[edge.first];
        int v = in.path[edge.second];
        if (rnd.next(2)) swap(u, v);
        in.addEdge(u, v, u == v ? LOOP : EXTRA);
    }
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    config.generateLargeObjects = true;
    requireArguments(argc);

    const string mode = argv[1];
    Instance in(parseInt(argv[2], "N"), parseInt(argv[3], "M"));

    if (mode == "parallel-bomb")
        generateParallelBomb(in);
    else if (mode == "loop-bomb")
        generateLoopBomb(in);
    else if (mode == "theta")
        generateTheta(in);
    else if (mode == "hub")
        generateHub(in);
    else if (mode == "dense-core")
        generateDenseCore(in);
    else
        Instance::fail("unknown structured mode: " + mode);

    in.print(argv[4], argv[5], argv[6]);
}
