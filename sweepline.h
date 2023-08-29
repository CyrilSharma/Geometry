#include <assert.h>
#include <cmath>
#include <functional>
#include <limits>
#include <set>
#include <vector>
using namespace std;

constexpr double EPS  = 1e-9;
constexpr double EPS2 = 1e-10;
struct pt { 
    double x, y; 
    pt operator-(const pt &other) {
        return pt{x-other.x, y-other.y};
    }
    bool operator>(const pt &other) {
        if (abs(x - other.x) > EPS) return x > other.x;
        return y > other.y + EPS;
    }
};

/* no point segments. */
struct seg {
    pt p, q;
    double a, b, c;
    seg(pt _p, pt _q): p(_p), q(_q) {
        if (p > q) swap(p, q);
        if (q.x - p.x < EPS2)
            q.x += EPS2;
        a = q.y - p.y;
        b = p.x - q.x;
        c = a * p.x + b * p.y;
    }

    void norm() {
        double mg = (a*a + b*b);
        a /= mg, b /= mg, c /= mg;
    }

    double get_y(double x) { 
        return (-a*x + c) / b;
    }

    static double det(double a, double b, double c, double d) { 
        return a * d - b * c;
    }

    bool betw(double l, double r, double x) {
        return min(l, r) <= x + EPS && x <= max(l, r) + EPS;
    }
    
    bool overlaps(seg &o) {
        if (abs(a-o.a) < EPS && abs(b-o.b) < EPS && abs(c-o.c) < EPS) {
            return betw(o.p.x, o.q.x, p.x) || betw(o.p.x, o.q.x, q.x) ||
                   betw(p.x, q.x, o.p.x)   || betw(p.x, q.x, o.q.x);
        }
        return false;
    }

    /* not counting overlapping segments. */
    bool intersects(seg &o) {
        double zn = det(a, b, o.a, o.b);
        if (abs(zn) < EPS) {
            return abs(q.x-o.p.x) < EPS &&
                abs(q.y-o.q.y) < EPS;
        }
        pt inter = intersection(o);
        return betw(p.x, q.x, inter.x)     && betw(p.y, q.y, inter.y) &&
               betw(o.p.x, o.q.x, inter.x) && betw(o.p.y, o.q.y, inter.y);
    }

    pt intersection(seg &o) {
        double zn = det(a, b, o.a, o.b);
        double x = det(c, b, o.c, o.b) / zn;
        double y = det(a, c, o.a, o.c) / zn;
        return pt{x, y};
    }
};

struct sweepline {
    /* status stuff */
    struct scomp_t {
        sweepline *owner;
        scomp_t(sweepline *owner) : owner(owner) {}
        bool operator()(seg *a, seg *b) const {
            double ay = a->get_y(owner->pos - EPS), by = b->get_y(owner->pos - EPS);
            if (ay != by) return ay < by;
            double zn = seg::det(a->a, b->a, a->b, b->b);
            return zn > EPS;
        }
    };

    double pos;
    scomp_t scomp;
    set<seg*, scomp_t> status;
    /*********/

    /* queue stuff */
    enum etype { UP=0, CROSS=1, LOW=2 };
    struct event {
        pt p;
        mutable vector<set<seg*>> id;
        event(pt _p) : p(_p), id(3, set<seg*>()) {}
    };

    struct ecomp_t {
        sweepline *owner;
        ecomp_t(sweepline *owner) : owner(owner) {}
        bool operator()(const event &a, const event &b) const {
            if (a.p.x != b.p.x) return a.p.x < b.p.x;
            return EPS + a.p.y < b.p.y;
        }
    };
    set<event, ecomp_t> queue(ecomp);

    sweepline(): scomp(scomp_t(this)), status(scomp_t(this)),
                 queue(ecomp_t(this)) {}

    void enqueue(pt p, etype tp, seg *s) {
        assert(s);
        auto it = queue.find(event(p));
        if (it == queue.end()) {
            event e = event(p);
            e.id[tp].insert(s);
            queue.insert(e);
        } else {
            const event &e = (*it);
            for (auto t: vector<etype>{UP, CROSS, LOW})
                if (e.id[t].count(s)) return;
            e.id[tp].insert(s);
        }
    }

    void handle_intersection(seg *a, seg *b) {
        assert(a && b);
        /* printf("a: (%4.1f %4.1f %4.1f) - (%4.1f, %4.1f)\n", a->a, a->b, a->c, a->p.x, a->q.x);
        printf("b: (%4.1f %4.1f %4.1f) - (%4.1f, %4.1f)\n", b->a, b->b, b->c, b->p.x, b->q.x); */
        if (!a->intersects(*b)) return;
        pt time = a->intersection(*b);
        /* printf("TIME: (%f %f)\n", time.x, time.y); */
        if (pos >= time.x) return;
        enqueue(time, CROSS, a);
        enqueue(time, CROSS, b);
    }
    /*********/

    vector<pt> solve(vector<seg> &segs) {
        for (seg &s: segs) {
            enqueue(s.p, UP, &s);
            enqueue(s.q, LOW, &s);
        }

        bool first = false;
        vector<pt> answer;
        while (!queue.empty()) {
            event e = *queue.begin(); queue.erase(queue.begin());
            if (e.id[UP].size() + e.id[CROSS].size() + e.id[LOW].size() > 1)
                answer.push_back(e.p);

            pos = e.p.x;
            /* printf("pos: %4.1f, %4.1f\n", e.p.x, e.p.y); */
            auto display = [&](seg *s) {
                if (status.count(s)) status.erase(s);
                else {
                    printf("s: %4.1f, %4.1f, %4.1f\n", s->a, s->b, s->c);
                    assert(false);
                }
            };
            for (auto s: e.id[LOW])    display(s);
            for (auto s: e.id[CROSS])  display(s);

            pos += 2*EPS;
            for (auto s: e.id[CROSS])  status.insert(s);
            for (auto s: e.id[UP])     status.insert(s);

            if (e.id[CROSS].size() + e.id[UP].size() > 0) {
                /* printf("__\n"); */
                seg *l = 0, *r = 0;
                for (auto tp: vector<etype>{CROSS, UP}) {
                    for (seg *c: e.id[tp]) {
                        if (!l || scomp(c, l)) l = c;
                        if (!r || scomp(r, c)) r = c;
                    }
                }

                /* printf("Event Items: %2lu\n", (e.id[CROSS].size() + e.id[UP].size()));
                printf("l: (%4.1f, %4.1f, %4.1f)\n", l->a, l->b, l->c);
                printf("r: (%4.1f, %4.1f, %4.1f)\n", r->a, r->b, r->c);

                printf("Pos: (%f %f)\n", e.p.x, e.p.y);  */
                /* for (seg *s : status) printf("(%f, %f) -> (%f, %f), ", s->p.x, s->p.y, s->q.x, s->q.y);
                printf("\n"); */
                //for (seg *s : status) printf("Res r: %d\n", scomp(s, r));
                //for (seg *s : status) printf("Res l: %d\n", scomp(l, s));
                /* for (seg *s : status) {
                    printf("AMT: %5.2f - (%4.1f, %4.1f, %4.1f)\n", s->get_y(pos),
                           s->a, s->b, s->c);
                }
                printf("\n"); */
                auto itl = status.find(l);
                if (itl != status.begin())      handle_intersection(*prev(itl), l);
            
                auto itr = status.find(r);
                assert(itr != status.end());
                if (next(itr) != status.end())  handle_intersection(r, *next(itr));
            } else {
                auto it = status.lower_bound(*e.id[LOW].begin());
                if (it != status.begin() && it != status.end())
                    handle_intersection(*prev(it), *it);
            }
        }
        return answer;
    }
};