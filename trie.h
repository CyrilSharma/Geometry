#include <iostream>
using namespace std;
struct trie {
    struct node {
        bool end;
        node *children[26];
        node(): end(false) {}
    };

    node *root;
    trie(): root(new node()) {}

    void insert(string word) {
        int pos = 0;
        node *prev = nullptr, *cur = root;
        for (; pos < word.size(); pos++) {
            prev = cur;
            cur = cur->children[index(word[pos])];
            if (cur == nullptr) break;
        }

        for (; pos < word.size(); pos++) {
            prev->children[index(word[pos])] = new node();
            prev = prev->children[index(word[pos])];
        }
        prev->end = true;
    }

    bool contains(string word) {
        auto cur = root;
        for (int i = 0; i < word.length(); i++) {
            cur = cur->children[index(word[i])];
            if (cur == nullptr) return false;
        }
        return cur->end;
    }

    int index(char c) { return c - 'a'; }

    void clear() { clear(root); }
    void clear(node *ptr) {
        for (int i = 0; i < 26; i++) {
            if (ptr->children[i])
                clear(ptr->children[i]);
        }
        delete ptr;
    }
    ~trie() { clear(); }
};