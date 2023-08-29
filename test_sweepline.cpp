#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "sweepline.h"

#include <algorithm>
#include "doctest.h"
#include <iostream>
#include <random>
#include <vector>

using namespace std;

/* should be noted that this same algorithm fails for vertical lines
 * for it to work in that case, instead of using epsilon shifting,
 * use a custom erase and insert iterator to appropriately manage the tree.
 * it should work right out of the box.
 */
TEST_CASE("Test Reals") {
    mt19937 rng(69420);
    for (int sz = 10; sz <= 100; sz++) {
        vector<seg> segs;
        auto overlaps = [&](seg &o) {
            for (seg &s: segs) {
                if (s.overlaps(o))
                    return true;
            }
            return false;
        };

        for (int cnt = 0; cnt < sz;) {
            double px = (double) uniform_real_distribution<double>(0, sz-1)(rng);
            double qx = (double) uniform_real_distribution<double>(0, sz-1)(rng);
            double py = (double) uniform_real_distribution<double>(0, sz-1)(rng);
            double qy = (double) uniform_real_distribution<double>(0, sz-1)(rng);
            if (abs(px-qx) < EPS && abs(py-qy) < EPS) continue;
            seg s = seg{pt{px, py}, pt{qx, qy}};
            if (overlaps(s)) continue;
            segs.push_back(s);
            cnt++;
        }

        auto find = [&]() {
            auto comp = [&](pt a, pt b) {
                if (abs(a.x-b.x) > EPS) return a.x < b.x;
                return EPS + a.y < b.y; 
            };
            set<pt, decltype(comp)> pts(comp);
            for (int i = 0; i < segs.size(); i++) {
                for (int j = 0; j < i; j++) {
                    if (segs[i].intersects(segs[j])) {
                        auto insc = segs[i].intersection(segs[j]);
                        if (pts.find(insc) == pts.end()) {
                            pts.insert(insc);
                        }
                    }
                }
            }
            return pts;
        };

        auto verify = [&](vector<pt> ret) {
            auto answer = find();
            /* printf("R: %lu, A: %lu\n", ret.size(), answer.size()); */
            CHECK(ret.size() == answer.size());
            for (int i = 0; i < ret.size(); i++) {
                CHECK(answer.count(ret[i]));
            }
            return true;
        };

        sweepline s;
        vector<pt> ret = s.solve(segs);
        verify(ret);
    }
}