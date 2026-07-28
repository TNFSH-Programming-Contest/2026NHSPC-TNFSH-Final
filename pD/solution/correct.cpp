#include <bits/stdc++.h>
using namespace std;

using loli = long long;
const loli INF = (1LL << 62);

loli minimum_[200005 * 4];
loli lazy_[200005 * 4];

class SegmentTree {
public:
    SegmentTree(int n) : n_(n) {}

    void build(const std::vector<loli>& values) {
        build(1, 0, n_ - 1, values);
    }

    void range_add(int left, int right, loli delta) {
        if (left > right) return;
        range_add(1, 0, n_ - 1, left, right, delta);
    }

    loli range_min(int left, int right) {
        if (left > right) return INF;
        return range_min(1, 0, n_ - 1, left, right);
    }

private:
    int n_;

    void apply(int node, loli delta) {
        minimum_[node] += delta;
        lazy_[node] += delta;
    }

    void push(int node) {
        if (lazy_[node] == 0) return;
        apply(node * 2, lazy_[node]);
        apply(node * 2 + 1, lazy_[node]);
        lazy_[node] = 0;
    }

    void pull(int node) {
        minimum_[node] = min(minimum_[node * 2], minimum_[node * 2 + 1]);
    }

    void build(int node, int left, int right, const vector<loli>& values) {
        lazy_[node] = 0;
        if (left == right) {
            minimum_[node] = values[left];
            return;
        }

        int middle = (left + right) / 2;
        build(node * 2, left, middle, values);
        build(node * 2 + 1, middle + 1, right, values);
        pull(node);
    }

    void range_add(int node, int left, int right,
                   int query_left, int query_right, loli delta) {
        if (query_left <= left && right <= query_right) {
            apply(node, delta);
            return;
        }

        push(node);
        int middle = (left + right) / 2;

        if (query_left <= middle) {
            range_add(node * 2, left, middle, query_left, query_right, delta);
        }
        if (middle < query_right) {
            range_add(node * 2 + 1, middle + 1, right,
                      query_left, query_right, delta);
        }

        pull(node);
    }

    loli range_min(int node, int left, int right,
                    int query_left, int query_right) {
        if (query_left <= left && right <= query_right) {
            return minimum_[node];
        }

        push(node);
        int middle = (left + right) / 2;
        loli answer = INF;

        if (query_left <= middle) {
            answer = min(answer,
                         range_min(node * 2, left, middle,
                                   query_left, query_right));
        }
        if (middle < query_right) {
            answer = min(answer,
                         range_min(node * 2 + 1, middle + 1, right,
                                   query_left, query_right));
        }

        return answer;
    }
};

#define nitrogen std::ios::sync_with_stdio(false); std::cin.tie(nullptr);

int main() {
    nitrogen;

    int N, H, K;
    cin >> N >> H >> K;

    std::vector<int> U(H + 1);
    std::vector<loli> w(H + 1);
    for (int h = 1; h <= H; ++h) std::cin >> U[h];
    for (int h = 1; h <= H; ++h) std::cin >> w[h];

    std::vector<int> x(N + 1), v(N + 1);
    for (int i = 1; i <= N; ++i) {
        std::cin >> x[i] >> v[i];
    }

    // While sweeping the right endpoint r, commit r changes every candidate
    // cut j in [0, right[r]] by delta[r].
    std::vector<int> right(N + 1, -1);
    std::vector<loli> delta(N + 1, 0);

    std::vector<int> first(H + 1, 0);
    std::vector<int> current(H + 1, 0);

    for (int r = 1; r <= N; ++r) {
        int h = x[r];
        int old_value = current[h];
        int new_value = v[r];

        if (first[h] == 0) {
            first[h] = r;
            right[r] = r - 1;

            if (new_value != U[h]) {
                delta[r] = w[h];
            }
        } else {
            right[r] = first[h] - 1;

            bool old_bad = (old_value != U[h]);
            bool new_bad = (new_value != U[h]);

            if (!old_bad && new_bad) {
                delta[r] = w[h];
            } else if (old_bad && !new_bad) {
                delta[r] = -w[h];
            }
        }

        current[h] = new_value;
    }

    std::vector<loli> prev(N + 1, INF), cur(N + 1, INF);
    prev[0] = 0;

    SegmentTree tree(N);  // Candidate cut positions are 0, 1, ..., N - 1.
    std::vector<loli> base(N);

    for (int groups = 1; groups <= K; ++groups) {
        for (int j = 0; j < N; ++j) {
            base[j] = prev[j];
        }
        tree.build(base);

        std::fill(cur.begin(), cur.end(), INF);

        for (int r = 1; r <= N; ++r) {
            if (delta[r] != 0) {
                tree.range_add(0, right[r], delta[r]);
            }

            if (r >= groups) {
                cur[r] = tree.range_min(groups - 1, r - 1);
            }
        }

        prev.swap(cur);
    }

    std::cout << prev[N] << '\n';

    return 0;
}
