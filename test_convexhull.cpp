#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
struct point { T x, y; };

/*
 * Tested via https://open.kattis.com/problems/convexhull
 */
int main() {
    using ll = long long;
    using pt = point<ll>;
    while (true) {
        int n; cin >> n;
        if (n == 0) break;
        auto right = [](pt &a, pt &b, pt &c) {
            return  ((b.x - a.x) * (c.y - b.y) - 
                     (c.x - b.x) * (b.y - a.y)) < 0;
        };
        vector<pt> points(n);
        for (int i = 0; i < n; i++) {
            ll x, y; cin >> x >> y;
            points[i] = pt{x, y};
        }
        pt bt = *min_element(points.begin(), points.end(),
            [](pt &a, pt &b) { 
                if (a.y != b.y) return a.y < b.y;
                return a.x < b.x;
            }
        );
        sort(points.begin(), points.end(), [&](pt &a, pt &b) {
            ll det = ((a.x - bt.x) * (b.y - bt.y) -
                      (b.x - bt.x) * (a.y - bt.y));
            if (det != 0) return det < 0;
            return (a.x - bt.x) * (a.x - bt.x) + (a.y - bt.y) * (a.y - bt.y)
                 < (b.x - bt.x) * (b.x - bt.x) + (b.y - bt.y) * (b.y - bt.y);
        });
        vector<pt> pts;
        for (int i = 0; i < n; i++) {
            if (i > 0 && points[i].x == points[i-1].x &&
                points[i].y == points[i-1].y) continue;
            pts.push_back(points[i]); 
        }
        vector<pt> s;
        s.push_back(pts[0]);
        if (pts.size() > 1) s.push_back(pts[1]);
        for (int i = 2; i < pts.size(); i++) {
            while (!right(s[s.size()-2], s.back(), pts[i]) && s.size() >= 2) {
                s.pop_back();
            }
            s.push_back(pts[i]);
        }
        reverse(s.begin(), s.end());
        cout<<s.size()<<endl;
        for (auto p: s) cout<<p.x<<" "<<p.y<<endl;
        cout<<endl;
    }
}