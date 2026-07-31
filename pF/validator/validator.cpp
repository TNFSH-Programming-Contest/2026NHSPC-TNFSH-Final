#include "testlib.h"

#include <algorithm>
#include <set>
#include <string>
#include <vector>

using namespace std;

namespace {

const int MAX_N = 50000;
const int MAX_Q = 50000;
const int MAX_A = 1000000000;
const long long MAX_X = 1000000000LL;

struct HistoryEntry {
    int type;
    int x;
    int y;
    int z;
    int w;
};

class RollbackAlmostDsu {
public:
    RollbackAlmostDsu(int n, int maxMoves)
        : position_(n + 1),
          parent_(n + maxMoves + 5),
          treeSize_(n + maxMoves + 5, 1),
          nextNode_(n + 1) {
        for (int i = 1; i <= n + maxMoves + 4; ++i) {
            parent_[i] = i;
        }
        for (int i = 1; i <= n; ++i) {
            position_[i] = i;
        }
    }

    int findNode(int node) const {
        while (parent_[node] != node) {
            node = parent_[node];
        }
        return node;
    }

    int findVertex(int vertex) const {
        return findNode(position_[vertex]);
    }

    bool uniteVertices(int a, int b, vector<HistoryEntry>& history) {
        int rootA = findVertex(a);
        int rootB = findVertex(b);
        if (rootA == rootB) {
            return false;
        }

        if (treeSize_[rootA] > treeSize_[rootB]) {
            swap(rootA, rootB);
        }

        history.push_back({1, rootA, rootB, treeSize_[rootA], 0});
        parent_[rootA] = rootB;
        treeSize_[rootB] += treeSize_[rootA];
        return true;
    }

    bool moveVertex(int a, int b, vector<HistoryEntry>& history) {
        const int oldRoot = findVertex(a);
        const int targetRoot = findVertex(b);
        if (oldRoot == targetRoot) {
            return false;
        }

        const int oldNode = position_[a];
        const int newNode = nextNode_++;
        ensuref(newNode < static_cast<int>(parent_.size()),
                "internal validator error: exhausted auxiliary DSU nodes");

        parent_[newNode] = targetRoot;
        treeSize_[newNode] = 1;
        ++treeSize_[targetRoot];
        position_[a] = newNode;
        history.push_back({2, a, oldNode, newNode, targetRoot});
        return true;
    }

    void undo(const HistoryEntry& entry) {
        if (entry.type == 1) {
            const int childRoot = entry.x;
            const int parentRoot = entry.y;
            const int attachedSize = entry.z;

            ensuref(parent_[childRoot] == parentRoot,
                    "internal validator error while undoing union");
            parent_[childRoot] = childRoot;
            treeSize_[parentRoot] -= attachedSize;
        } else if (entry.type == 2) {
            const int vertex = entry.x;
            const int oldNode = entry.y;
            const int newNode = entry.z;
            const int targetRoot = entry.w;

            ensuref(nextNode_ == newNode + 1,
                    "internal validator error: move nodes are not rolled back in LIFO order");
            ensuref(position_[vertex] == newNode,
                    "internal validator error while restoring a moved vertex");
            ensuref(parent_[newNode] == targetRoot,
                    "internal validator error while detaching a moved vertex");

            position_[vertex] = oldNode;
            --treeSize_[targetRoot];
            parent_[newNode] = newNode;
            treeSize_[newNode] = 1;
            --nextNode_;
        } else {
            ensuref(entry.type == 3,
                    "internal validator error: unknown history type %d", entry.type);
        }
    }

private:
    vector<int> position_;
    vector<int> parent_;
    vector<int> treeSize_;
    int nextNode_;
};

bool rangeContainsPoint(
        const vector<int>& sortedA, int left, int right) {
    const vector<int>::const_iterator it =
        lower_bound(sortedA.begin(), sortedA.end(), left);
    return it != sortedA.end() && *it <= right;
}

}  // namespace

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    const string mode = argc >= 2 ? argv[1] : "full";
    ensuref(mode == "full" || mode == "small" ||
                mode == "range_only" || mode == "same_a" ||
                mode == "a32" || mode == "no_move",
            "unknown validator mode: %s", mode.c_str());

    const int n = inf.readInt(1, MAX_N, "N");
    inf.readSpace();
    const int q = inf.readInt(1, MAX_Q, "Q");
    inf.readEoln();

    vector<int> values(n + 1);
    vector<int> sortedA;
    sortedA.reserve(n);
    set<int> distinctA;

    for (int i = 1; i <= n; ++i) {
        values[i] = inf.readInt(0, MAX_A, format("A[%d]", i));
        sortedA.push_back(values[i]);
        distinctA.insert(values[i]);

        if (i == n) {
            inf.readEoln();
        } else {
            inf.readSpace();
        }
    }
    sort(sortedA.begin(), sortedA.end());

    if (mode == "small") {
        ensuref(n <= 2000 && q <= 2000,
                "small subtask requires N,Q <= 2000, but N = %d and Q = %d",
                n, q);
    } else if (mode == "same_a") {
        ensuref(distinctA.size() == 1,
                "same_a subtask requires all A[i] to be equal, but found %d distinct values",
                static_cast<int>(distinctA.size()));
    } else if (mode == "a32") {
        ensuref(distinctA.size() <= 32,
                "a32 subtask allows at most 32 distinct A[i], but found %d",
                static_cast<int>(distinctA.size()));
    }

    RollbackAlmostDsu dsu(n, q);
    vector<HistoryEntry> history;
    history.reserve(q);

    for (int operationIndex = 1; operationIndex <= q; ++operationIndex) {
        const int type =
            inf.readInt(1, 5, format("type[%d]", operationIndex));

        if (mode == "range_only") {
            ensuref(type >= 3,
                    "range_only subtask forbids operation type %d at operation %d",
                    type, operationIndex);
        }
        if (mode == "no_move") {
            ensuref(type != 2,
                    "no_move subtask forbids operation type 2 at operation %d",
                    operationIndex);
        }

        if (type == 1 || type == 2) {
            inf.readSpace();
            const int a =
                inf.readInt(1, n, format("a[%d]", operationIndex));
            inf.readSpace();
            const int b =
                inf.readInt(1, n, format("b[%d]", operationIndex));
            inf.readEoln();

            if (type == 1) {
                dsu.uniteVertices(a, b, history);
            } else {
                dsu.moveVertex(a, b, history);
            }
        } else if (type == 3) {
            inf.readSpace();
            const int left =
                inf.readInt(0, MAX_A, format("l[%d]", operationIndex));
            inf.readSpace();
            const int right =
                inf.readInt(left, MAX_A, format("r[%d]", operationIndex));
            inf.readSpace();
            const long long delta =
                inf.readLong(-MAX_X, MAX_X, format("x[%d]", operationIndex));
            inf.readEoln();

            if (delta != 0 && rangeContainsPoint(sortedA, left, right)) {
                history.push_back({3, 0, 0, 0, 0});
            }
        } else if (type == 4) {
            inf.readSpace();
            inf.readInt(1, n, format("a[%d]", operationIndex));
            inf.readEoln();
        } else {
            inf.readEoln();
            ensuref(!history.empty(),
                    "operation %d tries to undo while the successful-operation stack is empty",
                    operationIndex);

            const HistoryEntry entry = history.back();
            history.pop_back();
            dsu.undo(entry);
        }
    }

    inf.readEof();
    return 0;
}
