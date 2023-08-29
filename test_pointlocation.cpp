#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <tuple>
#include <vector>
using namespace std;

/* TESTER is https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=297&page=show_problem&problem=3732 */
/** GEOMETRY **/
double EPS = 1e-9;
struct pt {
    double x, y;
    bool operator<(const pt &other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

struct seg {
    int region;
    pt p, q;
    double a, b, c;
    seg(pt _p, pt _q) : p(_p), q(_q) {
        p.x = p.x + p.y * EPS;
        q.x = q.x + q.y * EPS;
        if (q < p) swap(p, q);
        a = q.y - p.y;
        b = p.x - q.x;
        c = a*p.x + b*p.y;
        region = 0;
    }
    double y(double x) { return (c - a*x) / b; }
};
/*************/

/** DATA STRUCTURES **/
struct dsu {
    vector<int> parent, size;
    dsu(int n) {
        parent.assign(n, -1);
        size.assign(n, 1);
    }
    int find(int a) {
        if (parent[a] == -1) return a;
        return parent[a] = find(parent[a]);
    }
    void merge(int a, int b) {
        int u = find(a), v = find(b);
        if (u == v) return;
        if (size[u] > size[v])
            swap(u, v);
        parent[u] = v;
        size[v] += size[u];
    }
};

struct treap {
    struct node {
        seg *value;
        int prior;
        node *l, *r;
        node(seg *v) : value(v), prior(rand()) {
            l = r = 0;
        }
        node(node *nd) {
            value = nd->value;
            prior = nd->prior;
            l = nd->l, r = nd->r; 
        }
    };

    double *pos;
    bool less(seg *s1, seg *s2) {
        double ay = s1->y(*pos), by = s2->y(*pos);
        if (ay != by) return ay < by;
        return (s1->a * s2->b - s1->b * s2->a) > EPS;
    }

    node *root;
    vector<node*> roots;
    vector<double> xs;
    treap(double *_pos): root(nullptr), pos(_pos)  {}

    pair<node*, node*> split(node *ptr, seg *value) {
        if (ptr == 0) return {0, 0};
        node *L = 0, *R = 0;
        if (less(ptr->value, value)) {
            L = new node(ptr);
            tie(L->r, R) = split(ptr->r, value);
        } else {
            R = new node(ptr);
            tie(L, R->l) = split(ptr->l, value);
        }
        return {L, R};
    }

    node* merge(node *small, node *large) {
        if (!small) return large;
        if (!large) return small;
        if (small->prior > large->prior) {
            node *copy = new node(small);
            copy->r = merge(small->r, large);
            return copy;
        } else {
            node *copy = new node(large);
            copy->l = merge(small, large->l);
            return copy;
        }
    }

    node* insert(seg *value) {
        auto [L, R] = split(root, value);
        L = merge(L, new node(value));
        return root = merge(L, R);
    }

    node* erase(seg *value) { 
        return root = erase(root, value); 
    }
    
    node* erase(node *ptr, seg *value) {
        if (ptr == nullptr) return nullptr;
        if (ptr->value == value) return merge(ptr->l, ptr->r);
    
        node *copy = new node(ptr);
        if      (less(value, copy->value)) copy->l = erase(copy->l, value);
        else if (less(copy->value, value)) copy->r = erase(copy->r, value);
        return copy;
    }

    seg* find(pt p) {
        auto it = lower_bound(xs.begin(), xs.end(), p.x);
        if (it == xs.begin()) return nullptr;
        node *rx = roots[(--it) - xs.begin()];
        seg *s = find_seg(rx, p);
        return s;
    }

    seg* find(double y) { return find_seg(root, pt{*pos, y}); }

    seg* find_seg(node *nd, pt p) {
        if (nd == nullptr) return nullptr;
        seg* found = nullptr;
        if (p.y <= nd->value->y(p.x)) {
            found = find_seg(nd->l, p);
        } else {
            found = find_seg(nd->r, p);
            if (found == nullptr) return nd->value;
        }
        return found;
    }

    void save(double x) {
        roots.push_back(root); 
        xs.push_back(x);
    }    
};
/*********************/

struct sweepline {
    /**QUEUE STUFF**/
    enum etype {ADD=0, DELETE=1, VERTICAL=2};
    struct event {
        pt p;
        mutable vector<seg*> id[3];
        event(pt _p): p(_p) {}
        bool operator<(const event &other) const {
            if (abs(p.x-other.p.x) > EPS) return p.x < other.p.x;
            if (abs(p.y-other.p.y) > EPS) return p.y > other.p.y;
            return false;
        }
    };
    
    set<event> q;
    void enqueue(pt p, seg *s, etype tp) {
        event e = event(p);
        if (q.count(e)) {
            auto it = q.find(e);
            it->id[tp].push_back(s);
        } else {
            e.id[tp].push_back(s);
            q.insert(e);
        }
    }
    /***************/

    void solve(vector<seg> &segs, vector<pt> &labels, vector<pt> &queries) {
        int n = segs.size();
        for (auto &s: segs) {
            enqueue(s.p, &s, ADD);
            enqueue(s.q, &s, DELETE);
        }

        double pos;
        treap status(&pos);
        dsu ds(n+1);
        int regcount = 1;
        double px = numeric_limits<double>::min();
        while (!q.empty()) {
            event e = *q.begin(); q.erase(q.begin());

            pos = e.p.x - EPS;            
            for (seg *s : e.id[DELETE]) status.erase(s);
            for (seg *s : e.id[ADD])    s->region = regcount++;

            seg *higha = 0, *highd = 0;
            if (!e.id[ADD].empty()) {
                higha = *max_element(e.id[ADD].begin(), e.id[ADD].end(),
                    [&](seg *u, seg *v) { return (u->a * v->b - u->b * v->a) > EPS; });
            }

            if (!e.id[DELETE].empty()) {
                highd = *min_element(e.id[DELETE].begin(), e.id[DELETE].end(), 
                    [&](seg *u, seg *v) { return (u->a * v->b - u->b * v->a) > EPS; });
            }


            if (e.id[DELETE].empty()) {
                seg *low = status.find(e.p.y);
                if (low == nullptr) ds.merge(0, higha->region);
                else                ds.merge(low->region, higha->region);
            } else if (e.id[ADD].empty()) {
                seg *low = status.find(e.p.y);
                if (low == nullptr) ds.merge(0, highd->region);
                else                ds.merge(low->region, highd->region);
            } else {
                ds.merge(higha->region, highd->region);
            }

            pos = e.p.x + EPS;
            for (seg *s : e.id[ADD]) status.insert(s);
            status.save(e.p.x);
        }

        vector<int> face(n+1);
        for (int i = 1; i <= labels.size(); i++) {
            seg *s = status.find(labels[i-1]);
            if (s == nullptr) face[ds.find(0)] = i;
            else              face[ds.find(s->region)] = i;
        }

        for (int i = 0; i < queries.size(); i++) {
            seg *s = status.find(queries[i]);
            int id = (s == nullptr) ? ds.find(0) : ds.find(s->region);
            cout<<face[id]<<endl;
        }
        /* for (auto &s: segs) {
            printf(
                "%f %f %f %f %d\n", 
                s.p.x, s.p.y, s.q.x, s.q.y, 
                ds.find(s.region)
            );
        } */
    }
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    while (true) {
        int n, m, p, q;
        cin >> n >> m >> p >> q;
        if (n == 0 && m == 0 && p == 0 && q == 0) break;
        vector<pt> points;
        for (int i = 0; i < n; i++) {
            double x, y; cin >> x >> y;
            points.push_back(pt{x, y});
        }

        vector<seg> segs;
        for (int i = 0; i < m; i++) {
            int p1, p2; cin >> p1 >> p2; p1--, p2--;
            segs.push_back(seg{points[p1], points[p2]});
        }

        vector<pt> labels;
        for (int i = 0; i < p; i++) {
            double x, y; cin >> x >> y;
            labels.push_back(pt{x, y});
        }

        vector<pt> queries;
        for (int i = 0; i < q; i++) {
            double x, y; cin >> x >> y;
            queries.push_back(pt{x, y});
        }

        sweepline s;
        s.solve(segs, labels, queries);
    }
}