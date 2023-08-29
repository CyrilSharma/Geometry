#include "assert.h"
#include <iostream>
using namespace std;
#define RED 1
#define BLACK 0
struct llrb {
    struct node {
        int value, color, total;
        node *left, *right;
        node(int v) {
            value = v;
            color = RED;
            total = 1;
            left = nullptr;
            right = nullptr;
        }
    };
    using node_ptr = node *; //unique_ptr<node>;

    int n;
    node_ptr root;
    llrb(): root(nullptr), n(0) {}
    
    int count(int val) {
        auto cur = root;
        while (cur != nullptr) {
            if (cur->value < val)
                cur = cur->right;
            else if (cur->value > val)
                cur = cur->left;
            else
                return 1;
        }
        return 0;
    }

    int rank(int val) { return rank(root, val); }
    int rank(node_ptr ptr, int val) {
        if (ptr == nullptr) return 0;
        int cnt = 0;
        if (val < ptr->value) cnt += rank(ptr->left, val);
        else cnt += total(ptr->left) + 1 + rank(ptr->right, val);
        return cnt;
    }

    int total(node_ptr item) {
        if (item == nullptr) return 0;
        return item->total;
    }

    void insert(int val) {
        root = insert(root, val);
        n++;
    }

    node_ptr insert(node_ptr start, int val) {
        if (start == nullptr) return new node(val);
        if (start->value < val) start->right = insert(start->right, val);
        else start->left = insert(start->left, val);

        if (color(start->right) == RED) 
            start = rotate_left(start);
        if (color(start->left) == RED && color(start->left->left) == RED) 
            start = rotate_right(start);
        if (color(start->left) == RED && color(start->right) == RED)
            color_flip(start);
        
        start->total = total(start->left) + total(start->right) + 1;
        return start;
    }

    int color(node_ptr item) {
        if (item == nullptr) return BLACK;
        return item->color;
    }

    node_ptr rotate_left(node_ptr start) {
        auto temp = start;
        start = start->right;

        temp->right = start->left;
        temp->total = total(temp->left) + total(temp->right) + 1;

        start->left = temp;
        start->total = total(start->left) + total(start->right) + 1;

        start->color = temp->color;
        temp->color = RED;
        return start;
    }

    node_ptr rotate_right(node_ptr start) {
        auto temp = start;
        start = start->left;

        temp->left = start->right;
        temp->total = total(temp->left) + total(temp->right) + 1;

        start->right = temp;
        start->total = total(start->left) + total(start->right) + 1;

        start->color = temp->color;
        temp->color = RED;
        return start;
    }

    void color_flip(node_ptr start) {
        start->left->color = BLACK;
        start->right->color = BLACK;
        if (start != root)
            start->color = RED;
    }

    int size() { return n; }

    void clear() {
        clear(root);
        n = 0;
    }

    void clear(node_ptr ptr) {
        if (ptr->left)  clear(ptr->left);
        if (ptr->right) clear(ptr->right);
        delete ptr;
    }

    ~llrb() { clear(); }
};