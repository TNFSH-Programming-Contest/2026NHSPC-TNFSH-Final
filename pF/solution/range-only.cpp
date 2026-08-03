#include <bits/stdc++.h>
using namespace std;

class Fenwick {
public:
    explicit Fenwick(int n) : bit_(n + 1, 0) {}

    void add(int position, long long delta) {
        for (int i = position; i < static_cast<int>(bit_.size()); i += i & -i) {
            bit_[i] += delta;
        }
    }

    long long query(int position) const {
        long long result = 0;
        for (int i = position; i > 0; i -= i & -i) result += bit_[i];
        return result;
    }

private:
    vector<long long> bit_;
};

struct Update {
    int left;
    int right;
    long long delta;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    vector<int> coordinates;
    coordinates.reserve(n);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        coordinates.push_back(a[i]);
    }
    sort(coordinates.begin(), coordinates.end());
    coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());

    Fenwick difference(static_cast<int>(coordinates.size()) + 1);
    vector<Update> history;
    history.reserve(q);

    auto apply = [&](int left, int right, long long delta) {
        difference.add(left + 1, delta);
        difference.add(right + 2, -delta);
    };

    while (q--) {
        int type;
        cin >> type;
        if (type == 1 || type == 2) {
            int x, y;
            cin >> x >> y;
            return 0;  // This operation is excluded by the subtask.
        }
        if (type == 3) {
            int leftValue, rightValue;
            long long delta;
            cin >> leftValue >> rightValue >> delta;
            const int left = lower_bound(coordinates.begin(), coordinates.end(), leftValue) -
                             coordinates.begin();
            const int right = upper_bound(coordinates.begin(), coordinates.end(), rightValue) -
                              coordinates.begin() - 1;
            if (delta == 0 || left > right) continue;
            apply(left, right, delta);
            history.push_back({left, right, delta});
        } else if (type == 4) {
            int vertex;
            cin >> vertex;
            const int position = lower_bound(coordinates.begin(), coordinates.end(), a[vertex]) -
                                 coordinates.begin();
            cout << difference.query(position + 1) << '\n';
        } else if (type == 5) {
            const Update update = history.back();
            history.pop_back();
            apply(update.left, update.right, -update.delta);
        }
    }
}
