#include <iostream>

template <class Type1>
struct Base {
    Base() : type1(0) {}

    Type1 type1;
};

template <class Type1, class Type2>
struct Child : public Base<Type1> {
    Child() : type2(1) {}

    Type2 type2;
};

int main(void) {
    auto base  = Base<int>();
    auto child = Child<int, float>();

    std::cout << "base: " << base.type1 << std::endl;
    std::cout << "child: " << child.type1 << " " << child.type2 << std::endl;

    return 0;
}