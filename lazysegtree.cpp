#include <bits/stdc++.h>
using namespace std;
struct SegTree {
  int n;
  vector<int> incr, tree;
  SegTree(vector<int> nums): n(nums.size()) {
    tree.resize(4*n + 5, 0);
    incr.resize(4*n + 5, 0);
    build(1, 0, n-1, nums);
  }
  void build(int i, int bl, int br, vector<int> &nums) {
    if (bl == br) {
      tree[i] = nums[bl];
      return;
    }
    int m = (bl + br) / 2;
    build(2*i,        bl,  m, nums);
    build(2*i + 1,   m+1, br, nums);
    tree[i] = tree[2*i] + tree[2*i + 1];
  }
  int below(int r) {
    return access(1, 0, n-1, 0, r-1, 0);
  }
  int access(int l, int r) {
    return access(1, 0, n-1, l, r, 0);
  }
  int access(int i, int bl, int br, int l, int r, int tot) {
    if (l > r) return 0;
    if (bl == l && r == br) {
      return tree[i] + (incr[i] + tot) * 
        (r - l + 1);
    }
    int m = (bl + br) / 2;
    return 
      access(i*2,     bl,   m, l,   min(m, r), tot + incr[i]) + 
      access(i*2 + 1, m+1, br, max(l, m+1), r, tot + incr[i]);
  }
  void add(int l, int r, int a) {
    add(1, 0, n-1, l, r, a);
  }
  void add(int i, int bl, int br, int l, int r, int a) {
    if (l > r) return;
    if (bl == l && r == br) {
      incr[i] += a;
      return;
    }
    int m = (bl + br) / 2;
    add(i*2,      bl,  m, l,   min(m, r), a);
    add(i*2 + 1, m+1, br, max(l, m+1), r, a);
  }
};

int main() {
  vector<int> nums(20, 0);
  nums[0] = 1, nums[1] = 1;
  SegTree s = SegTree(nums);
  auto print = [&](SegTree &tree) {
    for (int i = 0; i < nums.size(); i++) {
      cout<<tree.access(i,i)<<" ";
    }
    cout<<"\n";
  };
  print(s);
  s.add(4, 8, 1);
  print(s);
  s.add(4, 8, -1);

  s.add(3, 5, 1);
  print(s);
  s.add(3, 5, -1);

  int idx = s.below(3);
  cout<<idx<<endl;
}