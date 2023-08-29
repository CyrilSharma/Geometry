#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "dsu.h"

#include <algorithm>
#include "doctest.h"
#include <random>
#include <vector>

using namespace std;

TEST_CASE("Connectivity") {
    mt19937 rng(69420);
    for (int sz = 10; sz <= 100; sz++) {
        vector<int> permutation(sz);
        for (int i = 0; i < sz; i++)
            permutation[i] = i;

        shuffle(permutation.begin(), permutation.end(), rng);

        dsu ds(sz);
        for (int i = 0; i < sz; i++) {
            int l = uniform_int_distribution<int>(0, sz-1)(rng);
            int r = uniform_int_distribution<int>(0, sz-1)(rng);
            if (l > r) swap(l, r);
            ds.merge(l, r);
            CHECK(ds.find(l) == ds.find(r));
        }
    }
}