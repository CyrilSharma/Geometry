#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "heap.h"

#include <algorithm>
#include "doctest.h"
#include <iostream>
#include <random>
#include <vector>

using namespace std;

TEST_CASE("make_heap + deletion") {
    mt19937 rng(69420);
    for (int sz = 10; sz <= 100; sz++) {
        vector<int> permutation(sz);

        for (int i = 0; i < sz; i++)
            permutation[i] = i;

        shuffle(permutation.begin(), permutation.end(), rng);

        heap h(permutation);
        vector<int> sorted(sz);
        for (int i = sz-1; i >= 0; i--) {
            sorted[i] = h.top(); h.pop();
        }

        for (int i = 1; i < sz; i++)
            CHECK(sorted[i-1] <= sorted[i]);
    }
}

TEST_CASE("insertion + deletion") {
    mt19937 rng(69420);
    for (int sz = 10; sz <= 100; sz++) {
        
        vector<int> permutation(sz);
        for (int i = 0; i < sz; i++)
            permutation[i] = i;

        shuffle(permutation.begin(), permutation.end(), rng);

        heap h;
        for (int i = 0; i < sz; i++)
            h.insert(permutation[i]);
        
        vector<int> sorted(sz);
        for (int i = sz-1; i >= 0; i--) {
            sorted[i] = h.top(); h.pop();
        }

        for (int i = 1; i < sz; i++)
            CHECK(sorted[i-1] <= sorted[i]);
    }
}