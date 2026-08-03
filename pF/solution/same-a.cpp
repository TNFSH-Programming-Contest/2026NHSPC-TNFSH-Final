#include <bits/stdc++.h>
using namespace std;

struct History {
    int type;
    int a;
    int b;
    int c;
    int d;
    int e;
    long long delta;
};

class RollbackComponents {
public:
    RollbackComponents(int n, int q)
        : position_(n + 1),
          parent_(n + q + 5),
          treeSize_(n + q + 5, 1),
          activeSize_(n + q + 5, 0),
          nextNode_(n + 1) {
        iota(parent_.begin(), parent_.end(), 0);
        for (int i = 1; i <= n; ++i) {
            position_[i] = i;
            activeSize_[i] = 1;
        }
    }

    int rootOf(int vertex) const {
        int node = position_[vertex];
        while (parent_[node] != node) node = parent_[node];
        return node;
    }

    bool unite(int x, int y, vector<History>& history) {
        int a = rootOf(x), b = rootOf(y);
        if (a == b) return false;
        if (treeSize_[a] > treeSize_[b]) swap(a, b);
        history.push_back({1, a, b, treeSize_[a], activeSize_[a], 0, 0});
        parent_[a] = b;
        treeSize_[b] += treeSize_[a];
        activeSize_[b] += activeSize_[a];
        return true;
    }

    bool moveVertex(int vertex, int target, vector<History>& history) {
        const int sourceRoot = rootOf(vertex);
        const int targetRoot = rootOf(target);
        if (sourceRoot == targetRoot) return false;

        const int oldNode = position_[vertex];
        const int newNode = nextNode_++;
        history.push_back({2, vertex, oldNode, newNode, sourceRoot, targetRoot, 0});

        --activeSize_[sourceRoot];
        ++activeSize_[targetRoot];
        position_[vertex] = newNode;
        parent_[newNode] = targetRoot;
        treeSize_[newNode] = 1;
        ++treeSize_[targetRoot];
        return true;
    }

    int sizeOf(int vertex) const { return activeSize_[rootOf(vertex)]; }

    void undo(const History& entry) {
        if (entry.type == 1) {
            parent_[entry.a] = entry.a;
            treeSize_[entry.b] -= entry.c;
            activeSize_[entry.b] -= entry.d;
        } else if (entry.type == 2) {
            position_[entry.a] = entry.b;
            ++activeSize_[entry.d];
            --activeSize_[entry.e];
            --treeSize_[entry.e];
            parent_[entry.c] = entry.c;
            treeSize_[entry.c] = 1;
            --nextNode_;
        }
    }

private:
    vector<int> position_;
    vector<int> parent_;
    vector<int> treeSize_;
    vector<int> activeSize_;
    int nextNode_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];
    const int commonValue = a[1];

    RollbackComponents components(n, q);
    vector<History> history;
    history.reserve(q);
    long long value = 0;

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int x, y;
            cin >> x >> y;
            components.unite(x, y, history);
        } else if (type == 2) {
            int x, y;
            cin >> x >> y;
            components.moveVertex(x, y, history);
        } else if (type == 3) {
            int left, right;
            long long delta;
            cin >> left >> right >> delta;
            if (delta != 0 && left <= commonValue && commonValue <= right) {
                value += delta;
                history.push_back({3, 0, 0, 0, 0, 0, delta});
            }
        } else if (type == 4) {
            int vertex;
            cin >> vertex;
            cout << static_cast<long long>(components.sizeOf(vertex)) * value << '\n';
        } else {
            if (history.empty()) return 0;
            const History entry = history.back();
            history.pop_back();
            if (entry.type == 3) value -= entry.delta;
            else components.undo(entry);
        }
    }
}
