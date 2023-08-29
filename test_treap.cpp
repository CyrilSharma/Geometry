#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "treap.h"

#include <algorithm>
#include "doctest.h"
#include <iostream>
#include <random>
#include <vector>

using namespace std;

TEST_CASE("Ordered") {
    mt19937 rng(69420);
    for (int sz = 10; sz <= 100; sz++) {
        vector<int> permutation(sz);
        for (int i = 0; i < sz; i++)
            permutation[i] = i;

        shuffle(permutation.begin(), permutation.end(), rng);

        treap tree;
        for (int i = 0; i < sz; i++)
            tree.insert(permutation[i]);

        for (int i = 0; i < sz; i++)
            CHECK(tree.select(i+1) == i);
    }
}