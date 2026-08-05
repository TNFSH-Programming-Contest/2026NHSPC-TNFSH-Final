#include <bits/extc++.h>
#include <bits/stdc++.h>
using namespace std;

// #pragma GCC optimize("Ofast,unroll-loops,no-stack-protector,fast-math")
// #pragma GCC target("sse4,avx,avx2,abm,bmi,bmi2,adx,lzcnt,popcnt,tune=native")
// #pragma GCC optimize("Ofast,unroll-loops")
// #pragma GCC target("avx2,popcnt,lzcnt,abm,bmi,bmi2,tune=native") // avx or sse
// #pragma pack(1) // 省記憶體用的

#ifdef TOBIICHI3227
#include <algo/debug.h>
#define debug(...) \
    cerr << "[" << __FUNCTION__ << "]L: " << __LINE__,\
    cerr << " (" << #__VA_ARGS__ << ") =", debug_out(__VA_ARGS__)
#else
#define debug(...) 3227
#endif

using loli = int64_t;
using pii = std::pair<int, int>;
using pll = std::pair<loli, loli>;

template <typename T>
using order_multiset =
    __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less_equal<T>,
                     __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;

template <typename T>
using order_set =
    __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less<T>,
                     __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;
#define pb push_back
#define eb emplace_back
#define ss second
#define ff first
#define dd cout << '\n';
#define all(container) (container).begin(), (container).end()
#define each(x, arr) for (auto &(x) : (arr))
#define c_each(x, arr) for (const auto &(x) : (arr))
#define F_OR(i, a, b, s) for (int (i) = (a); (s) > 0 ? (i) < (b) : (i) > (b); (i) += (s))
#define F_OR1(e) F_OR(i, 0, e, 1)
#define F_OR2(i, e) F_OR(i, 0, e, 1)
#define F_OR3(i, b, e) F_OR(i, b, e, 1)
#define F_OR4(i, b, e, s) F_OR(i, b, e, s)
#define GET5(a, b, c, d, e, ...) e
#define F_ORC(...) GET5(__VA_ARGS__, F_OR4, F_OR3, F_OR2, F_OR1)
#define rep(...)       \
    F_ORC(__VA_ARGS__) \
    (__VA_ARGS__)
#define INF 0x3f
#define endl '\n'

template <typename T, typename U>
std::istream &operator>>(std::istream &is, std::pair<T, U> &val) {
    is >> val.first >> val.second;
    return is;
}
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &arr) {
    for (T &it : arr) {
        is >> it;
    }
    return is;
}

#if __cplusplus >= 201703L
template <typename... T> inline void ccin(T &...args) { ((std::cin >> args), ...); }
template <typename... T> inline void ccout(T &&...args) {
    ((std::cout << args), ...);
}
template <typename... T> inline void ccoutl(T &&...args) {
    ((std::cout << args), ...);
    std::cout << '\n';
}
#endif

#if __cplusplus <= 201402L
inline void ccin() {}
inline void ccout() {}
inline void ccoutl() { std::cout << "\n"; }

template <typename T, typename... Args> void ccin(T &first, Args &...args) {
    std::cin >> first;
    ccin(args...);
}

template <typename T, typename... Args> void ccout(T &&first, Args &&...args) {
    std::cout << first;
    ccout(args...);
}

template <typename T, typename... Args> void ccoutl(T &&first, Args &&...args) {
    std::cout << first;
    ccoutl(args...);
}
#endif

// from blameazu
struct DSU{
	int com;
	vector<int> p;
	DSU(int n) : com(n), p(n+1, -1) {}
	int fp(int id) {return p[id] < 0 ? id : p[id] = fp(p[id]);}
	bool same(int a, int b) {return fp(a) == fp(b);}
	void upd(int a, int b) {
		int ar = fp(a), br = fp(b);
		if(ar != br) {
			if(p[ar] > p[br]) swap(ar, br);
			p[ar] += p[br];
			p[br] = ar;
			com--;
		}
	}
};

struct Edge {
   	int from, to;
    loli cost;
};

#define nitrogen std::ios::sync_with_stdio(false), std::cin.tie(nullptr)

int main() {
    nitrogen;

	int n, m;
	loli r;
 	std::cin >> n >> m >> r;
  	std::vector<Edge> edges(m);
   	loli total_cost = 0;
    for (auto &e : edges) {
        std::cin >> e.from >> e.to >> e.cost;
        total_cost += e.cost;
    }

    std::sort(edges.begin(), edges.end(), [](const auto& left, const auto& right) -> bool {
        return left.cost > right.cost;
    });

    DSU dsu(n);
    loli mst_cost = 0;

    for (const auto& e : edges) {
        if (!dsu.same(e.from, e.to)) {
           	dsu.upd(e.from, e.to);
            mst_cost += e.cost;
        }
    }

    std::cout << total_cost - mst_cost << '\n';

    return 0;
}
