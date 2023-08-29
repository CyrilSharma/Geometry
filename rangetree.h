#include <iostream>
#include <functional>
#include <vector>
using namespace std;

template <typename T>
struct point {
    T x, y;
    bool operator<(const point &other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    bool operator>(const point &other) const {
        return other < *this;
    }
    bool operator==(const point &other) const {
        return (x == other.x && y == other.y);
    }
    bool operator<=(const point &other) const {
        if (x != other.x) return x < other.x;
        return y <= other.y;
    }
    bool operator>=(const point &other) const {
        if (x != other.x) return x > other.x;
        return y >= other.y;
    }
};

template <typename T>
struct rangetree {
    static constexpr int INVALID = -1e9;
    using point = point<T>;
    struct anode {
        point p;
        int l, r;
        anode() { l = r = INVALID; }
        anode(point p): p(p) {
            l = r = INVALID;
        }
    };
    struct tnode {
        point p;
        tnode *l, *r;
        vector<anode> assc;
        tnode(point p) : p(p) {
            l = r = nullptr;
            assc.push_back(p);
        }
    };

    int n;
    tnode *root = nullptr;
    rangetree(vector<point> &array) {
        n = array.size();
        build(array);
    }

    void build(vector<point> &array) {
        sort(array.begin(), array.end());
        auto merge = [&](const vector<anode> &a, const vector<anode> &b) {
            int sz = a.size() + b.size();
            vector<anode> list(sz);
            bool update_i = false;
            int i = 0, j = 0, prev = 0, prev_update = -1;
            while (i + j < sz) {
                if (j >= b.size())                       update_i = true;
                else if (i >= a.size())                  update_i = false;
                else if (a[i].p.y <= b[j].p.y)           update_i = true;
                else                                     update_i = false;

                list[i+j].p = (update_i) ? a[i].p : b[j].p;
                if (prev_update != update_i) {
                    for (int idx = prev; idx < i+j; idx++) {
                        if (update_i)   list[idx].l = i;
                        else            list[idx].r = j;
                    }
                    prev = i + j;
                }
                if (update_i) {
                    list[i+j].l = i;
                    i++;
                } else {
                    list[i+j].r = j;
                    j++;
                }
                prev_update = update_i;
            }
            for (int idx = prev; idx < sz; idx++) {
                if (!update_i)   list[idx].l = INVALID;
                else             list[idx].r = INVALID;
            }
            return list;
        };

        function<tnode*(int,int)> make;
        make = [&](int l, int r) {
            if (l > r) return (tnode*) nullptr;
            if (l == r) return new tnode(array[l]);
            int m = (l + r) / 2;
            tnode *cur = new tnode(array[m]);
            cur->l = make(l, m);
            cur->r = make(m+1, r);
            cur->assc = merge(cur->l->assc, cur->r->assc);
            return cur;
        };
        root = make(0, n-1);
    }

    vector<point> query(point p1, point p2) {
        vector<point> answer;
        tnode *split = root;
        while (split) {
            point p = split->p;
            if (p1 < p && p2 < p)       split = split->l;
            else if (p1 > p && p2 > p)  split = split->r;
            else                        break;
        }
        if (split == nullptr) return answer;
        if (!split->l && !split->r) {
            if (p1.y <= split->p.y && 
                p2.y >= split->p.y) {
                answer.push_back(split->p);
            }
            return answer;
        }

        auto &array = split->assc;
        auto it = lower_bound(array.begin(), array.end(), anode(p1), 
            [](anode a, anode b) { return a.p.y < b.p.y; }
        );
        if (it == array.end()) return answer;
        int start = it - array.begin();

        int start1 = split->assc[start].l;
        tnode *cur = split->l;
        while (cur->l || cur->r) {
            if (start1 == INVALID) break;
            point p = cur->p;
            if (p1 <= cur->p) {
                auto &array = cur->r->assc;
                int s = cur->assc[start1].r;
                for (int i = s; i < array.size(); i++) {
                    if (array[i].p.y > p2.y) break;
                    answer.push_back(array[i].p);
                }
                start1 = cur->assc[start1].l;
                cur = cur->l;
            } else {
                start1 = cur->assc[start1].r;
                cur = cur->r;
            }
        }
        if (p1.x <= cur->p.x && cur->p.x <= p2.x &&
            p1.y <= cur->p.y && cur->p.y <= p2.y) {
            answer.push_back(cur->p);
        }

        int start2 = split->assc[start].r;
        cur = split->r;
        while (cur->l || cur->r) {
            if (start2 == INVALID) break;
            point p = cur->p;
            if (p2 >= cur->p) {
                auto &array = cur->l->assc;
                int s = cur->assc[start2].l;
                for (int i = s; i < array.size(); i++) {
                    if (array[i].p.y > p2.y) break;
                    answer.push_back(array[i].p);
                }
                start2 = cur->assc[start2].r;
                cur = cur->r;
            } else {
                start2 = cur->assc[start2].l;
                cur = cur->l;
            }
        }
        if (p1.x <= cur->p.x && cur->p.x <= p2.x &&
            p1.y <= cur->p.y && cur->p.y <= p2.y) {
            answer.push_back(cur->p);
        }
        return answer;
    }

    void clear() { 
        clear(root); 
        root = nullptr;
    }

    void clear(tnode *node) {
        if (node->l) clear(node->l);
        if (node->r) clear(node->r);
        delete node;
    }

    void display() { display(root); }
    void display(tnode *node) {
        if (node->l) {
            printf("(%d %d) -> (%d %d)\n",
                node->p.x, node->p.y,
                node->l->p.x, node->l->p.y);
        }
        if (node->r) {
            printf("(%d %d) -> (%d %d)\n",
                node->p.x, node->p.y,
                node->r->p.x, node->r->p.y);
        }
        printf("assc: ");
        for (anode a: node->assc) printf("(%d %d), ", a.p.x, a.p.y);
        printf("\n");
        printf("assc-l: ");
        for (anode a: node->assc) printf("%d, ", a.l);
        printf("\n");
        printf("assc-r: ");
        for (anode a: node->assc) printf("%d, ", a.r);
        printf("\n\n");
        if (node->l) display(node->l);
        if (node->r) display(node->r);
    }

    ~rangetree() { clear(); }
};