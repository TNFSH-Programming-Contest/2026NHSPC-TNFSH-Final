#include <bits/stdc++.h>
using namespace std;

namespace {

struct HistoryEntry {
    int type;
    int a;
    int b;
    int c;
    int d;
    int e;
    long long delta;
};

class Solver {
public:
    Solver(int n, int q, vector<int> values)
        : n_(n),
          q_(q),
          values_(move(values)),
          positionNode_(n + 1),
          parent_(n + q + 5),
          treeSize_(n + q + 5, 1),
          rawSum_(n + q + 5, 0),
          nextNode_(n + 1) {
        buildBlocks();

        for (int i = 1; i <= n + q + 4; ++i) {
            parent_[i] = i;
        }
        for (int i = 1; i <= n; ++i) {
            positionNode_[i] = i;
            addBlockCount(blockOfVertex_[i], i, 1);
        }
        history_.reserve(q);
    }
    void unite(int a, int b) {
        int rootA = findVertex(a);
        int rootB = findVertex(b);
        if (rootA == rootB) {
#ifdef PF_PUSH_INEFFECTIVE
            pushNoopHistory();
#endif
            return;
        }
        if (treeSize_[rootA] > treeSize_[rootB]) {
            swap(rootA, rootB);
        }

        history_.push_back({1, rootA, rootB, treeSize_[rootA], 0, 0, 0});

        for (int block = 0; block < blockCount_; ++block) {
            const int amount = getBlockCount(block, rootA);
            if (amount != 0) {
                addBlockCount(block, rootB, amount);
            }
        }
        rawSum_[rootB] += rawSum_[rootA];
        parent_[rootA] = rootB;
        treeSize_[rootB] += treeSize_[rootA];
    }

    void moveVertex(int a, int b) {
        const int sourceRoot = findVertex(a);
        const int targetRoot = findVertex(b);
        if (sourceRoot == targetRoot) {
#ifdef PF_PUSH_INEFFECTIVE
            pushNoopHistory();
#endif
            return;
        }

        const int oldNode = positionNode_[a];
        const int newNode = nextNode_++;
        const long long rawValue = raw_[sortedPosition_[a]];

#ifndef PF_MOVE_FORGET_BLOCK_COUNT
        const int block = blockOfVertex_[a];
        addBlockCount(block, sourceRoot, -1);
        addBlockCount(block, targetRoot, 1);
#endif
#ifndef PF_MOVE_FORGET_RAW_SUM
        rawSum_[sourceRoot] -= rawValue;
        rawSum_[targetRoot] += rawValue;
#endif

        parent_[newNode] = targetRoot;
        treeSize_[newNode] = 1;
        ++treeSize_[targetRoot];
        positionNode_[a] = newNode;

        history_.push_back(
            {2, a, oldNode, newNode, sourceRoot, targetRoot, 0});
    }

    void addRange(int leftValue, int rightValue, long long delta) {
#ifdef PF_IGNORE_NEGATIVE
        if (delta < 0) {
            return;
        }
#endif
        if (delta == 0) {
#ifdef PF_PUSH_INEFFECTIVE
            pushNoopHistory();
#endif
            return;
        }

#ifdef PF_RANGE_BY_INDEX
        const int left = max(0, leftValue - 1);
        const int right = min(n_ - 1, rightValue - 1);
#elif defined(PF_OPEN_RANGE)
        const int left = static_cast<int>(
            upper_bound(sortedValues_.begin(), sortedValues_.end(), leftValue) -
            sortedValues_.begin());
        const int right = static_cast<int>(
            lower_bound(sortedValues_.begin(), sortedValues_.end(), rightValue) -
            sortedValues_.begin()) - 1;
#else
        const int left = static_cast<int>(
            lower_bound(sortedValues_.begin(), sortedValues_.end(), leftValue) -
            sortedValues_.begin());
        const int right = static_cast<int>(
            upper_bound(sortedValues_.begin(), sortedValues_.end(), rightValue) -
            sortedValues_.begin()) - 1;
#endif

        if (left > right) {
#ifdef PF_PUSH_INEFFECTIVE
            pushNoopHistory();
#endif
            return;
        }

        applyRange(left, right, delta);
        history_.push_back({3, left, right, 0, 0, 0, delta});
    }

    long long query(int vertex) const {
        const int root = findVertex(vertex);
        long long answer = rawSum_[root];

        for (int block = 0; block < blockCount_; ++block) {
            const int amount = getBlockCount(block, root);
            if (amount != 0) {
                answer += static_cast<long long>(amount) * lazy_[block];
            }
        }
        return answer;
    }

    void pushNoopHistory() {
        history_.push_back({4, 0, 0, 0, 0, 0, 0});
    }

    void undo() {
        if (history_.empty()) {
            return;
        }
        const HistoryEntry entry = history_.back();
        history_.pop_back();

        if (entry.type == 1) {
            undoUnion(entry);
        } else if (entry.type == 2) {
            undoMove(entry);
        } else if (entry.type == 3) {
#ifndef PF_NO_RANGE_UNDO
            applyRange(entry.a, entry.b, -entry.delta);
#endif
        }
    }

private:
    int n_;
    int q_;
    vector<int> values_;

    int blockSize_;
    int blockCount_;
    vector<int> vertexAtPosition_;
    vector<int> sortedPosition_;
    vector<int> sortedValues_;
    vector<int> blockOfVertex_;
    vector<long long> raw_;
    vector<long long> lazy_;
    vector<unordered_map<int, int> > countInBlock_;

    vector<int> positionNode_;
    mutable vector<int> parent_;
    vector<int> treeSize_;
    vector<long long> rawSum_;
    int nextNode_;
    vector<HistoryEntry> history_;

    void buildBlocks() {
        vertexAtPosition_.resize(n_);
        iota(vertexAtPosition_.begin(), vertexAtPosition_.end(), 1);
        sort(vertexAtPosition_.begin(), vertexAtPosition_.end(),
             [&](int lhs, int rhs) {
                 if (values_[lhs] != values_[rhs]) {
                     return values_[lhs] < values_[rhs];
                 }
                 return lhs < rhs;
             });

        blockSize_ = max(1, static_cast<int>(sqrt(n_)));
        blockCount_ = (n_ + blockSize_ - 1) / blockSize_;

        sortedPosition_.resize(n_ + 1);
        sortedValues_.resize(n_);
        blockOfVertex_.resize(n_ + 1);
        raw_.assign(n_, 0);
        lazy_.assign(blockCount_, 0);
        countInBlock_.resize(blockCount_);

        for (int position = 0; position < n_; ++position) {
            const int vertex = vertexAtPosition_[position];
            sortedPosition_[vertex] = position;
            sortedValues_[position] = values_[vertex];
            blockOfVertex_[vertex] = position / blockSize_;
        }

        for (int block = 0; block < blockCount_; ++block) {
            const int begin = block * blockSize_;
            const int end = min(n_, begin + blockSize_);
            const int vertices = end - begin;
            countInBlock_[block].reserve(vertices * 20 + 16);
            countInBlock_[block].max_load_factor(0.7f);
        }
    }

    int findNode(int node) const {
#ifdef PF_PATH_COMPRESSION
        if (parent_[node] != node) {
            parent_[node] = findNode(parent_[node]);
        }
        return parent_[node];
#else
        while (parent_[node] != node) {
            node = parent_[node];
        }
        return node;
#endif
    }

    int findVertex(int vertex) const {
        return findNode(positionNode_[vertex]);
    }

    int getBlockCount(int block, int root) const {
        const unordered_map<int, int>& counts = countInBlock_[block];
        const unordered_map<int, int>::const_iterator it = counts.find(root);
        return it == counts.end() ? 0 : it->second;
    }

    void addBlockCount(int block, int root, int delta) {
        unordered_map<int, int>& counts = countInBlock_[block];
        unordered_map<int, int>::iterator it = counts.find(root);

        if (it == counts.end()) {
            assert(delta > 0);
            counts.emplace(root, delta);
            return;
        }

        it->second += delta;
        assert(it->second >= 0);
        if (it->second == 0) {
            counts.erase(it);
        }
    }

    void addRawAtPosition(int position, long long delta) {
        const int vertex = vertexAtPosition_[position];
        const int root = findVertex(vertex);
        raw_[position] += delta;
        rawSum_[root] += delta;
    }

    void applyRange(int left, int right, long long delta) {
        const int leftBlock = left / blockSize_;
        const int rightBlock = right / blockSize_;

        if (leftBlock == rightBlock) {
            for (int position = left; position <= right; ++position) {
                addRawAtPosition(position, delta);
            }
            return;
        }

        const int leftEnd = min(n_, (leftBlock + 1) * blockSize_);
        for (int position = left; position < leftEnd; ++position) {
            addRawAtPosition(position, delta);
        }

        for (int block = leftBlock + 1; block < rightBlock; ++block) {
            lazy_[block] += delta;
        }

        const int rightBegin = rightBlock * blockSize_;
        for (int position = rightBegin; position <= right; ++position) {
            addRawAtPosition(position, delta);
        }
    }

    void undoUnion(const HistoryEntry& entry) {
        const int childRoot = entry.a;
        const int parentRoot = entry.b;
        const int attachedSize = entry.c;

        for (int block = 0; block < blockCount_; ++block) {
            const int amount = getBlockCount(block, childRoot);
            if (amount != 0) {
                addBlockCount(block, parentRoot, -amount);
            }
        }

        rawSum_[parentRoot] -= rawSum_[childRoot];
        treeSize_[parentRoot] -= attachedSize;
        parent_[childRoot] = childRoot;
    }

    void undoMove(const HistoryEntry& entry) {
        const int vertex = entry.a;
        const int oldNode = entry.b;
        const int newNode = entry.c;
        const int sourceRoot = entry.d;
        const int targetRoot = entry.e;
        const long long rawValue = raw_[sortedPosition_[vertex]];

#ifndef PF_MOVE_FORGET_BLOCK_COUNT
        const int block = blockOfVertex_[vertex];
        addBlockCount(block, targetRoot, -1);
        addBlockCount(block, sourceRoot, 1);
#endif
#ifndef PF_MOVE_FORGET_RAW_SUM
        rawSum_[targetRoot] -= rawValue;
        rawSum_[sourceRoot] += rawValue;
#endif

        positionNode_[vertex] = oldNode;
        --treeSize_[targetRoot];
        parent_[newNode] = newNode;
        treeSize_[newNode] = 1;
        --nextNode_;
    }
};

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> values(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> values[i];
    }

    Solver solver(n, q, move(values));

    for (int operation = 0; operation < q; ++operation) {
        int type;
        cin >> type;

        if (type == 1) {
            int a, b;
            cin >> a >> b;
            solver.unite(a, b);
        } else if (type == 2) {
            int a, b;
            cin >> a >> b;
#ifdef PF_NO_MOVE_SUBTASK
            return 0;
#elif defined(PF_MOVE_WHOLE_COMPONENT)
            solver.unite(a, b);
#else
            solver.moveVertex(a, b);
#endif
        } else if (type == 3) {
            int left, right;
            long long delta;
            cin >> left >> right >> delta;
            solver.addRange(left, right, delta);
        } else if (type == 4) {
            int vertex;
            cin >> vertex;
            const long long answer = solver.query(vertex);
#ifdef PF_INT32_ANSWER
            cout << static_cast<int>(answer) << '\n';
#else
            cout << answer << '\n';
#endif
#ifdef PF_QUERY_IS_OPERATION
            solver.pushNoopHistory();
#endif
        } else {
            solver.undo();
        }
    }

    return 0;
}
