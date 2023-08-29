#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "intervaltree.h"

#include <algorithm>
#include "doctest.h"
#include <iostream>
#include <random>
#include <vector>

using namespace std;

TEST_CASE("Brute-Force Comparison") {
    mt19937 rng(69420);
    for (int sz = 1; sz <= 1000; sz++) {
        using interval = interval<int>;
        vector<interval> intervals;
        for (int i = 0; i < sz; i++) {
            auto l = uniform_int_distribution<int>(0, sz-1)(rng);
            auto r = uniform_int_distribution<int>(0, sz-1)(rng);
            intervals.push_back(interval(l, r));
        }

        /* presumes T is integral. */
        auto hash = [&](vector<interval> subset) {
            long long hash = 0;
            long long B = 27342;
            long long A = 348723;
            long long MOD = 1e9;
            for (interval i: subset) {
                hash = (hash + A * i.l) % MOD;
                hash = (hash + A * i.r) % MOD;
                hash = (hash + B) % MOD;
            }
            return hash;
        };

        auto select = [&](int x) {
            vector<interval> answer;
            for (auto i: intervals) {
                if (x < i.l || x > i.r) continue;
                answer.push_back(i);
            }
            return answer;
        };

        intervaltree<int> tree(intervals);
        for (int i = 0; i < sz; i++) {
            auto x1 = uniform_int_distribution<int>(0, sz-1)(rng);
            vector<interval> ret = tree.query(x1);
            vector<interval> answer = select(x1);
            if (hash(ret) != hash(answer))
                CHECK(false);
        }
    }
}