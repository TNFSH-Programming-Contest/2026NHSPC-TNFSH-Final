#include "common.h"

using namespace std;

void addRandomExtras(Instance& in, int m, EdgeRole role = EXTRA) {
    const int need = m - static_cast<int>(in.edges.size());
    if (need < 0) Instance::fail("too many fixed edges");
    Graph graph = Graph::random(in.n, need)
        .directed().allowLoops().allowMulti().allowAntiparallel();
    for (const auto& edge : graph.edges())
        in.addEdge(edge.first + 1, edge.second + 1, role);
}

void generateTheta(Instance& in, int m) {
    in.addBackbone(true);
    int turn = 0;
    while (static_cast<int>(in.edges.size()) < m) {
        int width = 2 + turn % min(31, max(2, in.n - 1));
        int left = (turn * 37LL) % max(1, in.n - width);
        int right = min(in.n - 1, left + width);
        int u = in.path[left];
        int v = in.path[right];
        if (turn & 1) swap(u, v);
        in.addEdge(u, v, EXTRA);
        ++turn;
    }
}

void generateClique(Instance& in, int m) {
    in.addBackbone(false);
    set<pair<int, int>> used;
    for (const Edge& edge : in.edges)
        used.insert(undirectedKey(edge.from, edge.to));

    int k = min(in.n, 700);
    for (int u = 1; u <= k && static_cast<int>(in.edges.size()) < m; ++u) {
        for (int v = u + 1; v <= k && static_cast<int>(in.edges.size()) < m; ++v) {
            if (!used.insert({u, v}).second) continue;
            if ((u + v) & 1) in.addEdge(u, v, EXTRA);
            else in.addEdge(v, u, EXTRA);
        }
    }
    addRandomExtras(in, m);
}

void generateParallel(Instance& in, int m) {
    in.addBackbone(true);
    int u = 1;
    int v = in.n == 1 ? 1 : in.path[in.n / 2];
    int turn = 0;
    while (static_cast<int>(in.edges.size()) < m) {
        if (in.n == 1 || turn % 7 == 0) {
            int x = in.path[turn % in.n];
            in.addEdge(x, x, LOOP);
        } else if (turn & 1) {
            in.addEdge(u, v, EXTRA);
        } else {
            in.addEdge(v, u, EXTRA);
        }
        ++turn;
    }
}

void generateLoops(Instance& in, int m) {
    in.addBackbone(true);
    int turn = 0;
    while (static_cast<int>(in.edges.size()) < m) {
        int v = in.path[(turn * 10007LL + 19) % in.n];
        in.addEdge(v, v, LOOP);
        ++turn;
    }
}

void generateBipartite(Instance& in, int m) {
    in.addBackbone(true);
    int need = m - static_cast<int>(in.edges.size());
    int left = max(1, in.n / 2);
    int right = in.n - left;
    if (right == 0) {
        addRandomExtras(in, m);
        return;
    }
    Graph graph = Graph::randomBipartite(left, right, need).allowMulti();
    for (const auto& edge : graph.edges()) {
        int u = edge.first + 1;
        int v = edge.second + 1;
        if (rnd.next(0, 1)) swap(u, v);
        in.addEdge(u, v, EXTRA);
    }
}

void generateTopCycle(Instance& in, int m) {
    in.addBackbone(false);
    int k = min(in.n, max(3, min(m - in.n + 2, 500)));
    for (int i = 1; i <= k && static_cast<int>(in.edges.size()) < m; ++i) {
        int j = i == k ? 1 : i + 1;
        in.addEdge(i, j, SPECIAL);
    }
    while (static_cast<int>(in.edges.size()) < m) {
        int u = rnd.next(1, k);
        int v = rnd.next(1, k);
        in.addEdge(u, v, v == u ? LOOP : SPECIAL);
    }
}

void generateAnnealing(Instance& in, int m) {
    if (in.n < 3 || m < in.n)
        Instance::fail("annealing mode needs N >= 3 and M >= N");
    in.addBackbone(false);
    for (Edge& edge : in.edges) edge.cost = 1;

    const int extra = m - (in.n - 1);
    const int specialCount = min({
        extra,
        5000,
        (in.n - 1) / 2
    });
    int turn = 0;
    while (static_cast<int>(in.edges.size()) + specialCount < m) {
        int vertex = 1 + turn % in.n;
        in.addEdge(vertex, vertex, LOOP, 1);
        ++turn;
    }
    // Large instances contain more indispensable improving chords than the
    // fake solver's entire candidate budget, making failure seed-independent.
    // With M=N this naturally degenerates to one unicyclic trap.
    for (int index = 0; index < specialCount; ++index) {
        int left = 2 * index + 1;
        in.addEdge(left, left + 2, SPECIAL, 1000000000LL);
    }
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    requireCommonArguments(argc);

    const string mode = argv[1];
    const int n = parseInt(argv[2], "N");
    const int m = parseInt(argv[3], "M");

    if (m < n - 1)
        Instance::fail("attack generator needs M >= N - 1");

    Instance in(n, m);
    if (mode == "theta")
        generateTheta(in, m);
    else if (mode == "clique")
        generateClique(in, m);
    else if (mode == "parallel")
        generateParallel(in, m);
    else if (mode == "loops")
        generateLoops(in, m);
    else if (mode == "bipartite")
        generateBipartite(in, m);
    else if (mode == "ordered") {
        in.addBackbone(false);
        addRandomExtras(in, m);
    }
    else if (mode == "top-cycle")
        generateTopCycle(in, m);
    else if (mode == "annealing")
        generateAnnealing(in, m);
    else
        Instance::fail("unknown attack mode: " + mode);

    in.print(argv[4], argv[5], argv[6]);
}
