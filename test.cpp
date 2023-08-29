#include <set>
#include <iostream>

struct MyClass {
    int currentValue;

    struct MyComparator {
        const MyClass* owner;

        explicit MyComparator(const MyClass* owner) : owner(owner) {}

        int operator()(int a, int b) const {
            if (a * owner->currentValue < b * owner->currentValue) {
                return -1; // a is less than b
            } else if (a * owner->currentValue > b * owner->currentValue) {
                return 1; // a is greater than b
            }
            return 0; // a and b are equal
        }
    };

    std::set<int, MyComparator> mySet;

    MyClass(int value) : currentValue(value), mySet(MyComparator(this)) {}
};

int main() {
    MyClass obj(2);
    obj.mySet.insert(3);
    obj.mySet.insert(1);
    obj.mySet.insert(2);

    int target = 3;

    auto it = obj.mySet.find(target);

    if (it != obj.mySet.end()) {
        std::cout << "Element found: " << *it << std::endl;
    } else {
        std::cout << "Element not found" << std::endl;
    }

    return 0;
}
