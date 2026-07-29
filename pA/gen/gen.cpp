#include "common.h"

using namespace std;

namespace {

void fillWithJngen(Instance& instance, int target) {
    const int need = target - instance.roads.size();
    if (need > 0) {
        Graph graph = Graph::random(instance.n, need)
            .directed().allowAntiparallel();
        for (const auto& edge : graph.edges())
            instance.addUnique(edge.first + 1, edge.second + 1);
    }

    // A generated road can coincide with a preinserted canonical road.
    // The sparse constraints make this short top-up loop sufficient.
    int attempts = 0;
    while (static_cast<int>(instance.roads.size()) < target && attempts < 10000000) {
        ++attempts;
        instance.addUnique(rnd.next(1, instance.n), rnd.next(1, instance.n));
    }
    if (static_cast<int>(instance.roads.size()) != target)
        Instance::fail("could not reach the requested number of roads");
}

void generateRandom(int n, int m, int maximum, const string& cost_style,
                    const string& order, bool feasible) {
    Instance instance(n);
    if (m < 1 || m > min(MAX_M, n * (n - 1)))
        Instance::fail("m is out of range");
    if (feasible) {
        if (m < n - 1) Instance::fail("feasible mode needs m >= n-1");
        instance.addCanonicalMatching();
    }
    fillWithJngen(instance, m);
    instance.assignCosts(cost_style, maximum);
    instance.print(order);
}

void generateImpossible(int n, int m, int maximum, const string& cost_style,
                        const string& order, bool remove_outgoing) {
    Instance instance(n);
    int attempts = 0;
    while (static_cast<int>(instance.roads.size()) < m && attempts < 10000000) {
        ++attempts;
        const int from = rnd.next(1, n);
        const int to = rnd.next(1, n);
        if ((remove_outgoing && from == 1) || (!remove_outgoing && to == n))
            continue;
        instance.addUnique(from, to);
    }
    if (static_cast<int>(instance.roads.size()) != m)
        Instance::fail("could not generate the impossible instance");
    instance.assignCosts(cost_style, maximum);
    instance.print(order);
}

}  // namespace

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    config.generateLargeObjects = true;
    requireArguments(argc, 2, "gen MODE ...");

    const string mode = argv[1];
    if (mode == "random") {
        requireArguments(argc, 8, "gen random N M MAX_COST COST_STYLE ORDER feasible|arbitrary");
        const string feasibility = argv[7];
        if (feasibility != "feasible" && feasibility != "arbitrary")
            Instance::fail("unknown feasibility mode");
        generateRandom(parseInt(argv[2], "N"), parseInt(argv[3], "M"),
                       parseInt(argv[4], "MAX_COST"), argv[5], argv[6],
                       feasibility == "feasible");
    } else if (mode == "chain") {
        requireArguments(argc, 6, "gen chain N MAX_COST COST_STYLE ORDER");
        Instance instance(parseInt(argv[2], "N"));
        instance.addCanonicalMatching();
        instance.assignCosts(argv[4], parseInt(argv[3], "MAX_COST"));
        instance.print(argv[5]);
    } else if (mode == "impossible-out" || mode == "impossible-in") {
        requireArguments(argc, 7, "gen impossible-* N M MAX_COST COST_STYLE ORDER");
        generateImpossible(parseInt(argv[2], "N"), parseInt(argv[3], "M"),
                           parseInt(argv[4], "MAX_COST"), argv[5], argv[6],
                           mode == "impossible-out");
    } else {
        Instance::fail("unknown mode: " + mode);
    }
    return 0;
}
