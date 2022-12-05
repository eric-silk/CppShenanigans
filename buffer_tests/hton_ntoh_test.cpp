#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>

// We'll need to add some special handling if we end up on a system with bytes != 8 bits
// (some DSP's, some TI MCU's, etc.)
// Note also that while std::byte is effectively an alias of unsigned char, there is no defined
// template specialization of numeric_limits and thus it reports 0 bits; hence, the use of unsigned
// char rather than std::byte here
constexpr auto BITS_PER_BYTE = std::numeric_limits<unsigned char>::digits;
static_assert(BITS_PER_BYTE == 8);

// From here: https://stackoverflow.com/a/13352059/4718836
// with modifications (std::byte instead of uint8_t)
template <class T>
static inline T hton_any(const T& input) {
    T                 output(0);
    const std::size_t size = sizeof(input);
    std::byte*        data = reinterpret_cast<std::byte*>(&output);

    for (std::size_t i = 0; i < size; i++) {
        data[i] =
            std::byte(reinterpret_cast<const size_t&>(input) >> ((size - i - 1) * BITS_PER_BYTE));
    }

    return output;
};

// ntoh and hton are inverses, the naming is a convenience
template <class T>
static inline T ntoh_any(const T& input) {
    return hton_any(input);
};

template <class T>
static inline T hton_any_stl(const T& input) {
    const std::size_t           size = sizeof(input);
    std::array<std::byte, size> data = {std::byte(0)};

    for (std::size_t i = 0; i < size; i++) {
        data[i] =
            std::byte(reinterpret_cast<const size_t&>(input) >> ((size - i - 1) * BITS_PER_BYTE));
    }

    return *reinterpret_cast<T*>(data.data());
};

template <class T>
static inline T ntoh_any_stl(const T& input) {
    return hton_any_stl(input);
};

template <class T>
static inline void test_endianness_conversion(const T& value) {
    const T value_swapped = hton_any(value);
    const T value_back    = ntoh_any(value_swapped);

    std::cout << typeid(T).name() << ((value == value_back) ? ": Yes" : ": No") << ::std::endl;
}

template <class T>
static inline void test_endianness_conversion_stl(const T& value) {
    const T value_swapped = hton_any_stl(value);
    const T value_back    = ntoh_any_stl(value_swapped);

    std::cout << typeid(T).name() << ((value == value_back) ? ": Yes" : ": No") << ::std::endl;
}

int main(void) {
    std::cout << "Raw C Arrays:" << std::endl;
    test_endianness_conversion<uint8_t>(5);
    test_endianness_conversion<uint8_t>(253);

    test_endianness_conversion<int>(2523);

    test_endianness_conversion<float>(632642.54534);
    test_endianness_conversion<float>(-632642.54534);

    test_endianness_conversion<double>(5345235235.62834562);
    test_endianness_conversion<double>(-5345235235.62834562);

    std::cout << "STL Arrays:" << std::endl;
    test_endianness_conversion_stl<uint8_t>(5);
    test_endianness_conversion_stl<uint8_t>(253);

    test_endianness_conversion_stl<int>(2523);

    test_endianness_conversion_stl<float>(632642.54534);
    test_endianness_conversion_stl<float>(-632642.54534);

    test_endianness_conversion_stl<double>(5345235235.62834562);
    test_endianness_conversion_stl<double>(-5345235235.62834562);

    return 0;
}