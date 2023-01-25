#include <iostream>

template <class Type1>
struct Base {
    Base() : type1(0) {}

    Type1 type1;
};

template <class Type1>
struct Child : public Base<Type1> {
    Child() = default;

    template <class T>
    void foo(T x) {
        std::cout << "x: " << x << std::endl;
    }
};

int main(void) {
    auto child = Child<int>();

    constexpr float x = 1234.567;
    child.foo(x);

    return 0;
}