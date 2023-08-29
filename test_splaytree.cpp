#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "splaytree.h"

#include <algorithm>
#include "doctest.h"
#include <iostream>
#include <random>
#include <vector>
#include <set>

using namespace std;

TEST_CASE("Count" * doctest::timeout(1.0)) {
    mt19937 rng(69420);
    for (int sz = 10; sz <= 1000; sz++) {
        vector<int> permutation(sz);
        for (int i = 0; i < sz; i++)
            permutation[i] = i;

        shuffle(permutation.begin(), permutation.end(), rng);

        splaytree tree;
        for (int i = 0; i < sz; i++)
            tree.insert(permutation[i]);
    
        for (int i = 0; i < sz; i++)
            CHECK(tree.count(permutation[i]));
    }
}

TEST_CASE("Cache Properties" * doctest::timeout(3.0)) {
    int sz = 1e5;
    mt19937 rng(69420);
    vector<int> permutation(sz);
    for (int i = 0; i < sz; i++)
        permutation[i] = i;

    shuffle(permutation.begin(), permutation.end(), rng);

    splaytree tree;
    for (int i = 0; i < sz; i++)
        tree.insert(permutation[i]);

    for (int i = 0; i < 1e8; i++)
        if (!tree.count(permutation[0]))
            CHECK(false);
}

/* 
TEST_CASE("STL Comparison" * doctest::timeout(3.0)) {
    int sz = 1e5;
    mt19937 rng(69420);
    vector<int> permutation(sz);
    for (int i = 0; i < sz; i++)
        permutation[i] = i;

    shuffle(permutation.begin(), permutation.end(), rng);

    set<int> s;
    for (int i = 0; i < sz; i++)
        s.insert(permutation[i]);

    for (int i = 0; i < 1e8; i++)
        if (!s.count(permutation[0]))
            CHECK(false);
} 
*/