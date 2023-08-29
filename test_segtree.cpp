#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "segtree.h"

#include <algorithm>
#include "doctest.h"
#include <iostream>
#include <random>
#include <vector>

using namespace std;

TEST_CASE("Sums") {
    mt19937 rng(69420);
    for (int sz = 10; sz <= 100; sz++) {
        vector<int> permutation(sz);
        for (int i = 0; i < sz; i++)
            permutation[i] = i;

        shuffle(permutation.begin(), permutation.end(), rng);

        vector<int> prefix(sz + 1);
        for (int i = 1; i <= sz; i++)
            prefix[i] = prefix[i-1] + permutation[i-1];

        segtree tree(permutation);
        for (int i = 0; i < sz; i++) {
            int l = uniform_int_distribution<int>(0, sz-1)(rng);
            int r = uniform_int_distribution<int>(0, sz-1)(rng);
            if (l > r) swap(l, r);
            CHECK(tree.query(l, r) == prefix[r+1]-prefix[l]);
        }
    }
}