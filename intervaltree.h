#include <vector>
using namespace std;

template <typename T>
struct interval {
    T l, r;
    bool valid;
    interval(T a, T b): l(a), r(b) {}
    interval(T a, T b, bool v) {
        l = a, r = b, valid = v;
    }
};

template <typename T>
struct intervaltree {
    using interval = interval<T>;
    struct node {
        T split;
        node *l, *r;
        vector<interval> sleft, sright;
        node(T v): l(nullptr), r(nullptr) {
            split = v;
        }
    };

    node *root = nullptr;
    intervaltree(vector<interval> &intervals) {
        vector<interval> points;
        for (interval i: intervals) {
            points.push_back(interval(i.l, i.r, true));
            points.push_back(interval(i.r, i.l, false));
        }
        sort(points.begin(), points.end(), [](interval a, interval b) {
            return a.l < b.l;
        });
        root = build(points);
    }

    node* build(vector<interval> &points) {
        auto midI = points[points.size() / 2];
        T mid = midI.l;
        node *cur = new node(mid);
        vector<interval> lpoints, rpoints;
        for (interval i: points) {
            if (i.l <= mid && mid <= i.r) {
                if (!i.valid) continue;
                cur->sleft.push_back(i);
                cur->sright.push_back(i);
            } else if (i.l < mid && i.r < mid) {
                lpoints.push_back(i);
            } else if (i.l > mid && i.r > mid) {
                rpoints.push_back(i);
            }
        }
        sort(cur->sleft.begin(), cur->sleft.end(), [](interval a, interval b) {
            return a.l < b.l;
        });
        sort(cur->sright.begin(), cur->sright.end(), [](interval a, interval b) {
            return a.r > b.r;
        });
        if (lpoints.size()) cur->l = build(lpoints);
        if (rpoints.size()) cur->r = build(rpoints);
        return cur;
    }

    vector<interval> query(T pos) { return query(root, pos); }
    vector<interval> query(node *item, T pos) {
        vector<interval> answer;
        if (item == nullptr) return answer;
        if (pos <= item->split) {
            for (auto i: item->sleft) {
                if (i.l > pos) break;
                answer.push_back(i);
            }
            auto left = query(item->l, pos);
            answer.insert(answer.end(), left.begin(), left.end());
        } else {
            for (auto i: item->sright) {
                if (i.r < pos) break;
                answer.push_back(i);
            }
            auto right = query(item->r, pos);
            answer.insert(answer.end(), right.begin(), right.end());
        }
        return answer;
    }

    void clear() { clear(root); }
    void clear(node *item) {
        if (item->l) clear(item->l);
        if (item->r) clear(item->r);
        delete item;
    }
    ~intervaltree() { clear(); }
};