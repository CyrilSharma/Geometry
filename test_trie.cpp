#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "trie.h"

#include <algorithm>
#include "doctest.h"
#include <iostream>
#include <random>
#include <vector>

using namespace std;

TEST_CASE("Lookup") {
    vector<string> strings1 = {
        "hello",
        "lasagna",
        "pickles",
        "hummus",
        "oogie",
        "word"
    };

    trie t;
    for (auto word: strings1)
        t.insert(word);

    for (auto word: strings1)
        CHECK(t.contains(word));

    vector<string> strings2 = {
        "helloz",
        "lasagnaz",
        "pickle",
        "hummu",
        "oogi",
        "wor"
    };

    for (auto word: strings2)
        CHECK(!t.contains(word));
}