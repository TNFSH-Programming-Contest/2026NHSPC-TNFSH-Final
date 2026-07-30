#include <bits/stdc++.h>

using namespace std;

namespace {

void issueResult(double score, const string& verdict, const string& reason) {
    cout << setprecision(12) << score << '\n';
    cerr << verdict << '\n' << reason << '\n';
}

int optimalWhistles(const vector<int>& permutation) {
    const int n = static_cast<int>(permutation.size()) - 1;
    vector<char> visited(n + 1, false);
    int optimum = 0;

    for (int start = 1; start <= n; ++start) {
        if (visited[start]) {
            continue;
        }
        int length = 0;
        int vertex = start;
        while (!visited[vertex]) {
            visited[vertex] = true;
            vertex = permutation[vertex];
            ++length;
        }
        if (length == 2) {
            optimum = max(optimum, 1);
        } else if (length >= 3) {
            optimum = 2;
        }
    }
    return optimum;
}

bool isSorted(const vector<int>& permutation) {
    for (int position = 1;
         position < static_cast<int>(permutation.size()); ++position) {
        if (permutation[position] != position) {
            return false;
        }
    }
    return true;
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 4) {
        issueResult(0, "Judge Failure", "manager expects one solution process");
        return 0;
    }

    int n;
    if (!(cin >> n) || n < 1 || n > 100000) {
        issueResult(0, "Judge Failure", "invalid n in judge input");
        return 0;
    }

    vector<int> initial(n + 1);
    vector<int> frequency(n + 1, 0);
    for (int position = 1; position <= n; ++position) {
        if (!(cin >> initial[position]) ||
            initial[position] < 1 || initial[position] > n) {
            issueResult(0, "Judge Failure", "invalid permutation in judge input");
            return 0;
        }
        ++frequency[initial[position]];
    }
    for (int value = 1; value <= n; ++value) {
        if (frequency[value] != 1) {
            issueResult(0, "Judge Failure", "judge input is not a permutation");
            return 0;
        }
    }

    ofstream toSolution(argv[2]);
    if (!toSolution) {
        issueResult(0, "Judge Failure", "cannot open manager-to-solution pipe");
        return 0;
    }
    ifstream fromSolution(argv[1]);
    if (!fromSolution) {
        issueResult(0, "Judge Failure", "cannot open solution-to-manager pipe");
        return 0;
    }
    ofstream transcript(argv[3]);

    toSolution << n << '\n';
    for (int position = 1; position <= n; ++position) {
        if (position != 1) toSolution << ' ';
        toSolution << initial[position];
    }
    toSolution << '\n' << flush;

    vector<int> permutation = initial;
    vector<pair<int, int> > pending;
    vector<char> used(n + 1, false);
    int whistles = 0;
    bool done = false;
    string failure;
    string line;

    const auto fail = [&](const string& message) {
        if (failure.empty()) {
            failure = message;
        }
    };

    while (getline(fromSolution, line)) {
        if (transcript) transcript << line << '\n';

        istringstream parser(line);
        string command;
        parser >> command;

        if (command == "SWAP") {
            int u, v;
            string extra;
            if (!(parser >> u >> v) || (parser >> extra)) {
                fail("invalid SWAP command");
                continue;
            }
            if (u < 1 || u > n || v < 1 || v > n || u == v) {
                fail("swap_student arguments are out of range");
                continue;
            }
            if (used[u] || used[v]) {
                fail("Wrong Answer(1): a student is swapped twice in one round");
                continue;
            }
            used[u] = used[v] = true;
            pending.push_back({u, v});
        } else if (command == "BLOW") {
            string extra;
            if (parser >> extra) {
                fail("invalid BLOW command");
                continue;
            }
            ++whistles;
            if (whistles > 2 * n) {
                fail("Wrong Answer(2): too many whistle calls");
            }
            for (const pair<int, int>& operation : pending) {
                swap(permutation[operation.first], permutation[operation.second]);
            }
            pending.clear();
            fill(used.begin(), used.end(), false);
        } else if (command == "DONE") {
            string extra;
            if (parser >> extra) {
                fail("invalid DONE command");
            }
            done = true;
            break;
        } else {
            fail("unknown grader protocol command");
        }
    }

    if (!done) {
        fail("solution terminated without DONE");
    }
    if (!failure.empty()) {
        issueResult(0, "Wrong Answer", failure);
        return 0;
    }
    if (!isSorted(permutation)) {
        issueResult(0, "Wrong Answer", "the final permutation is not sorted");
        return 0;
    }

    const int optimum = optimalWhistles(initial);
    if (whistles < optimum) {
        issueResult(0, "Judge Failure", "solution beat the computed optimum");
        return 0;
    }
    if (whistles == optimum) {
        issueResult(1, "Correct", "used the optimal number of whistles");
        return 0;
    }

    const double score =
        max(0.0, 0.5 - static_cast<double>(whistles - optimum) * 0.02);
    issueResult(score, "Partially Correct",
                "sorted successfully, but used extra whistles");
    return 0;
}
