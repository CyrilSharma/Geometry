#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

template <typename T>
struct point {
    T x, y;
    point(T x, T y): x(x), y(y) {}
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
struct kdtree {
    int n;
    using point = point<T>;
    vector<point> t;
    kdtree(vector<point> &a): t(a)  {
        n = a.size();
        build(0, n, false);
    }

    void build(int L, int R, bool cy) {
        if (L >= R) return;
        int M = (L + R) / 2;
        auto comp = (cy) ? compy : compx;
        auto start = t.begin();
        nth_element(start+L, start+M, start+R, comp);
        build(L, M, !cy);
        build(M+1, R, !cy);
    }
    vector<point> query(point p1, point p2) {
        const T mn = numeric_limits<T>::min();
        const T mx = numeric_limits<T>::max();
        const point MN = {mn, mn};
        const point MX = {mx, mx};
        return query(p1, p2, MN, MX, 0, n, 0);
    }
    vector<point> query(point p1, point p2, point BL, point TR, int L, int R, bool cy) {
        vector<point> answer;
        if (L >= R) return answer;
        int M = (L + R) / 2;
        if (p1.x <= BL.x && TR.x <= p2.x &&
            p1.y <= BL.y && TR.y <= p2.y) {
            return report(L, R);
        }
        if (p1.x <= t[M].x && t[M].x <= p2.x &&
            p1.y <= t[M].y && t[M].y <= p2.y) {
            answer.push_back(t[M]);
        }
        auto comp = (cy) ? compy : compx;
        point BP = (cy) ? point(p2.x, t[M].y) : point(t[M].x, p2.y);
        point TP = (cy) ? point(p1.x, t[M].y) : point(t[M].x, p1.y);
        point BC = (cy) ? point(TR.x, t[M].y) : point(t[M].x, TR.y);
        point TC = (cy) ? point(BL.x, t[M].y) : point(t[M].x, BL.y);
        if (comp(p1, t[M]) && comp(p2, t[M])) return query(p1, p2, BL, BC, L, M, !cy);
        if (comp(t[M], p1) && comp(t[M], p2)) return query(p1, p2, TC, TR, M+1, R, !cy);
        auto left = query(p1, BP, BL, BC, L, M, !cy);
        answer.insert(answer.end(), left.begin(), left.end());
        auto right = query(TP, p2, TC, TR, M+1, R, !cy);
        answer.insert(answer.end(), right.begin(), right.end());
        return answer;
    }

    vector<point> report(int L, int R) {
        vector<point> ret;
        if (L >= R) return ret;
        int M = (L + R) / 2;
        ret.push_back(t[M]);
        auto left = report(L, M);
        ret.insert(ret.end(), left.begin(), left.end());
        auto right = report(M+1, R);
        ret.insert(ret.end(), right.begin(), right.end());
        return ret;
    }

    static bool compx(point a, point b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }

    static bool compy(point a, point b) {
        if (a.y != b.y) return a.y < b.y;
        return a.x < b.x;
    }

    void display() {
        for (point p : t) {
            printf("(%d %d), ", p.x, p.y);
        }
        printf("\n");
    }
};