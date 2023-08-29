#include <random>
#include <utility>
using namespace std;
struct treap {
    struct node {
        int value, total, priority;
        node *left, *right;
        node(int val) {
            value = val;
            total = 1;
            priority = rand();
            left = nullptr;
            right = nullptr;
        }
    };

    int n = 0;
    node *root = nullptr;
    treap() {}

    int total(node *item) {
        return (item) ? item->total : 0;
    }

    pair<node*, node*> split(node* ptr, int val) {
        if (!ptr) return {nullptr, nullptr};
        node *L = 0, *R = 0;
        if (ptr->value <= val) {
            L = ptr;
            tie(L->right, R) = split(ptr->right, val);
            L->total = total(L->left) + total(L->right) + 1;
        } else  {
            R = ptr;
            tie(L, R->left) = split(ptr->left, val);
            R->total = total(R->left) + total(R->right) + 1;
        }
        return {L, R};
    }

    node *merge(node *t1, node *t2) {
        if (!t1) return t2;
        if (!t2) return t1;
        if (t1->priority <= t2->priority) {
            t2->left = merge(t1, t2->left);
            t2->total = total(t2->left) + total(t2->right) + 1;
            return t2;
        } else {
            t1->right = merge(t1->right, t2);
            t1->total = total(t1->left) + total(t1->right) + 1;
            return t1;
        }
    }

    void insert(int val) {
        auto [T1, T2] = split(root, val);
        T1 = merge(T1, new node(val));
        root = merge(T1, T2);
        n++;
    }

    int count(int val) {
        node *cur = root;
        while (cur) {
            if (val < cur->value) {
                cur = cur->left;
            } else if (val > cur->value) {
                cur = cur->right;
            } else {
                return 1;
            }
        }
        return 0;
    }

    int select(int rank) { return select(root, rank); }
    int select(node *ptr, int count) {
        assert(count >= 1 && count <= n);
        if (total(ptr->left) < count) {
            count -= total(ptr->left);
        } else {
            return select(ptr->left, count);
        }
        if (count == 1) return ptr->value;
        return select(ptr->right, count - 1);
    }

    void clear() { 
        clear(root);
        n = 0;
    }
    void clear(node *ptr) {
        if (ptr->left)  clear(ptr->left);
        if (ptr->right) clear(ptr->right);
        delete ptr;
    }
    ~treap() { clear(); }
};