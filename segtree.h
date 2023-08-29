#include <iostream>
#include <vector>
using namespace std;
struct segtree {
  int n;
  vector<int> t;
  segtree(vector<int> array) {
    n = array.size();
    t.resize(4 * n);
    build(array, 1, 0, n-1);
  }

  void build(vector<int> &a, int v, int bl, int br) {
    if (bl == br) {
      t[v] = a[bl];
    } else {
      int m = (bl + br) / 2;
      build(a, 2*v, bl, m);
      build(a, 2*v + 1, m + 1, br);
      t[v] = t[2*v] + t[2*v + 1];
    }
  }

  int query(int l, int r) {
    return query(1, l, r, 0, n - 1);
  }

  int query(int i, int l, int r, int bl, int br) {
    if (l > r) return 0;
    if (l == bl && r == br) return t[i];
    int m = (bl + br) / 2;
    return query(2*i, l, min(m, r), bl, m) + 
      query(2*i+1, max(l, m+1), r, m + 1, br);
  }
};