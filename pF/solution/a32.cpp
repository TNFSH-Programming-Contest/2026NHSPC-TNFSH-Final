#include <bits/stdc++.h>
using namespace std;

struct History {
    int type;
    int a;
    int b;
    int c;
    int d;
    int e;
    int valueId;
    long long delta;
};

class Solver {
public:
    Solver(int n, int q, vector<int> valueId, int kinds)
        : kinds_(kinds),
          valueId_(move(valueId)),
          position_(n + 1),
          parent_(n + q + 5),
          treeSize_(n + q + 5, 1),
          count_(static_cast<size_t>(n + q + 5) * kinds, 0),
          nextNode_(n + 1) {
        iota(parent_.begin(), parent_.end(), 0);
        for (int i = 1; i <= n; ++i) {
            position_[i] = i;
            at(i, valueId_[i]) = 1;
        }
    }

    int rootOf(int vertex) const {
        int node = position_[vertex];
        while (parent_[node] != node) node = parent_[node];
        return node;
    }

    void unite(int x, int y, vector<History>& history) {
        int a = rootOf(x), b = rootOf(y);
        if (a == b) return;
        if (treeSize_[a] > treeSize_[b]) swap(a, b);
        history.push_back({1, a, b, treeSize_[a], 0, 0, 0, 0});
        for (int id = 0; id < kinds_; ++id) at(b, id) += at(a, id);
        parent_[a] = b;
        treeSize_[b] += treeSize_[a];
    }

    void moveVertex(int vertex, int target, vector<History>& history) {
        const int sourceRoot = rootOf(vertex);
        const int targetRoot = rootOf(target);
        if (sourceRoot == targetRoot) return;
        const int oldNode = position_[vertex];
        const int newNode = nextNode_++;
        const int id = valueId_[vertex];
        history.push_back({2, vertex, oldNode, newNode, sourceRoot, targetRoot, id, 0});
        --at(sourceRoot, id);
        ++at(targetRoot, id);
        position_[vertex] = newNode;
        parent_[newNode] = targetRoot;
        treeSize_[newNode] = 1;
        ++treeSize_[targetRoot];
    }

    long long query(int vertex, const vector<long long>& value) const {
        const int root = rootOf(vertex);
        long long answer = 0;
        for (int id = 0; id < kinds_; ++id) {
            answer += static_cast<long long>(at(root, id)) * value[id];
        }
        return answer;
    }

    void undo(const History& entry) {
        if (entry.type == 1) {
            for (int id = 0; id < kinds_; ++id) at(entry.b, id) -= at(entry.a, id);
            parent_[entry.a] = entry.a;
            treeSize_[entry.b] -= entry.c;
        } else if (entry.type == 2) {
            ++at(entry.d, entry.valueId);
            --at(entry.e, entry.valueId);
            position_[entry.a] = entry.b;
            --treeSize_[entry.e];
            parent_[entry.c] = entry.c;
            treeSize_[entry.c] = 1;
            --nextNode_;
        }
    }

private:
    int kinds_;
    vector<int> valueId_;
    vector<int> position_;
    vector<int> parent_;
    vector<int> treeSize_;
    vector<int> count_;
    int nextNode_;

    int& at(int root, int id) { return count_[static_cast<size_t>(root) * kinds_ + id]; }
    int at(int root, int id) const {
        return count_[static_cast<size_t>(root) * kinds_ + id];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1), coordinates;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        coordinates.push_back(a[i]);
    }
    sort(coordinates.begin(), coordinates.end());
    coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
    if (coordinates.size() > 32) return 0;

    vector<int> valueId(n + 1);
    for (int i = 1; i <= n; ++i) {
        valueId[i] = lower_bound(coordinates.begin(), coordinates.end(), a[i]) -
                     coordinates.begin();
    }

    Solver solver(n, q, move(valueId), static_cast<int>(coordinates.size()));
    vector<long long> value(coordinates.size(), 0);
    vector<History> history;
    history.reserve(q);

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int x, y;
            cin >> x >> y;
            solver.unite(x, y, history);
        } else if (type == 2) {
            int x, y;
            cin >> x >> y;
            solver.moveVertex(x, y, history);
        } else if (type == 3) {
            int leftValue, rightValue;
            long long delta;
            cin >> leftValue >> rightValue >> delta;
            const int left = lower_bound(coordinates.begin(), coordinates.end(), leftValue) -
                             coordinates.begin();
            const int right = upper_bound(coordinates.begin(), coordinates.end(), rightValue) -
                              coordinates.begin() - 1;
            if (delta == 0 || left > right) continue;
            for (int id = left; id <= right; ++id) value[id] += delta;
            history.push_back({3, left, right, 0, 0, 0, 0, delta});
        } else if (type == 4) {
            int vertex;
            cin >> vertex;
            cout << solver.query(vertex, value) << '\n';
        } else {
            const History entry = history.back();
            history.pop_back();
            if (entry.type == 3) {
                for (int id = entry.a; id <= entry.b; ++id) value[id] -= entry.delta;
            } else {
                solver.undo(entry);
            }
        }
    }
}
