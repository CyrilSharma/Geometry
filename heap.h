#include <limits>
#include <vector>
using namespace std;
struct heap {
    int n;
    int elements;
    vector<int> array;
    heap(): n(1), elements(0) {}
    heap(vector<int> input): array(input) {
        elements = input.size();
        make_heap();
    }

    void make_heap() {
        makep2();
        for (int i = n/2; i >= 0; i--) {
            heap_down(i);
        }
    }

    void heap_down(int i) {
        while (i < (n-1)/2) {
            int left = 2*i + 1;
            int right = 2*i + 2;
            int best = (array[left] >= array[right]) ? left : right;
            if (array[best] > array[i]) {
                swap(array[best], array[i]);
                i = best;
            } else {
                break;
            }
        }
    }

    void heap_up(int i) {
        while (i > 0) {
            int dad = (i - 1) / 2;
            if (array[dad] < array[i]) {
                swap(array[i], array[dad]);
                i = dad;
            } else {
                break;
            }
        }
    }

    void insert(int a) {
        if (elements + 1 >= array.size())
            upsize();
        array[elements] = a;
        heap_up(elements);
        elements++;
    }

    int top() { return array[0]; }

    void pop() {
        elements--;
        swap(array[0], array[elements]);
        array[elements] = numeric_limits<int>::min();
        heap_down(0);

        if (elements < n/4)
            downsize();
    }

    void upsize() {
        array.resize(n*2, numeric_limits<int>::min());
        n *= 2;
    }

    void downsize() {
        array.resize(n/2);
        n /= 2;
    }

    void makep2() {
        int sz = 1;
        while (sz < array.size())
            sz *= 2;
        array.resize(sz, numeric_limits<int>::min());
        n = sz;
    }
};