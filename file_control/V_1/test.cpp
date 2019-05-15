#include <iostream>
using namespace std;

class A {
public:
    A() {     cout << "1";    }
    ~A() {    cout << "2";    }
};
class B: public A {
public:
    B() {    cout << "3";    }
    ~B() {    cout << "4";    }
};
int main() {
    B b;
    return 0;
}
