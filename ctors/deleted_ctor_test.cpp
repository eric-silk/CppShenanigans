#include <iostream>
#include <string>

class Base {
    public:
    // Won't work
    // Base() = delete;
    virtual void pure_virtual(void) = 0;

    virtual void not_pure_virtual(void) { std::cout << "Base not_pure_virtual" << std::endl; }

    void concrete(void) { std::cout << "Base concrete" << std::endl; }

    protected:
    Base() = default;
};

class DerivedWithConcreteOverride : public Base {
    public:
    DerivedWithConcreteOverride() {
        std::cout << "DerivedWithConcreteOverride default ctor" << std::endl;
    }

    DerivedWithConcreteOverride(const std::string& msg) {
        std::cout << "DerivedWithConcreteOverride non-default: " << msg << std::endl;
    }

    void pure_virtual(void) override final {
        std::cout << "DerivedWithConcreteOverride pure_virtual" << std::endl;
    }

    void not_pure_virtual(void) override final {
        std::cout << "DerivedWithConcreteOverride not_pure_virtual" << std::endl;
    }

    void concrete(void) { std::cout << "DerivedWithConcreteOverride concrete" << std::endl; }
};

class DerivedWithoutConcreteOverride : public Base {
    public:
    DerivedWithoutConcreteOverride() {
        std::cout << "DerivedWithoutConcreteOverride default ctor" << std::endl;
    }

    DerivedWithoutConcreteOverride(const std::string& msg) {
        std::cout << "DerivedWithoutConcreteOverride non-default: " << msg << std::endl;
    }

    void pure_virtual(void) override final {
        std::cout << "DerivedWithoutConcreteOverride pure_virtual" << std::endl;
    }

    void not_pure_virtual(void) override final {
        std::cout << "DerivedWithoutConcreteOverride not_pure_virtual" << std::endl;
    }
};

int main(void) {
    // Won't work
    // auto base = Base();

    auto derived_override = DerivedWithConcreteOverride();
    derived_override      = DerivedWithConcreteOverride("wooters");
    derived_override.pure_virtual();
    derived_override.not_pure_virtual();
    derived_override.concrete();

    auto derived_no_override = DerivedWithoutConcreteOverride();
    derived_no_override      = DerivedWithoutConcreteOverride("wooters");
    derived_no_override.pure_virtual();
    derived_no_override.not_pure_virtual();
    derived_no_override.concrete();

    return 0;
}