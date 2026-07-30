#pragma once

#include <bits/stdc++.h>
#include "testlib.h"

using int64 = long long;

constexpr int MAX_N = 100000;
constexpr int MAX_M = 100000;
constexpr int64 MIN_X = -1000000000LL;
constexpr int64 MAX_X = 1000000000LL;
constexpr int64 MAX_A = 1000000000LL;

struct Instance {
    int n;
    int64 m;
    std::vector<int64> x;
    std::vector<int64> capacity;

    Instance(int n_, int64 m_)
        : n(n_), m(m_), x(n_), capacity(n_, 0) {
        if (n < 3 || n > MAX_N) {
            fail("n is out of range");
        }
        if (m < 1 || m > MAX_M) {
            fail("m is out of range");
        }
    }

    [[noreturn]] static void fail(const std::string& message) {
        std::cerr << "generator error: " << message << '\n';
        std::exit(1);
    }

    int64 required() const {
        return 2 * m;
    }

    void setCoordinatesFromWeights(const std::vector<int64>& weight) {
        if (static_cast<int>(weight.size()) != n - 1) {
            fail("wrong number of gap weights");
        }

        __int128 totalWeight = 0;
        for (int64 value : weight) {
            if (value <= 0) {
                fail("gap weight must be positive");
            }
            totalWeight += value;
        }

        const int64 extra = (MAX_X - MIN_X) - (n - 1);
        __int128 prefixWeight = 0;
        x[0] = MIN_X;
        for (int i = 1; i < n; ++i) {
            prefixWeight += weight[i - 1];
            const int64 distributed = static_cast<int64>(
                static_cast<__int128>(extra) * prefixWeight / totalWeight);
            x[i] = MIN_X + i + distributed;
        }
    }

    void setCoordinates(const std::string& style) {
        if (style == "unit") {
            const int64 first = -n / 2;
            for (int i = 0; i < n; ++i) {
                x[i] = first + i;
            }
            return;
        }

        if (style == "compressed-random") {
            x[0] = -500000000;
            for (int i = 1; i < n; ++i) {
                x[i] = x[i - 1] + rnd.next(1, 100);
            }
            return;
        }

        std::vector<int64> weight(n - 1, 1);
        if (style == "full-uniform") {
            // Already all one.
        } else if (style == "random") {
            for (int64& value : weight) {
                value = rnd.next(1, 1000000000);
            }
        } else if (style == "alternating") {
            for (int i = 0; i + 1 < n; ++i) {
                weight[i] = (i % 2 == 0 ? 1 : 1000000000000LL);
            }
        } else if (style == "increasing") {
            for (int i = 0; i + 1 < n; ++i) {
                weight[i] = i + 1;
            }
        } else if (style == "decreasing") {
            for (int i = 0; i + 1 < n; ++i) {
                weight[i] = n - i;
            }
        } else if (style == "gap-left") {
            weight.front() = 1000000000000000LL;
        } else if (style == "gap-middle") {
            weight[(n - 2) / 2] = 1000000000000000LL;
        } else if (style == "gap-right") {
            weight.back() = 1000000000000000LL;
        } else if (style == "clusters") {
            const int step = std::max(1, (n - 1) / 10);
            for (int i = step - 1; i + 1 < n; i += step) {
                weight[i] = 1000000000000LL;
            }
        } else {
            fail("unknown coordinate style: " + style);
        }

        setCoordinatesFromWeights(weight);
    }

    void setCapacities(const std::string& style) {
        const int64 need = required();

        if (style == "budget") {
            for (int i = 1; i + 1 < n; ++i) {
                capacity[i] = 1;
            }
            int remaining = 10000 - (n - 2);
            while (remaining > 0) {
                const int index = rnd.next(1, n - 2);
                const int add = rnd.next(1, std::min(remaining, 200));
                capacity[index] += add;
                remaining -= add;
            }
            return;
        }

        for (int i = 1; i + 1 < n; ++i) {
            if (style == "one") {
                capacity[i] = 1;
            } else if (style == "max") {
                capacity[i] = MAX_A;
            } else if (style == "random") {
                const int choice = rnd.next(0, 6);
                if (choice == 0) {
                    capacity[i] = 1;
                } else if (choice == 1) {
                    capacity[i] = std::max<int64>(1, need - 1);
                } else if (choice == 2) {
                    capacity[i] = need;
                } else if (choice == 3) {
                    capacity[i] = std::min<int64>(MAX_A, need + 1);
                } else if (choice == 4) {
                    capacity[i] = MAX_A;
                } else {
                    capacity[i] = rnd.next(1, 1000000000);
                }
            } else if (style == "small") {
                capacity[i] = rnd.next(1, 100);
            } else if (style == "threshold") {
                static const int offset[] = {-1, 0, 1, 0, -1, 1};
                capacity[i] = std::max<int64>(
                    1, std::min<int64>(MAX_A, need + offset[i % 6]));
            } else if (style == "exact") {
                capacity[i] = need;
            } else if (style == "below") {
                capacity[i] = std::max<int64>(1, need - 1);
            } else if (style == "alternating") {
                capacity[i] = (i % 2 == 0 ? 1 : MAX_A);
            } else if (style == "blocks") {
                const int phase = static_cast<int>(10LL * i / n);
                capacity[i] = (phase % 3 == 1)
                    ? std::max<int64>(1, need / 7)
                    : (phase % 3 == 2 ? MAX_A : 1);
            } else if (style == "spike") {
                capacity[i] = (i == n / 2 ? MAX_A : 1);
            } else if (style == "increasing") {
                capacity[i] = std::min<int64>(
                    MAX_A, 1 + static_cast<int64>(i) * need / n);
            } else if (style == "decreasing") {
                capacity[i] = std::min<int64>(
                    MAX_A, 1 + static_cast<int64>(n - i) * need / n);
            } else {
                fail("unknown capacity style: " + style);
            }
        }

    }

    void verifyAndPrint() const {
        if (x.front() < MIN_X || x.back() > MAX_X) {
            fail("coordinate is out of range");
        }
        for (int i = 1; i < n; ++i) {
            if (x[i - 1] >= x[i]) {
                fail("coordinates are not strictly increasing");
            }
        }
        for (int i = 1; i + 1 < n; ++i) {
            if (capacity[i] < 1 || capacity[i] > MAX_A) {
                fail("capacity is out of range");
            }
        }

        std::cout << n << ' ' << m << '\n';
        for (int i = 0; i < n; ++i) {
            if (i != 0) {
                std::cout << ' ';
            }
            std::cout << x[i];
        }
        std::cout << '\n';

        for (int i = 1; i + 1 < n; ++i) {
            if (i != 1) {
                std::cout << ' ';
            }
            std::cout << capacity[i];
        }
        std::cout << '\n';
    }
};

inline int64 parseInteger(const char* text, const std::string& name) {
    try {
        std::size_t consumed = 0;
        const int64 value = std::stoll(text, &consumed);
        if (consumed != std::strlen(text)) {
            Instance::fail("invalid integer argument " + name);
        }
        return value;
    } catch (...) {
        Instance::fail("invalid integer argument " + name);
    }
}
