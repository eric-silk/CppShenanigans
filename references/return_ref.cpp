#include <iostream>

/* Bad, don't do
int& return_int(void) {
    int i = 1523;
    return i;
}
*/

struct foo {
    foo(const int& i) : i(i) {};

    const int& i;
};

foo foo_factory(void) {
    int tmp = 1523;
    return foo(tmp);
}

int main(void) {
    // Bad, don't do
    // const int& i = return_int();

    int  j  = 1234;
    auto f1 = foo(j);
    auto f2 = foo_factory();

    // printed 1234
    // reference is still in scope -> still exists -> still valid
    std::cout << "f1.i is: " << f1.i << std::endl;
    // printed 32700
    // reference no longer in scope -> doesn't exist -> no longer valid
    std::cout << "f2.i is: " << f2.i << std::endl;

    return 0;
}