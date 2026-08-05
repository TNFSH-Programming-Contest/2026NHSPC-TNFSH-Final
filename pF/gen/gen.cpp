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

vector<int> buildValues(int n, const string& style) {
    vector<int> values(n + 1);
    if (style == "same") {
        fill(values.begin() + 1, values.end(), 500000000);
    } else if (style == "distinct" || style == "shuffled-distinct") {
        for (int i = 1; i <= n; ++i) {
            values[i] = n == 1 ? 0 :
                static_cast<long long>(i - 1) * MAX_A / (n - 1);
        }
        if (style == "shuffled-distinct") {
            vector<int> order(values.begin() + 1, values.end());
            randomShuffle(order);
            copy(order.begin(), order.end(), values.begin() + 1);
        }
    } else if (style == "random") {
        for (int i = 1; i <= n; ++i) values[i] = rnd.next(0, MAX_A);
    } else if (style == "few32") {
        vector<int> choices;
        for (int i = 0; i < 32; ++i) {
            choices.push_back(static_cast<long long>(i) * MAX_A / 31);
        }
        for (int i = 1; i <= n; ++i) values[i] = choices[(i * 13LL + 7) % 32];
        vector<int> order(values.begin() + 1, values.end());
        randomShuffle(order);
        copy(order.begin(), order.end(), values.begin() + 1);
    } else if (style == "few2") {
        for (int i = 1; i <= n; ++i) values[i] = (i & 1) ? 0 : MAX_A;
    } else if (style == "clusters") {
        const int center[8] = {
            0, 1, 999, 1000, 499999999, 500000000, 999999999, MAX_A
        };
        for (int i = 1; i <= n; ++i) values[i] = center[(i * 5LL + i / 17) % 8];
        vector<int> order(values.begin() + 1, values.end());
        randomShuffle(order);
        copy(order.begin(), order.end(), values.begin() + 1);
    } else if (style == "alternating") {
        for (int i = 1; i <= n; ++i) values[i] = (i & 1) ? MAX_A : 0;
    } else if (style == "gaps") {
        for (int i = 1; i <= n; ++i) {
            values[i] = i <= n / 2 ? i - 1 : MAX_A - (n - i);
        }
        vector<int> order(values.begin() + 1, values.end());
        randomShuffle(order);
        copy(order.begin(), order.end(), values.begin() + 1);
    } else {
        quitf(_fail, "unknown A style: %s", style.c_str());
    }
    return values;
}

struct History {
    int type;
    int a;
    int b;
    int c;
    int d;
};

class Builder {
public:
    Builder(int n, int q, vector<int> values)
        : n_(n), q_(q), values_(move(values)), position_(n + 1),
          parent_(n + q + 5), treeSize_(n + q + 5, 1), nextNode_(n + 1) {
        iota(parent_.begin(), parent_.end(), 0);
        iota(position_.begin(), position_.end(), 0);
        sorted_.assign(values_.begin() + 1, values_.end());
        sort(sorted_.begin(), sorted_.end());
        operations_.reserve(q);
        history_.reserve(q);
    }

    bool full() const { return static_cast<int>(operations_.size()) >= q_; }
    int historySize() const { return static_cast<int>(history_.size()); }
    int n() const { return n_; }
    const vector<int>& sortedValues() const { return sorted_; }
    int valueOf(int vertex) const { return values_[vertex]; }

    int rootOf(int vertex) const {
        int node = position_[vertex];
        while (parent_[node] != node) node = parent_[node];
        return node;
    }

    pair<int, int> differentPair() const {
        if (n_ == 1) return {1, 1};
        for (int attempt = 0; attempt < 80; ++attempt) {
            const int a = rnd.next(1, n_);
            const int b = rnd.next(1, n_);
            if (rootOf(a) != rootOf(b)) return {a, b};
        }
        const int base = rootOf(1);
        for (int vertex = 2; vertex <= n_; ++vertex) {
            if (rootOf(vertex) != base) return {1, vertex};
        }
        return {1, 1};
    }

    void addUnion(int x, int y) {
        if (full()) return;
        operations_.push_back("1 " + to_string(x) + " " + to_string(y));
        int a = rootOf(x), b = rootOf(y);
        if (a == b) return;
        if (treeSize_[a] > treeSize_[b]) swap(a, b);
        history_.push_back({1, a, b, treeSize_[a], 0});
        parent_[a] = b;
        treeSize_[b] += treeSize_[a];
    }

    void addMove(int vertex, int target) {
        if (full()) return;
        operations_.push_back("2 " + to_string(vertex) + " " + to_string(target));
        const int sourceRoot = rootOf(vertex);
        const int targetRoot = rootOf(target);
        if (sourceRoot == targetRoot) return;
        const int oldNode = position_[vertex];
        const int newNode = nextNode_++;
        history_.push_back({2, vertex, oldNode, newNode, targetRoot});
        position_[vertex] = newNode;
        parent_[newNode] = targetRoot;
        treeSize_[newNode] = 1;
        ++treeSize_[targetRoot];
    }

    void addRange(int left, int right, long long delta) {
        if (full()) return;
        operations_.push_back("3 " + to_string(left) + " " + to_string(right) +
                              " " + to_string(delta));
        const auto it = lower_bound(sorted_.begin(), sorted_.end(), left);
        if (delta != 0 && it != sorted_.end() && *it <= right) {
            history_.push_back({3, 0, 0, 0, 0});
        }
    }

    void addQuery(int vertex) {
        if (!full()) operations_.push_back("4 " + to_string(vertex));
    }

    void addUndo() {
        if (full()) return;
        if (history_.empty()) {
            addQuery(rnd.next(1, n_));
            return;
        }
        operations_.push_back("5");
        const History entry = history_.back();
        history_.pop_back();
        if (entry.type == 1) {
            parent_[entry.a] = entry.a;
            treeSize_[entry.b] -= entry.c;
        } else if (entry.type == 2) {
            position_[entry.a] = entry.b;
            --treeSize_[entry.d];
            parent_[entry.c] = entry.c;
            treeSize_[entry.c] = 1;
            --nextNode_;
        }
    }

    int emptyPoint() const {
        if (sorted_.front() > 0) return 0;
        for (int i = 1; i < static_cast<int>(sorted_.size()); ++i) {
            if (sorted_[i - 1] + 1 < sorted_[i]) return sorted_[i - 1] + 1;
        }
        if (sorted_.back() < MAX_A) return sorted_.back() + 1;
        quitf(_fail, "could not find an empty coordinate");
        return 0;
    }

    void print() const {
        ensuref(static_cast<int>(operations_.size()) == q_,
                "generator made %d operations, expected %d",
                static_cast<int>(operations_.size()), q_);
        cout << n_ << ' ' << q_ << '\n';
        for (int i = 1; i <= n_; ++i) {
            if (i > 1) cout << ' ';
            cout << values_[i];
        }
        cout << '\n';
        for (const string& operation : operations_) cout << operation << '\n';
    }

private:
    int n_;
    int q_;
    vector<int> values_;
    vector<int> sorted_;
    vector<int> position_;
    vector<int> parent_;
    vector<int> treeSize_;
    int nextNode_;
    vector<History> history_;
    vector<string> operations_;
};

long long randomDelta(bool allowZero = true) {
    const int kind = rnd.next(0, 9);
    if (allowZero && kind == 0) return 0;
    if (kind == 1) return MAX_A;
    if (kind == 2) return -MAX_A;
    if (kind == 3) return 1;
    if (kind == 4) return -1;
    return rnd.next(-MAX_A, MAX_A);
}

void addRandomRange(Builder& builder, bool allowIneffective = true) {
    const vector<int>& sorted = builder.sortedValues();
    const int kind = rnd.next(0, allowIneffective ? 7 : 5);
    int left = 0, right = MAX_A;
    if (kind == 1) {
        left = right = sorted[rnd.next(0, static_cast<int>(sorted.size()) - 1)];
    } else if (kind == 2) {
        int a = rnd.next(0, static_cast<int>(sorted.size()) - 1);
        int b = rnd.next(0, static_cast<int>(sorted.size()) - 1);
        if (a > b) swap(a, b);
        left = sorted[a];
        right = sorted[b];
    } else if (kind == 3) {
        right = sorted[rnd.next(0, static_cast<int>(sorted.size()) - 1)];
    } else if (kind == 4) {
        left = sorted[rnd.next(0, static_cast<int>(sorted.size()) - 1)];
    } else if (kind == 5) {
        left = right = builder.emptyPoint();
    } else if (kind >= 6) {
        left = rnd.next(0, MAX_A);
        right = rnd.next(left, MAX_A);
    }
    builder.addRange(left, right, randomDelta(allowIneffective));
}

void fillRandom(Builder& builder, bool allowUnion, bool allowMove) {
    while (!builder.full()) {
        const int kind = rnd.next(0, 99);
        if (kind < 12 && builder.historySize() > 0) {
            builder.addUndo();
        } else if (allowUnion && kind < 28) {
            const pair<int, int> vertices = builder.differentPair();
            if (rnd.next(0, 4) == 0) builder.addUnion(vertices.first, vertices.first);
            else builder.addUnion(vertices.first, vertices.second);
        } else if (allowMove && kind < 48) {
            const pair<int, int> vertices = builder.differentPair();
            if (rnd.next(0, 4) == 0) builder.addMove(vertices.first, vertices.first);
            else builder.addMove(vertices.first, vertices.second);
        } else if (kind < 76) {
            addRandomRange(builder);
        } else {
            builder.addQuery(rnd.next(1, builder.n()));
        }
    }
}

void generateRangeOnly(Builder& builder) {
    while (!builder.full()) {
        const int kind = rnd.next(0, 99);
        if (kind < 24 && builder.historySize() > 0) builder.addUndo();
        else if (kind < 70) addRandomRange(builder);
        else builder.addQuery(rnd.next(1, builder.n()));
    }
}

void generateHistoryAttack(Builder& builder, bool rangeOnly) {
    while (!builder.full()) {
        const int vertex = rnd.next(1, builder.n());
        const int value = builder.sortedValues()[rnd.next(
            0, static_cast<int>(builder.sortedValues().size()) - 1)];
        builder.addRange(value, value, 7);
        builder.addQuery(vertex);
        builder.addQuery(vertex);
        builder.addRange(value, value, 0);
        const int empty = builder.emptyPoint();
        builder.addRange(empty, empty, 11);
        if (!rangeOnly) {
            builder.addUnion(vertex, vertex);
            builder.addMove(vertex, vertex);
        }
        builder.addQuery(vertex);
        builder.addUndo();
        builder.addQuery(vertex);
    }
}

void generateEmptyComponents(Builder& builder) {
    int first = 1;
    while (!builder.full()) {
        if (builder.n() < 3) {
            fillRandom(builder, true, true);
            return;
        }
        const int a = first;
        const int b = first % builder.n() + 1;
        const int c = (first + 1) % builder.n() + 1;
        first = (first + 3) % builder.n() + 1;
        builder.addUnion(a, b);
        builder.addMove(a, c);
        builder.addMove(b, c);  // The old component becomes empty here.
        builder.addRange(0, MAX_A, -13);
        builder.addQuery(c);
        builder.addUndo();
        builder.addQuery(b);
        builder.addUndo();
        builder.addQuery(a);
        builder.addUndo();
        builder.addUndo();
        builder.addQuery(a);
    }
}

void generateOverflow(Builder& builder) {
    const int unionLimit = min(builder.n(), max(2, builder.n() * 3 / 4));
    for (int vertex = 2; vertex <= unionLimit && !builder.full(); ++vertex) {
        builder.addUnion(1, vertex);
    }
    while (!builder.full()) {
        builder.addRange(0, MAX_A, MAX_A);
        builder.addQuery(1);
        builder.addRange(0, MAX_A, -MAX_A + 1);
        builder.addQuery(1);
        builder.addUndo();
        builder.addQuery(1);
    }
}

void generateRollback(Builder& builder) {
    while (!builder.full()) {
        const int before = builder.historySize();
        for (int step = 0; step < 40 && !builder.full(); ++step) {
            if (step % 3 == 0) {
                const pair<int, int> vertices = builder.differentPair();
                builder.addUnion(vertices.first, vertices.second);
            } else if (step % 3 == 1) {
                const pair<int, int> vertices = builder.differentPair();
                builder.addMove(vertices.first, vertices.second);
            } else {
                addRandomRange(builder, false);
            }
        }
        builder.addQuery(rnd.next(1, builder.n()));
        while (builder.historySize() > before && !builder.full()) {
            builder.addUndo();
            if (rnd.next(0, 3) == 0) builder.addQuery(rnd.next(1, builder.n()));
        }
    }
}

void generateBlocks(Builder& builder) {
    const int unions = min(builder.n() - 1, 4000);
    for (int i = 1; i <= unions && !builder.full(); ++i) {
        builder.addUnion(i, i + 1);
    }
    const vector<int>& sorted = builder.sortedValues();
    const int block = max(1, static_cast<int>(sqrt(builder.n())));
    int iteration = 0;
    while (!builder.full()) {
        const int pivot = min(static_cast<int>(sorted.size()) - 1,
                              (iteration * 97) % static_cast<int>(sorted.size()));
        const int leftPosition = max(0, pivot - block - 1);
        const int rightPosition = min(static_cast<int>(sorted.size()) - 1,
                                      pivot + block + 1);
        builder.addRange(sorted[leftPosition], sorted[rightPosition],
                         iteration & 1 ? -999999937LL : 999999937LL);
        builder.addQuery(iteration % builder.n() + 1);
        if (iteration % 5 == 0) {
            const pair<int, int> vertices = builder.differentPair();
            builder.addMove(vertices.first, vertices.second);
        }
        if (iteration % 7 == 0 && builder.historySize() > 0) builder.addUndo();
        ++iteration;
    }
}

void generateQuadraticAttack(Builder& builder) {
    for (int vertex = 2; vertex <= builder.n() && !builder.full(); ++vertex) {
        builder.addUnion(1, vertex);
    }
    int step = 0;
    while (!builder.full()) {
        if (step % 4 == 0) builder.addRange(0, MAX_A, step & 8 ? -1 : 1);
        else if (step % 4 == 1) builder.addQuery(1);
        else if (step % 4 == 2) {
            const pair<int, int> vertices = builder.differentPair();
            builder.addMove(vertices.first, vertices.second);
        } else if (builder.historySize() > 0) {
            builder.addUndo();
        } else {
            builder.addQuery(1);
        }
        ++step;
    }
}

void generateCompressionAttack(Builder& builder) {
    if (builder.n() < 5) {
        fillRandom(builder, true, true);
        return;
    }
    while (!builder.full()) {
        builder.addRange(0, MAX_A, 17);
        builder.addUnion(1, 2);
        builder.addUnion(3, 4);
        builder.addUnion(3, 5);
        builder.addUnion(1, 3);
        builder.addQuery(1);
        builder.addUndo();
        builder.addQuery(1);
        builder.addUndo();
        builder.addUndo();
        builder.addUndo();
        builder.addUndo();
        builder.addQuery(1);
    }
}

void generateMoveValueAttack(Builder& builder) {
    if (builder.n() < 3) {
        fillRandom(builder, true, true);
        return;
    }
    while (!builder.full()) {
        const int value = builder.sortedValues().front();
        builder.addRange(value, value, 123456789);
        builder.addUnion(1, 2);
        builder.addMove(1, 3);
        builder.addQuery(2);
        builder.addQuery(3);
        builder.addUndo();
        builder.addQuery(2);
        builder.addQuery(3);
        builder.addUndo();
        builder.addUndo();
        builder.addQuery(1);
    }
}

void generateSamplingAttack(Builder& builder) {
    if (builder.n() < 130 || builder.n() > 50000) {
        fillRandom(builder, true, false);
        return;
    }
    for (int vertex = 2; vertex < builder.n() && !builder.full(); ++vertex)
        builder.addUnion(1, vertex);

    const int rare = builder.n() / 2;
    builder.addRange(builder.valueOf(rare), builder.valueOf(rare), 1000000000LL);
    builder.addQuery(1);
    while (!builder.full()) builder.addQuery(1);
}

}  // namespace

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    ensuref(argc >= 5, "usage: gen MODE N Q A_STYLE [seed-tag]");
    const string mode = argv[1];
    const int n = atoi(argv[2]);
    const int q = atoi(argv[3]);
    ensuref(1 <= n && n <= 50000, "N out of range");
    ensuref(1 <= q && q <= 50000, "Q out of range");

    Builder builder(n, q, buildValues(n, argv[4]));
    if (mode == "random") {
        fillRandom(builder, true, true);
    } else if (mode == "move-heavy") {
        for (int i = 1; i + 1 <= n && !builder.full() && i <= n / 2; i += 2) {
            builder.addUnion(i, i + 1);
        }
        fillRandom(builder, true, true);
    } else if (mode == "range-only") {
        generateRangeOnly(builder);
    } else if (mode == "range-history") {
        generateHistoryAttack(builder, true);
    } else if (mode == "no-move") {
        fillRandom(builder, true, false);
    } else if (mode == "history") {
        generateHistoryAttack(builder, false);
    } else if (mode == "empty-components") {
        generateEmptyComponents(builder);
    } else if (mode == "overflow") {
        generateOverflow(builder);
    } else if (mode == "rollback") {
        generateRollback(builder);
    } else if (mode == "blocks") {
        generateBlocks(builder);
    } else if (mode == "quadratic") {
        generateQuadraticAttack(builder);
    } else if (mode == "compression") {
        generateCompressionAttack(builder);
    } else if (mode == "move-values") {
        generateMoveValueAttack(builder);
    } else if (mode == "sampling") {
        generateSamplingAttack(builder);
    } else {
        quitf(_fail, "unknown generator mode: %s", mode.c_str());
    }
    builder.print();
}
