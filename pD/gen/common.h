#pragma once

constexpr int MAX_VALUE = 1000000000;

struct Instance {
    int n, h, k;
    std::vector<int> u, w, current;
    std::vector<std::pair<int, int>> commits;

    Instance(int n_, int h_, int k_)
        : n(n_), h(h_), k(k_), u(h_ + 1), w(h_ + 1), current(h_ + 1, 0) {}

    int otherValue(int hunk, int variant) const {
        long long z = 1LL * hunk * 1000003 + 1LL * variant * 9176 + 1234567;
        int value = static_cast<int>(z % MAX_VALUE) + 1;
        while (value == u[hunk] || value == current[hunk])
            value = value == MAX_VALUE ? 1 : value + 1;
        return value;
    }

    void add(int hunk, int value) {
        if (hunk < 1 || hunk > h || value < 1 || value > MAX_VALUE ||
            value == current[hunk]) {
            std::cerr << "generator produced an invalid commit\n";
            std::exit(1);
        }
        commits.push_back({hunk, value});
        current[hunk] = value;
    }

    void print() const {
        if (static_cast<int>(commits.size()) != n) {
            std::cerr << "generator produced the wrong number of commits\n";
            std::exit(1);
        }
        std::cout << n << ' ' << h << ' ' << k << '\n';
        for (int i = 1; i <= h; ++i)
            std::cout << (i == 1 ? "" : " ") << u[i];
        std::cout << '\n';
        for (int i = 1; i <= h; ++i)
            std::cout << (i == 1 ? "" : " ") << w[i];
        std::cout << '\n';
        for (const auto& commit : commits)
            std::cout << commit.first << ' ' << commit.second << '\n';
    }
};

inline void shuffleVector(std::vector<int>& values) {
    for (int i = static_cast<int>(values.size()) - 1; i > 0; --i)
        std::swap(values[i], values[rnd.next(0, i)]);
}

inline void initialize(Instance& in, bool overflow) {
    static const int special[] = {1, 2, 3, 999999937, 999999999, 1000000000};
    for (int h = 1; h <= in.h; ++h) {
        if (overflow) {
            in.u[h] = 1;
            in.w[h] = MAX_VALUE;
        } else {
            in.u[h] = rnd.next(0, 4) == 0
                ? special[rnd.next(0, 5)] : rnd.next(1, MAX_VALUE);
            in.w[h] = rnd.next(0, 4) <= 1
                ? special[rnd.next(0, 5)] : rnd.next(1, MAX_VALUE);
        }
    }
}

inline int cycleValue(Instance& in, int hunk, int phase) {
    int kind = (phase + hunk) % 4;
    if (kind == 0 && in.current[hunk] != in.u[hunk]) return in.u[hunk];
    return in.otherValue(hunk, phase + kind * 100003);
}

inline Instance makeInstance(int argc, char* argv[], int& active,
                             bool overflow = false) {
    if (argc < 5) {
        std::cerr << "usage: GENERATOR MODE N H K [ACTIVE] [seed-tag]\n";
        std::exit(1);
    }
    int n = std::stoi(argv[2]);
    int h = std::stoi(argv[3]);
    int k = std::stoi(argv[4]);
    active = argc >= 6 ? std::stoi(argv[5]) : std::min(n, h);
    if (n < 1 || n > 200000 || h < 1 || h > 200000 ||
        k < 1 || k > std::min(n, 30) ||
        active < 1 || active > h || active > n) {
        std::cerr << "invalid generator arguments\n";
        std::exit(1);
    }
    Instance in(n, h, k);
    initialize(in, overflow);
    return in;
}
