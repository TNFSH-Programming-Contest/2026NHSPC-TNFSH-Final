#pragma once

#include <bits/stdc++.h>
#include "jngen.h"

constexpr int MAX_N = 2000;
constexpr int MAX_M = 10000;
constexpr int MAX_COST = 100000;

struct Road {
    int from;
    int to;
    int cost;
};

struct Instance {
    int n;
    std::vector<Road> roads;
    std::set<std::pair<int, int> > used;

    explicit Instance(int vertex_count) : n(vertex_count) {
        if (n < 2 || n > MAX_N) fail("n is out of range");
    }

    [[noreturn]] static void fail(const std::string& message) {
        std::cerr << "generator error: " << message << '\n';
        std::exit(1);
    }

    bool addUnique(int from, int to, int cost = 0) {
        if (from < 1 || from > n || to < 1 || to > n || from == to)
            return false;
        if (!used.insert(std::make_pair(from, to)).second)
            return false;
        roads.push_back({from, to, cost});
        return true;
    }

    void add(int from, int to, int cost) {
        if (!addUnique(from, to, cost))
            fail("invalid or duplicate road");
    }

    void addCanonicalMatching() {
        for (int v = 1; v < n; ++v)
            add(v, v + 1, 0);
    }

    void assignCosts(const std::string& style, int maximum) {
        if (maximum < 0 || maximum > MAX_COST)
            fail("maximum cost is out of range");

        const int count = roads.size();
        for (int i = 0; i < count; ++i) {
            if (style == "zero")
                roads[i].cost = 0;
            else if (style == "one")
                roads[i].cost = 1;
            else if (style == "maximum")
                roads[i].cost = maximum;
            else if (style == "alternating")
                roads[i].cost = i % 2 == 0 ? 0 : maximum;
            else if (style == "increasing")
                roads[i].cost = count <= 1 ? maximum
                    : static_cast<long long>(i) * maximum / (count - 1);
            else if (style == "decreasing")
                roads[i].cost = count <= 1 ? maximum
                    : static_cast<long long>(count - 1 - i) * maximum / (count - 1);
            else if (style == "random")
                roads[i].cost = rnd.next(0, maximum);
            else
                fail("unknown cost style: " + style);
        }
    }

    void print(const std::string& order = "keep") {
        if (roads.empty() || roads.size() > MAX_M)
            fail("m is out of range");
        for (const Road& road : roads) {
            if (road.cost < 0 || road.cost > MAX_COST)
                fail("road cost is out of range");
        }

        if (order == "shuffle")
            jngen::shuffle(roads.begin(), roads.end());
        else if (order != "keep")
            fail("unknown order: " + order);

        std::cout << n << ' ' << roads.size() << '\n';
        for (const Road& road : roads)
            std::cout << road.from << ' ' << road.to << ' '
                      << road.cost << '\n';
    }
};

inline int parseInt(const char* text, const std::string& name) {
    try {
        std::size_t consumed = 0;
        long long value = std::stoll(text, &consumed);
        if (consumed != std::strlen(text) || value < INT_MIN || value > INT_MAX)
            Instance::fail("invalid integer argument " + name);
        return static_cast<int>(value);
    } catch (...) {
        Instance::fail("invalid integer argument " + name);
    }
}

inline void requireArguments(int argc, int minimum, const std::string& usage) {
    if (argc < minimum) Instance::fail("usage: " + usage);
}
