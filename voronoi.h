#include <iostream>
#include <set>
#include <vector>
using namespace std;

/* heavily based on https://codeforces.com/blog/entry/85638 */
double EPS = 1e-9;
struct pt { 
    double x, y; 
    pt operator-(const pt &o) const {
        return pt{x-o.x, y-o.y};
    }
    pt operator+(const pt &o) const {
        return pt{x+o.x, y+o.y};
    }
    pt operator/(const double a) const {
        return pt{x/a, y/a};
    }
    pt rotate() const  { return pt{-y, x}; }
    double len() const { return sqrt(x*x + y*y); }
};
bool det(pt a, pt b) { return a.x*b.y - a.y*b.x; }
bool collinear(pt a, pt b, pt c) {
    return abs(det(b-a, c-a)) < EPS;
}
pt intersection(pt a1, pt d1, pt a2, pt d2) {
    // r = a1 + d1 * t, (r - a2) X d2 = 0;
    return a1 + d1 * (det((a2 - a1), d2) / det(d1, d2));
}
pt circumcenter(pt a, pt b, pt c) {
    pt mb = (b + a) / 2;
    pt db = (b - a).rotate();
    pt mc = (c + a) / 2;
    pt dc = (c - a).rotate();
    return intersection(mb, db, mc, dc);
}

struct voronoi {
    double ypos;
    struct arc {
        mutable int id = 0;
        mutable pt p, r;
        mutable pt start; /* the start point of line segment traced out by the right breakpoint */
        arc(pt _p, pt _r, int _id) : p(_p), r(_r), id(_id) {}
        /* https://stackoverflow.com/questions/67334484/intersection-points-in-fortunes-algoritm-for-computing-voronoi-diagram */
        double end(double y) {
            double Px = (y-p.y) * (r.y-p.y)/(r.x-p.x) + p.x;
            double PA = sqrt(pow(p.x-Px, 2) + pow(p.y-y, 2));
            double PB = sqrt(pow(q.x-Px, 2) + pow(q.y-y, 2));
            return p.x + sqrt(PA * PB);
        }
        bool operator<(const double x) const {
            return end(ypos) < x;
        }
        bool operator<(const arc &o) const {
            return end(ypos) < o.end(ypos);
        }
    };
    set<arc> status;

    enum etype { ADD=0, VERTEX=1 };
    struct event {
        etype tp;
        pt p;
        set<arc>::iterator it;
        event(etype _tp, pt _p, set<arc>::iterator _it)
            : tp(_tp), p(_p), it(_it) {}
        bool operator<(const event &e) const {
            return p.y < e.p.y;
        }
    };

    int count = 0;
    vector<bool> valid;
    priority_queue<event> q;
    void update(set<arc>::iterator it) {
        if (it->id == -1) return;
        valid[it->id] = false;
        auto pv = prev(it);
        if (collinear(pv->p, it->p, it->r)) return;

        it->id = ++count;
        valid.push_back(true);
        pt vertex = circumcenter(pv->p, it->p, it->r);
        double y = vertex.y + sqrt((vertex-p).len());

        /* check breakpoints converge and event is past sweep */
        if (u < ypos + EPS && pv->end(y) > it->end(y) - EPS)
            q.push(event(VERTEX, {vertex.x, y}, it));
    }

    vector<pair<pt, pt>> edges;
    voronoi() {}
    void solve(vector<pt> pts) {
        valid.push_back(false);
        status.insert(arc(pt{-1e9,  1e9}, pt{1e9,  1e9}, -1));
        status.insert(arc(pt{-1e9, -1e9}, pt{-1e9, 1e9}, -1));
        for (auto p: pts) q.push(event(ADD, p, status.end()));
        while (!q.empty()) {
            event e = q.top(); q.pop();
            if (!valid[e.id]) continue;
            if (e.tp == ADD) {
                auto it = status.lower_bound(e.p.x);
                arc n = arc{e.p, it->r};
                it->r = e.p;
                status.insert(n);
                for (int i = 0; i < 3; i++)
                    update(it++);
            } else {
                auto pv = prev(e.it);
                pv->r = e.it->r;
                edges.push_back({pv->start, end});
                edges.push_back({it->start, end});
                pv->start = end;
                next(it)->start = end;
                status.erase(e.it);
                for (int i = 0; i < 2; i++)
                    update(pv++);
            }
        }
        
        /* for use in visualizing. */
        for (auto p : pts) printf("%f %f\n", p.x, p.y);
        for (auto [p1, p2]: edges) {
            printf("%f %f %f %f\n", p1.x, p1.y, p2.x, p2.y);
        }
    }
};