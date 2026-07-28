#include <bits/stdc++.h>
using namespace std;

#ifdef FAKE_INT32
using Cost = int;
const Cost INF = 2000000000;
#else
using Cost = long long;
const Cost INF = (1LL << 62);
#endif

struct SegmentTree {
    int n;
    vector<Cost> minimum, lazy;

    explicit SegmentTree(int n_) : n(n_), minimum(4 * n), lazy(4 * n) {}

    void apply(int p, Cost value) {
        minimum[p] += value;
        lazy[p] += value;
    }
    void push(int p) {
        if (lazy[p] == 0) return;
        apply(p * 2, lazy[p]);
        apply(p * 2 + 1, lazy[p]);
        lazy[p] = 0;
    }
    void pull(int p) {
        minimum[p] = min(minimum[p * 2], minimum[p * 2 + 1]);
    }
    void build(int p, int l, int r, const vector<Cost>& value) {
        lazy[p] = 0;
        if (l == r) {
            minimum[p] = value[l];
            return;
        }
        int m = (l + r) / 2;
        build(p * 2, l, m, value);
        build(p * 2 + 1, m + 1, r, value);
        pull(p);
    }
    void build(const vector<Cost>& value) {
        build(1, 0, n - 1, value);
    }
    void add(int p, int l, int r, int ql, int qr, Cost value) {
        if (ql <= l && r <= qr) {
            apply(p, value);
            return;
        }
        push(p);
        int m = (l + r) / 2;
        if (ql <= m) add(p * 2, l, m, ql, qr, value);
        if (m < qr) add(p * 2 + 1, m + 1, r, ql, qr, value);
        pull(p);
    }
    void add(int l, int r, Cost value) {
        if (l <= r) add(1, 0, n - 1, l, r, value);
    }
    Cost query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return minimum[p];
        push(p);
        int m = (l + r) / 2;
        Cost result = INF;
        if (ql <= m) result = min(result, query(p * 2, l, m, ql, qr));
        if (m < qr) result = min(result, query(p * 2 + 1, m + 1, r, ql, qr));
        return result;
    }
    Cost query(int l, int r) {
        return l <= r ? query(1, 0, n - 1, l, r) : INF;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, h, k;
    cin >> n >> h >> k;
    vector<int> target(h + 1);
    vector<Cost> weight(h + 1);
    for (int i = 1; i <= h; ++i) cin >> target[i];
    for (int i = 1; i <= h; ++i) cin >> weight[i];

    vector<int> first(h + 1), current(h + 1);
    vector<int> right(n + 1, -1);
    vector<Cost> delta(n + 1);
    for (int i = 1; i <= n; ++i) {
        int x, value;
        cin >> x >> value;
        int old = current[x];
        if (first[x] == 0) {
            first[x] = i;
            right[i] = i - 1;
            if (value != target[x]) delta[i] = weight[x];
        } else {
#ifdef FAKE_WRONG_PREFIX
            right[i] = i - 1;
#else
            right[i] = first[x] - 1;
#endif
            bool oldBad = old != target[x];
            bool newBad = value != target[x];
            if (!oldBad && newBad) delta[i] = weight[x];
            else if (oldBad && !newBad) {
#ifndef FAKE_NO_NEGATIVE_DELTA
                delta[i] = -weight[x];
#endif
            }
        }
        current[x] = value;
    }

    vector<Cost> previous(n + 1, INF), next(n + 1), base(n);
    previous[0] = 0;
    SegmentTree tree(n);
    for (int groups = 1; groups <= k; ++groups) {
        copy(previous.begin(), previous.begin() + n, base.begin());
        tree.build(base);
        fill(next.begin(), next.end(), INF);
        for (int r = 1; r <= n; ++r) {
            tree.add(0, right[r], delta[r]);
            if (r >= groups) next[r] = tree.query(groups - 1, r - 1);
        }
        previous.swap(next);
    }
    cout << previous[n] << '\n';
}
