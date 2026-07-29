#include "common.h"

using namespace std;

void generateList(Instance& in) {
    if (in.targetM != in.n - 1)
        Instance::fail("list mode requires M = N - 1");
    in.addBackbone(false);
}

void generateUnicyclic(Instance& in, const string& mode) {
    if (in.n < 3 || in.targetM != in.n)
        Instance::fail("unicyclic mode requires N >= 3 and M = N");

    in.addBackbone(true);
    int left = 0;
    int right = in.n - 1;
    if (mode == "unicyclic-short") {
        right = 2;
    } else if (mode == "unicyclic-middle") {
        left = max(0, in.n / 2 - 2);
        right = min(in.n - 1, left + max(2, in.n / 7));
    } else if (mode != "unicyclic-long") {
        Instance::fail("unknown unicyclic mode: " + mode);
    }

    int u = in.path[left];
    int v = in.path[right];
    if (rnd.next(2)) swap(u, v);
    for (int i = left; i < right; ++i)
        in.edges[i].role = SPECIAL;
    in.addEdge(u, v, SPECIAL);
}

void generateCactus(Instance& in, const string& mode) {
    if (in.targetM < in.n - 1)
        Instance::fail("cactus mode needs M >= N - 1");
    in.addBackbone(true);

    int remaining = in.targetM - (in.n - 1);
    int cursor = 0;
    int made = 0;

    while (remaining > 0 && cursor + 1 < in.n) {
        bool produced = false;
        if ((mode == "cactus-cycles" || mode == "cactus-mixed") &&
            cursor + 2 < in.n &&
            (mode != "cactus-mixed" || made % 3 == 0)) {
            int u = in.path[cursor];
            int v = in.path[cursor + 2];
            if (rnd.next(2)) swap(u, v);
            in.addEdge(u, v, EXTRA);
            cursor += 2;
            produced = true;
        } else if ((mode == "cactus-parallel" || mode == "cactus-mixed") &&
                   (mode != "cactus-mixed" || made % 3 == 1)) {
            int u = in.path[cursor];
            int v = in.path[cursor + 1];
            if (rnd.next(2)) swap(u, v);
            in.addEdge(u, v, EXTRA);
            ++cursor;
            produced = true;
        } else if (mode == "cactus-loops" || mode == "cactus-mixed") {
            int v = in.path[(made * 1009LL + 17) % in.n];
            in.addEdge(v, v, LOOP);
            produced = true;
        }

        if (!produced) break;
        --remaining;
        ++made;
    }

    while (remaining-- > 0) {
        int v = in.path[(made * 1009LL + 17) % in.n];
        in.addEdge(v, v, LOOP);
        ++made;
    }
}

void generateRandom(Instance& in, const string& mode) {
    if (in.targetM < in.n - 1)
        Instance::fail("random mode needs M >= N - 1");
    in.addBackbone(true);

    if (mode == "random-multi") {
        const int need = in.targetM - static_cast<int>(in.edges.size());
        Graph graph = Graph::random(in.n, need)
            .directed().allowLoops().allowMulti().allowAntiparallel();
        for (const auto& edge : graph.edges())
            in.addEdge(edge.first + 1, edge.second + 1,
                       edge.first == edge.second ? LOOP : EXTRA);
    } else if (mode == "random-simple") {
        set<pair<int, int>> used;
        for (const Edge& edge : in.edges)
            used.insert(undirectedKey(edge.from, edge.to));
        while (static_cast<int>(in.edges.size()) < in.targetM) {
            pair<int, int> edge = rnd.nextp(1, in.n, dpair);
            pair<int, int> key = undirectedKey(edge.first, edge.second);
            if (!used.insert(key).second) continue;
            if (rnd.next(2)) swap(edge.first, edge.second);
            in.addEdge(edge.first, edge.second, EXTRA);
        }
    } else {
        Instance::fail("unknown random mode: " + mode);
    }
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    config.generateLargeObjects = true;
    requireArguments(argc);

    const string mode = argv[1];
    Instance in(parseInt(argv[2], "N"), parseInt(argv[3], "M"));

    if (mode == "list")
        generateList(in);
    else if (mode.find("unicyclic-") == 0)
        generateUnicyclic(in, mode);
    else if (mode.find("cactus-") == 0)
        generateCactus(in, mode);
    else if (mode == "random-multi" || mode == "random-simple")
        generateRandom(in, mode);
    else
        Instance::fail("unknown mode: " + mode);

    in.print(argv[4], argv[5], argv[6]);
}
