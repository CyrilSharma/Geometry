#include <vector>
using namespace std;
struct dsu {
    vector<int> parent, size;
    dsu(int n) {
        parent.resize(n, -1);
        size.resize(n, 1);
    }
    int find(int node) {
        if (parent[node] == -1) return node;
        return parent[node] = find(parent[node]);
    }
    void merge(int a, int b) {
        int s = find(a), l = find(b);
        if (s == l) return;
        if (size[s] > size[l]) swap(s, l);
        size[l] += size[s];
        parent[s] = l;
    }
};