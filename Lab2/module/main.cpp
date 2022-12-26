#include <iostream>
#include "../Array.h"

int main() {
    Array<int> a{16};

    for (int i = 0; i < 16; ++i){
        a.insert(i + 1);
    }

    a.insert(5, -50);
    Array<int> b = a;

    for (int i = 0; i < b.size(); ++i){
        b[i] *= 2;
    }

    Array<int> c;
    c=a;

    for (auto it = c.reverseIterator(); it.hasNext(); it.next()){
        std::cout << it.get() << std::endl;
    }
}
