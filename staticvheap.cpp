#include <bits/stdc++.h>
#include <stdlib.h>
using namespace std;
const int AMT = 60000000;
void f0() {
  int* v = (int*) calloc(AMT, sizeof(int));
  cout<<"Random El: "<<v[rand()%AMT]<<endl;
}
void f1() {
  vector<int> v(AMT);
  cout<<"Random El: "<<v[rand()%AMT]<<endl;
}
void f2() {
  if (AMT > 1e5) {
    cout<<"Too Large For Stack"<<endl;
  } else {
    int v[AMT] = { 0 };
    cout<<"Random El: "<<v[rand()%AMT]<<endl;
  }
}
void f3() {
  static int v[AMT] = { 0 };
  cout<<"Random El: "<<v[rand()%AMT]<<endl;
}
void f4() {
  int *v = new int[AMT];
  cout<<"Random El: "<<v[rand()%AMT]<<endl;
}
void f5() {
  static vector<int> v(AMT);
  cout<<"Random El: "<<v[rand()%AMT]<<endl;
}

int main() {
  srand(time(NULL)); // Seed the random number generator

  // Create an array of function pointers
  void (*functions[])() = {f0, f1, f2, f3, f4, f5};
  for (int i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i) {
    auto start = chrono::high_resolution_clock::now();
    functions[i]();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time taken by f" << i << "(): " << elapsed.count() << " seconds" << endl;
  }
  return 0;
}