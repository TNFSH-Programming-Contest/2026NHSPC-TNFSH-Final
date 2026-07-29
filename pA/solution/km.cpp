#include <bits/stdc++.h>

using namespace std;

using int64 = long long;

namespace {

const int64 NEG_INF = -(1LL << 50);
const int64 INF = (1LL << 60);

bool hasPerfectMatching(const vector<vector<int> >& adjacency) {
    const int n = adjacency.size();
    vector<int> match_left(n, -1);
    vector<int> match_right(n, -1);
    vector<int> distance(n);

    const auto bfs = [&]() {
        queue<int> queue;
        fill(distance.begin(), distance.end(), -1);
        for (int x = 0; x < n; ++x) {
            if (match_left[x] == -1) {
                distance[x] = 0;
                queue.push(x);
            }
        }

        bool found = false;
        while (!queue.empty()) {
            const int x = queue.front();
            queue.pop();
            for (int y : adjacency[x]) {
                const int next = match_right[y];
                if (next == -1) {
                    found = true;
                } else if (distance[next] == -1) {
                    distance[next] = distance[x] + 1;
                    queue.push(next);
                }
            }
        }
        return found;
    };

    function<bool(int)> dfs = [&](int x) {
        for (int y : adjacency[x]) {
            const int next = match_right[y];
            if (next == -1 ||
                (distance[next] == distance[x] + 1 && dfs(next))) {
                match_left[x] = y;
                match_right[y] = x;
                return true;
            }
        }
        distance[x] = -1;
        return false;
    };

    int matching = 0;
    while (bfs()) {
        for (int x = 0; x < n; ++x) {
            if (match_left[x] == -1 && dfs(x)) {
                ++matching;
            }
        }
    }
    return matching == n;
}

class KuhnMunkres {
public:
    explicit KuhnMunkres(int side)
        : n(side), weight(side, vector<int64>(side, NEG_INF)),
          left_label(side), right_label(side), match_right(side, -1),
          slack(side), seen_left(side), seen_right(side) {}

    void addEdge(int left, int right, int cost) {
        weight[left][right] = max(weight[left][right], -static_cast<int64>(cost));
    }

    pair<bool, int64> run() {
        for (int x = 0; x < n; ++x) {
            left_label[x] = *max_element(weight[x].begin(), weight[x].end());
            if (left_label[x] == NEG_INF) {
                return make_pair(false, 0);
            }
        }

        for (int root = 0; root < n; ++root) {
            fill(slack.begin(), slack.end(), INF);
            while (true) {
                fill(seen_left.begin(), seen_left.end(), false);
                fill(seen_right.begin(), seen_right.end(), false);
                if (augment(root)) {
                    break;
                }

                int64 delta = INF;
                for (int y = 0; y < n; ++y) {
                    if (!seen_right[y]) {
                        delta = min(delta, slack[y]);
                    }
                }
                if (delta == INF) {
                    return make_pair(false, 0);
                }

                for (int x = 0; x < n; ++x) {
                    if (seen_left[x]) {
                        left_label[x] -= delta;
                    }
                }
                for (int y = 0; y < n; ++y) {
                    if (seen_right[y]) {
                        right_label[y] += delta;
                    } else {
                        slack[y] -= delta;
                    }
                }
            }
        }

        int64 answer = 0;
        for (int y = 0; y < n; ++y) {
            if (match_right[y] == -1 || weight[match_right[y]][y] == NEG_INF) {
                return make_pair(false, 0);
            }
            answer -= weight[match_right[y]][y];
        }
        return make_pair(true, answer);
    }

private:
    bool augment(int x) {
        if (seen_left[x]) {
            return false;
        }
        seen_left[x] = true;
        for (int y = 0; y < n; ++y) {
            if (seen_right[y] || weight[x][y] == NEG_INF) {
                continue;
            }

            const int64 difference =
                left_label[x] + right_label[y] - weight[x][y];
            if (difference == 0) {
                seen_right[y] = true;
                if (match_right[y] == -1 || augment(match_right[y])) {
                    match_right[y] = x;
                    return true;
                }
            } else {
                slack[y] = min(slack[y], difference);
            }
        }
        return false;
    }

    int n;
    vector<vector<int64> > weight;
    vector<int64> left_label;
    vector<int64> right_label;
    vector<int> match_right;
    vector<int64> slack;
    vector<char> seen_left;
    vector<char> seen_right;
};

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    KuhnMunkres matching(n - 1);
    vector<vector<int> > adjacency(n - 1);

    for (int i = 0; i < m; ++i) {
        int from, to, cost;
        cin >> from >> to >> cost;
        if (from != n && to != 1) {
            matching.addEdge(from - 1, to - 2, cost);
            adjacency[from - 1].push_back(to - 2);
        }
    }

    if (!hasPerfectMatching(adjacency)) {
        cout << -1 << '\n';
        return 0;
    }

    const pair<bool, int64> result = matching.run();
    cout << (result.first ? result.second : -1) << '\n';
    return 0;
}
