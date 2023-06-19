#include <iostream>

struct Foo {
    virtual void bar(void) final { std::cout << "Foo::bar() called!" << std::endl; }
};

int main(void) {
    auto x = Foo();
    x.bar();

    return 0;
}