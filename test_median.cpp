#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <algorithm>
#include <assert.h>
#include "doctest.h"
#include <iostream>
#include <random>
#include <vector>
using namespace std;

TEST_CASE("Ordered") {
    mt19937 rng(69420);
    for (int sz = 2; sz <= 1000; sz++) {
        vector<int> nums(sz);
        for (int i = 0; i < sz; i++) {
            nums[i] = uniform_int_distribution<int>(0, sz-1)(rng);
        }

        function<int(int,int,int)> median;
        median = [&](int l, int r, int idx) {
            if (l == r) return nums[l];
            int mi = uniform_int_distribution<int>(l, r)(rng);
            int m  = nums[mi];
            swap(nums[mi], nums[r]);
            int i = l - 1;
            for (int j = l; j < r; j++) {
                if (nums[j] <= m) {
                    i++;
                    swap(nums[i], nums[j]);
                }
            }
            i++;
            swap(nums[r], nums[i]);
            if      (idx > i)   return median(i+1, r, idx);
            else if (idx < i)   return median(l, i-1, idx);
            else                return m;
        };

        int ret = median(0, sz-1, sz/2);
        sort(nums.begin(), nums.end());
        int answer = nums[sz/2];
        CHECK(ret == answer);
    }
}