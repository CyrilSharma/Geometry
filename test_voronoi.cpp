#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "voronoi.h"

#include <algorithm>
#include "doctest.h"
#include <iostream>
#include <random>
#include <vector>
using namespace std;

/* verified visually. */
TEST_CASE("Test Lattice Points") {
    mt19937 rng(69420);
    int sz = 100;
    set<pt> s;
    vector<pt> pts;
    for (int cnt = 0; cnt < sz;) {
        double px = (double) uniform_int_distribution<int>(0, sz-1)(rng);
        double py = (double) uniform_int_distribution<int>(0, sz-1)(rng);
        if (s.count(pt{px, py})) continue;
        pts.push_back(pt{px, py});
        cnt++;
    }
    voronoi v;
    v.solve(pts);
}