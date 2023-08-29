#include <iostream>
#include <random>
#include <tuple>
using namespace std;
template<typename T>
struct treap {
    struct node {
        T value;
        int prior;
        node *l, *r;
        node(T v) : value(v), prior(rand()) {
            l = r = 0;
        }
        node(node *nd) {
            value = nd->value;
            prior = nd->prior;
            l = nd->l, r = nd->r; 
        }
    };

    node *root;
    vector<node*> roots;
    treap(): root(nullptr) {}

    pair<node*, node*> split(node *ptr, T value) {
        if (ptr == 0) return {0, 0};
        node *L = 0, *R = 0;
        if (value > ptr->value) {
            L = new node(ptr);
            tie(L->r, R) = split(ptr->r, value);
        } else {
            R = new node(ptr);
            tie(L, R->l) = split(ptr->l, value);
        }
        return {L, R};
    }

    node* merge(node *small, node *large) {
        if (!small) return large;
        if (!large) return small;
        if (small->prior > large->prior) {
            node *copy = new node(small);
            copy->r = merge(small->r, large);
            return copy;
        } else {
            node *copy = new node(large);
            copy->l = merge(small, large->l);
            return copy;
        }
    }

    void insert(T value) {
        auto [L, R] = split(root, value);
        L = merge(L, new node(value));
        roots.push_back(root = merge(L, R));
    }

    void erase(T value) { 
        roots.push_back(root = erase(root, value)); 
    }
    
    node* erase(node *ptr, T value) {
        if (ptr == nullptr) return nullptr;
        node *copy = new node(ptr);
        if      (value < copy->value) copy->l = erase(copy->l, value);
        else if (copy->value < value) copy->r = erase(copy->r, value);
        else                           return merge(copy->l, copy->r);
        return copy;
    }

    int count(int t, T value) {
        node *cur = roots[t];
        while (cur) {
            if (value < cur->value)      cur = cur->l;
            else if (value > cur->value) cur = cur->r;
            else                         break;
        }
        return (cur == nullptr) ? 0 : 1;
    }

    void print(int t) { print(roots[t]), printf("\n"); }
    void print(node *nd) {
        if (nd->l) print(nd->l);
        printf("%d ", nd->value);
        if (nd->r) print(nd->r); 
    }
};