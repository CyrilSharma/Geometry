#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "rangetree.h"

#include <algorithm>
#include "doctest.h"
#include <iostream>
#include <random>
#include <vector>

using namespace std;

TEST_CASE("Brute-Force Comparison") {
    mt19937 rng(69420);
    for (int sz = 1; sz <= 100; sz++) {
        using T = int;
        using point = point<T>;
        set<point> used;
        vector<point> points;
        for (int i = 0; i < sz; i++) {
            auto l = uniform_int_distribution<T>(0, sz-1)(rng);
            auto r = uniform_int_distribution<T>(0, sz-1)(rng);
            if (!used.count({l, r})) {
                points.push_back({l, r});
                used.insert({l, r});
            }
        }

        /* presumes T is integral. */
        auto hash = [&](vector<point> subset) {
            long long hash = 0;
            long long B = 27342;
            long long A = 348723;
            long long MOD = 1e9;
            for (point p: subset) {
                hash = (hash + A * p.x) % MOD;
                hash = (hash + A * p.y) % MOD;
                hash = (hash + B) % MOD;
            }
            return hash;
        };

        auto select = [&](point p1, point p2) {
            vector<point> answer;
            for (int i = 0; i < points.size(); i++) {
                auto [x, y] = points[i];
                if (x < p1.x || x > p2.x) continue;
                if (y < p1.y || y > p2.y) continue;
                answer.push_back(points[i]);
            }
            return answer;
        };

        rangetree<T> tree(points);
        //tree.display();

        for (int i = 0; i < sz; i++) {
            auto x1 = uniform_int_distribution<T>(0, sz-1)(rng);
            auto x2 = uniform_int_distribution<T>(0, sz-1)(rng);
            auto y1 = uniform_int_distribution<T>(0, sz-1)(rng);
            auto y2 = uniform_int_distribution<T>(0, sz-1)(rng);
            if (x1 > x2) swap(x1, x2);
            if (y1 > y2) swap(y1, y2);
            point p1 = {x1, y1};
            point p2 = {x2, y2};
            /* cerr<<"____"<<endl;

            printf("QUERY: (%d %d) -> (%d %d)\n", p1.x, p1.y, p2.x, p2.y);

            cerr<<"RET: ";
            for (point p: ret) cerr<<p.x<<" "<<p.y<<" | ";
            cerr<<endl;

            
            cerr<<"ANSWER: ";
            for (point p: answer) cerr<<p.x<<" "<<p.y<<" | ";
            cerr<<endl;
            */
            vector<point> ret = tree.query(p1, p2);
            vector<point> answer = select(p1, p2);
            if (hash(ret) != hash(answer))
                CHECK(false);
        }
    }
}