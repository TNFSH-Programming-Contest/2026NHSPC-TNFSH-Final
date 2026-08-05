#include "testlib.h"
using namespace std;

int main(int argc, char* argv[]) {
	registerValidation(argc, argv);
	// about testlib, see https://codeforces.com/blog/entry/18426

	const int MAXN = std::stoi(argv[1]);
	const int MAXH = std::stoi(argv[2]);
	const int MINK = 30;
	const int MAXU = (int)1e9;
	const int MAXW = (int)1e9;
	const int MAXV = (int)1e9;

	int n = inf.readInt(1, MAXN, "n");
	inf.readSpace();
	int h = inf.readInt(1, MAXH, "h");
	inf.readSpace();
	inf.readInt(1, std::min(n, MINK), "k");
	inf.readEoln();

	std::vector<int> U(h);
	for (int i = 0; i < h; i++) {
		U[i] = inf.readInt(1, MAXU, "Uh");
		if (i != h - 1) inf.readSpace();
	}
    inf.readEoln();

	std::vector<int> W(h);
	for (int i = 0; i < h; i++) {
		W[i] = inf.readInt(1, MAXW, "Wh");
		if (i != h - 1) inf.readSpace();
	}
    inf.readEoln();

	std::vector<int> hunks(h, 0);
	for (int i = 0; i < n; i++) {
		int x = inf.readInt(1, h, "xi");
		inf.readSpace();
		int v = inf.readInt(1, MAXV, "vi");

		ensuref(hunks[x - 1] == 0, 
				"commit %d change hunk %d more than once: old value = %d, new value = %d", i + 1, x, hunks[x - 1], v);
		hunks[x - 1] = v;

		inf.readEoln();
	}

	inf.readEof();

	return 0;
}
