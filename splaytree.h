#include "assert.h"
#include <iostream>
using namespace std;
struct splaytree {
    struct node {
        int value;
        node *left, *right, *parent;
        node(int v) {
            value = v;
            parent = nullptr;
            left = nullptr;
            right = nullptr;
        }
    };

    int n;
    node *root;
    splaytree(): root(nullptr), n(0) {}

    void splay(node *cur) {
        while (cur->parent && cur->parent->parent) {
            rotate(cur->parent);
            rotate(cur);
        }
        if (cur->parent) rotate(cur);
        root = cur;
    }
    
    int count(int val) {
        auto cur = root;
        while (cur != nullptr) {
            if (cur->value < val) {
                cur = cur->right;
            } else if (cur->value > val) {
                cur = cur->left;
            } else {
                splay(cur);
                return 1;
            }
        }
        return 0;
    }

    void insert(int val) {
        n++;
        if (root == nullptr) {
            root = new node(val);
            return;
        }

        /* insert */
        node **child_ptr = nullptr;
        node *cur = root, *prev = nullptr;
        while (cur) {
            prev = cur;
            if (cur->value <= val) {
                child_ptr = &cur->right;
                cur = cur->right;
            } else {
                child_ptr = &cur->left;
                cur = cur->left;
            }
        }
        (*child_ptr) = new node(val);
        cur = *child_ptr;
        cur->parent = prev;
        splay(cur);
    }

    void rotate(node *a) {
        if (is_left(a))      zig(a->parent);
        else                 zag(a->parent);
    }

    bool is_left(node *a) {
        assert(a->parent != nullptr);
        return a->parent->left == a;
    }

    void zag(node *start) {
        auto temp = start;
        start = start->right;
        start->parent = temp->parent;
        auto dad = temp->parent;
        if (dad != nullptr) {
            if (is_left(temp))  dad->left  = start;
            else                dad->right = start;
        }

        temp->right = start->left;
        if (temp->right) {
            temp->right->parent = temp;
        }

        start->left = temp;
        temp->parent = start;
    }

    void zig(node *start) {
        auto temp = start;
        start = start->left;
        start->parent = temp->parent;
        auto dad = temp->parent;
        if (dad != nullptr) {
            if (is_left(temp))  dad->left  = start;
            else                dad->right = start;
        }
    
        temp->left = start->right;
        if (temp->left) {
            temp->left->parent = temp;
        }

        start->right = temp;
        temp->parent = start;
    }

    int size() { return n; }

    /* For a better implementation, see
     * https://github.com/gcc-mirror/gcc/blob/master/libiberty/splay-tree.c */
    void clear() {
        node *ptr = root;
        while (ptr) {
            while (ptr->left || ptr->right) {
                if (ptr->left)      ptr = ptr->left;
                else                ptr = ptr->right;       
            }
            node *parent = ptr->parent;
            if (parent) {
                if (parent->left == ptr)    parent->left = nullptr;
                else                        parent->right = nullptr;
            }
            delete ptr;
            ptr = parent;
        }
        n = 0;
    }

    ~splaytree() { clear(); }
};