#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "llrb.h"

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

        llrb tree;
        for (int i = 0; i < sz; i++)
            tree.insert(permutation[i]);

        for (int i = 0; i < sz; i++)
            CHECK(tree.rank(i) == i+1);
    }
}

TEST_CASE("Black Height") {
    mt19937 rng(69420);
    for (int sz = 10; sz <= 100; sz++) {
        vector<int> permutation(sz);

        for (int i = 0; i < sz; i++)
            permutation[i] = i;

        shuffle(permutation.begin(), permutation.end(), rng);

        llrb tree;
        for (int i = 0; i < sz; i++)
            tree.insert(permutation[i]);

        vector<int> heights;
        using node_ptr = llrb::node_ptr;
        function<void(node_ptr,int)> dfs;
        dfs = [&](node_ptr root, int height) {
            if (root->left) dfs(root->left, height + (root->left->color == BLACK));
            if (root->right) dfs(root->right, height + (root->right->color == BLACK));
            if (!root->left && !root->right)
                heights.push_back(height);
        };
        dfs(tree.root, 0);

        for (int i = 1; i < heights.size(); i++) {
            CHECK(heights[i-1] == heights[i]);
        }
    }
}