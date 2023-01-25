#include <iostream>

struct Base {
    Base() : x(5) {};

    Base(const Base& rhs) { x = rhs.x; }

    Base& operator=(const Base& rhs) {
        this->x = rhs.x;
        return *this;
    }

    void set_x(const int x) { this->x = x; }

    const int get_x(void) { return x; }

    protected:
    int x;
};

struct Derived : public Base {
    Derived() : Base(), y(10) {};

    Derived(const Derived& rhs) { y = rhs.y; }

    Derived& operator=(const Derived& rhs) {
        this->y = rhs.y;
        return *this;
    }

    void set_y(const int y) { this->y = y; }

    const int get_y(void) { return y; }

    protected:
    int y;
};

struct Proper : public Base {
    Proper() : Base(), y(10) {};

    Proper(const Proper& rhs) : Base(rhs), y(rhs.y) {};

    Proper& operator=(const Proper& rhs) {
        Base::operator=(rhs);
        this->y = rhs.y;
        return *this;
    }

    void set_y(const int y) { this->y = y; }

    const int get_y(void) { return y; };

    protected:
    int y;
};

int main(void) {
    Derived derived1 = Derived();
    Derived derived2 = derived1;

    derived1.set_x(1);
    Derived derived3 = derived1;
    Derived derived4 = Derived();
    derived4         = derived1;

    std::cout << "derived1.x: " << derived1.get_x() << std::endl;
    std::cout << "derived2.x: " << derived2.get_x() << std::endl;
    std::cout << "derived3.x: " << derived3.get_x() << std::endl;
    std::cout << "derived4.x: " << derived4.get_x() << std::endl;

    Proper proper1 = Proper();
    Proper proper2 = proper1;

    proper1.set_x(1);
    Proper proper3 = proper1;
    Proper proper4 = Proper();
    proper4        = proper1;

    std::cout << "proper1.x: " << proper1.get_x() << std::endl;
    std::cout << "proper2.x: " << proper2.get_x() << std::endl;
    std::cout << "proper3.x: " << proper3.get_x() << std::endl;
    std::cout << "proper4.x: " << proper4.get_x() << std::endl;

    return 0;
}