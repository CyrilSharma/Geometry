#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "pathcopy.h"

#include <algorithm>
#include "doctest.h"
#include <iostream>
#include <random>
#include <set>
#include <vector>

using namespace std;

TEST_CASE("Insertions") {
    mt19937 rng(69420);
    for (int sz = 10; sz <= 100; sz++) {
        vector<int> array(sz);
        for (int i = 0; i < sz; i++) array[i] = i;
        shuffle(array.begin(), array.end(), rng);

        auto copy = [](set<int> &a, set<int> &b) {
            for (auto el : b) a.insert(el);
        };
        
        set<int> cur;
        treap<int> tree;
        vector<set<int>> sets(sz);
        for (int i = 0; i < sz; i++) {
            cur.insert(array[i]);
            tree.insert(array[i]);
            copy(sets[i], cur);
        }

        for (int i = 0; i < sz; i++) {
            int t = uniform_int_distribution<int>(0, sz-1)(rng);
            int x = uniform_int_distribution<int>(0, sz-1)(rng);
            if (tree.count(t, x) != sets[t].count(x)) {
                printf("T: %d | X: %d\n", t, x);
                tree.print(t);
                for (auto el : sets[t]) printf("%d ", el);
                printf("\n");
                assert(false);
            }
        }
    }
}

TEST_CASE("Insertions && Deletions ") {
    mt19937 rng(69420);
    for (int sz = 10; sz <= 100; sz++) {
        vector<int> array(2*sz);
        for (int i = 0; i < sz; i++) array[i] = i;
        for (int i = 0; i < sz; i++) array[sz+i] = i;
        shuffle(array.begin(), array.end(), rng);

        auto copy = [](set<int> &a, set<int> &b) {
            for (auto el : b) a.insert(el);
        };
        
        set<int> cur;
        treap<int> tree;
        vector<set<int>> sets(2*sz);
        for (int i = 0; i < 2*sz; i++) {
            if (!cur.count(array[i])) {
                cur.insert(array[i]);
                tree.insert(array[i]);
                copy(sets[i], cur);
            } else {
                cur.erase(array[i]);
                tree.erase(array[i]);
                copy(sets[i], cur);
            }
        }

        for (int i = 0; i < 2*sz; i++) {
            int t = uniform_int_distribution<int>(0, 2*sz-1)(rng);
            int x = uniform_int_distribution<int>(0, sz-1)(rng);
            if (tree.count(t, x) != sets[t].count(x)) {
                printf("T: %d | X: %d\n", t, x);
                tree.print(t);
                for (auto el : sets[t]) printf("%d ", el);
                printf("\n");
                assert(false);
            }
        }
    }
}