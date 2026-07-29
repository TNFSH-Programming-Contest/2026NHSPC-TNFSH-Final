#pragma once

#include <bits/stdc++.h>
#include "jngen.h"

constexpr int MAX_N = 100000;
constexpr int MAX_M = 200000;
constexpr int MAX_COST = 1000000000;
constexpr long long MAX_R = 1000000000000000000LL;

enum EdgeRole {
    BACKBONE,
    EXTRA,
    LOOP,
    SPECIAL
};

struct Edge {
    int from;
    int to;
    long long cost;
    EdgeRole role;
};

struct Instance {
    int n;
    int targetM;
    std::vector<Edge> edges;
    std::vector<int> path;

    explicit Instance(int n_, int targetM_ = -1)
        : n(n_), targetM(targetM_) {
        if (n < 1 || n > MAX_N) fail("N is out of range");
        if (targetM != -1 && (targetM < 1 || targetM > MAX_M))
            fail("M is out of range");
    }

    [[noreturn]] static void fail(const std::string& message) {
        std::cerr << "generator error: " << message << '\n';
        std::exit(1);
    }

    int addEdge(int from, int to, EdgeRole role = EXTRA,
                long long cost = 0) {
        if (from < 1 || from > n || to < 1 || to > n)
            fail("edge endpoint is out of range");
        edges.push_back({from, to, cost, role});
        return static_cast<int>(edges.size()) - 1;
    }

    int addEdge(int from, int to, int role) {
        if (role < BACKBONE || role > SPECIAL)
            fail("edge role is out of range");
        return addEdge(from, to, static_cast<EdgeRole>(role));
    }

    void addBackbone(bool permuteInternalVertices) {
        path.resize(n);
        std::iota(path.begin(), path.end(), 1);
        if (permuteInternalVertices && n > 3)
            jngen::shuffle(path.begin() + 1, path.end() - 1);
        for (int i = 0; i + 1 < n; ++i)
            addEdge(path[i], path[i + 1], BACKBONE);
    }

    void assignWeights(const std::string& style) {
        static const int specials[] = {
            1, 2, 3, 999999937, 999999999, 1000000000
        };

        for (int i = 0; i < static_cast<int>(edges.size()); ++i) {
            Edge& edge = edges[i];
            if (style == "keep") {
                if (edge.cost < 1 || edge.cost > MAX_COST)
                    fail("keep weight is out of range");
            } else if (style == "one") {
                edge.cost = 1;
            } else if (style == "max") {
                edge.cost = MAX_COST;
            } else if (style == "overflow") {
                edge.cost = MAX_COST;
            } else if (style == "random") {
                edge.cost = rnd.next(0, 3) == 0
                    ? specials[rnd.next(0, 5)]
                    : rnd.next(1, MAX_COST);
            } else if (style == "increasing") {
                edge.cost = i % MAX_COST + 1;
            } else if (style == "decreasing") {
                edge.cost = MAX_COST - i % MAX_COST;
            } else if (style == "alternating") {
                edge.cost = i % 2 == 0 ? 1 : MAX_COST;
            } else if (style == "late-heavy") {
                edge.cost = i * 3 >= static_cast<int>(edges.size()) * 2
                    ? MAX_COST - i % 1009 : 1 + i % 17;
            } else if (style == "loop-heavy") {
                edge.cost = edge.role == LOOP
                    ? MAX_COST - i % 1009 : 1 + i % 13;
            } else if (style == "ties") {
                static const int tied[] = {1, 1, 2, 2, 7, 7, 1000000000};
                edge.cost = tied[i % 7];
            } else if (style == "backbone-low") {
                edge.cost = edge.role == BACKBONE
                    ? 1 + i % 7 : MAX_COST - i % 997;
            } else if (style == "backbone-high") {
                edge.cost = edge.role == BACKBONE
                    ? MAX_COST - i % 997 : 1 + i % 7;
            } else if (style == "top-cycle") {
                if (edge.role == SPECIAL)
                    edge.cost = MAX_COST - i % 17;
                else if (edge.role == BACKBONE)
                    edge.cost = 100 + i % 31;
                else
                    edge.cost = 100000 + i % 1009;
            } else {
                fail("unknown weight style: " + style);
            }
        }
    }

    void shuffleEdges() {
        jngen::shuffle(edges.begin(), edges.end());
    }

    void verifyAndPrint(int expectedM, const std::string& rStyle) const {
        if (expectedM < 1 || expectedM > MAX_M)
            fail("M is out of range");
        if (static_cast<int>(edges.size()) != expectedM)
            fail("wrong number of generated edges");

        std::vector<std::vector<int>> out(n + 1), in(n + 1);
        for (const Edge& edge : edges) {
            if (edge.cost < 1 || edge.cost > MAX_COST)
                fail("edge cost is out of range");
            out[edge.from].push_back(edge.to);
            in[edge.to].push_back(edge.from);
        }

        auto reachable = [&](int source,
                             const std::vector<std::vector<int>>& graph) {
            std::vector<char> seen(n + 1, false);
            std::queue<int> que;
            seen[source] = true;
            que.push(source);
            while (!que.empty()) {
                int u = que.front();
                que.pop();
                for (int v : graph[u]) {
                    if (!seen[v]) {
                        seen[v] = true;
                        que.push(v);
                    }
                }
            }
            return seen;
        };

        const std::vector<char> fromEntry = reachable(1, out);
        const std::vector<char> toExit = reachable(n, in);
        for (int v = 1; v <= n; ++v) {
            if (!fromEntry[v] || !toExit[v])
                fail("a BB is not on a directed path from 1 to N");
        }

        long long r;
        if (rStyle == "min")
            r = 2LL * expectedM;
        else if (rStyle == "max")
            r = MAX_R;
        else if (rStyle == "near-max")
            r = MAX_R - expectedM;
        else if (rStyle == "random")
            r = MAX_R / 3 + 1000003LL * expectedM;
        else
            fail("unknown R style: " + rStyle);

        std::cout << n << ' ' << expectedM << ' ' << r << '\n';
        for (const Edge& edge : edges)
            std::cout << edge.from << ' ' << edge.to << ' '
                      << edge.cost << '\n';
    }


    void print(const std::string& weightStyle,
               const std::string& rStyle,
               const std::string& orderStyle) {
        assignWeights(weightStyle);
        if (orderStyle == "shuffle")
            shuffleEdges();
        else if (orderStyle != "keep")
            fail("unknown edge order style: " + orderStyle);
        verifyAndPrint(targetM, rStyle);
    }
};

inline int parseInt(const char* text, const std::string& name) {
    try {
        size_t consumed = 0;
        long long value = std::stoll(text, &consumed);
        if (consumed != std::strlen(text) || value < INT_MIN || value > INT_MAX)
            Instance::fail("invalid integer argument " + name);
        return static_cast<int>(value);
    } catch (...) {
        Instance::fail("invalid integer argument " + name);
    }
}

inline void requireCommonArguments(int argc) {
    if (argc < 8) {
        Instance::fail(
            "usage: GENERATOR MODE N M WEIGHTS R_STYLE ORDER [seed-tag]");
    }
}

inline void requireArguments(int argc) {
    requireCommonArguments(argc);
}

inline std::pair<int, int> undirectedKey(int u, int v) {
    if (u > v) std::swap(u, v);
    return {u, v};
}
