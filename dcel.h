#include <iostream>
#include <functional>
#include <list>
#include <map>
#include <vector>
using namespace std;

template <typename T>
struct point {
    T x, y;
    point(T _x, T _y): x(_x), y(_y) {}
};

/* NOTE. Need a sweepline to account for holes.
 * The below implementation assumes no holes.
 */
template <typename T>
struct dcel {
    using point = point<T>;
    struct face;
    struct vertex;
    struct edge {
        vertex *origin;
        edge *twin, *prev, *next;
        face *f;
        edge(): twin(0), prev(0), next(0), f(0) {}
    };

    struct face {
        edge *out;
        vector<edge*> holes;
        face(): out(0) {}
    };

    struct vertex {
        point pos;
        edge *edge;
        vertex(point _pos): pos(_pos), edge(0) {}
    };

    list<face> faces;
    list<vertex> vertexes;
    list<edge> edges;

    void build(vector<point> points, vector<vector<int>> adj) {
        vector<vector<edge*>> graph; // can make global
        for (point p: points) vertexes.push_back(vertex(p));
        for (int u = 0; u < adj.size(); u++) {
            for (int v = 0; v < adj[u].size(); v++) {
                edges.push_back(edge());
                edges.push_back(edge());
                auto &h1 = *(next(edges.rbegin()));
                auto &h2 = *(edges.rbegin());
                h1.origin->pos = points[u];
                h2.origin->pos = points[v];
                h1.twin = &h2;
                h2.twin = &h1;
                graph[u].push_back(&h2);
                graph[v].push_back(&h1);
            }
        }
        for (auto &list: graph) {
            sort(list.begin(), list.end(), [&](edge *u, edge *v) {
                auto [x1, y1] = u->origin->pos;
                auto [x2, y2] = v->origin->pos;
                long long det = (x1*y2 - y1*x2);
                return det > 0;
            });
        }
        for (int u = 0; u < graph.size(); u++) {
            auto &list = graph[u];
            for (int i = 0; i < list.size(); i++) {
                edge *prev = i ? list[i-1] : list.back();
                edge *cur = list[i];
                prev->twin->next = cur;
                cur->prev = prev->twin;
            }
            vertex *v = list[0]->origin;
            v->edge = list[0];
        }
        for (edge &e : edges) {
            if (e.f) continue;
            faces.push_back(face());
            auto f = &faces.back();
            auto cur = &e;
            do {
                cur->f = f;
                cur = cur->next;
            } while (cur != &e);
        }
    }
};