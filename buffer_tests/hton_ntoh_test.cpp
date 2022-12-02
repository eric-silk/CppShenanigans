#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

constexpr auto BYTE_SIZE = sizeof(std::byte);

template <class T>
static const inline std::array<std::byte, sizeof(T)> hton_bytes(const T input) {
    static_assert(std::is_arithmetic_v<T>);
    constexpr size_t size_of_T = sizeof(T);

    std::array<std::byte, size_of_T> ret;
    const auto                       cast_value = static_cast<size_t>(input);
    for (std::size_t i = 0; i < size_of_T; i++) {
        const auto shift_size    = (size_of_T - i - i) * BYTE_SIZE;
        const auto shifted_value = cast_value >> shift_size;
        ret[i]                   = static_cast<std::byte>(shifted_value);
    }

    return ret;
}

template <class ReturnType, class BufferType>
static const inline ReturnType ntoh_bytes(const BufferType& bytes) {
    static_assert(std::is_arithmetic_v<ReturnType>);
    constexpr size_t size_of_ret = sizeof(ReturnType);
    assert(size_of_ret == bytes.size());

    size_t ret = 0;

    for (std::size_t i = 0; i < size_of_ret; i++) {
        // size_t to guarantee it will always fit after shifting
        const auto cast_value    = static_cast<size_t>(bytes[i]);
        const auto shift_size    = (size_of_ret - i - 1) * BYTE_SIZE;
        const auto shifted_value = cast_value << (size_of_ret - i - 1) * BYTE_SIZE;
        ret |= shifted_value;
    }

    return static_cast<ReturnType>(ret);
}

int main(void) {
    const uint8_t a = 5;
    const int     b = 123;
    const float   c = 5463.5;

    auto          a_bytes = hton_bytes(a);
    const uint8_t a_back  = ntoh_bytes<uint8_t>(a_bytes);
    auto          b_bytes = hton_bytes(123);
    const int     b_back  = ntoh_bytes<int>(b_bytes);
    auto          c_bytes = hton_bytes(c);
    const float   c_back  = ntoh_bytes<float>(c_bytes);


    const std::string uint8_worked = (a == a_back) ? "Yes" : "No";
    const std::string int_worked   = (a == a_back) ? "Yes" : "No";
    const std::string float_worked = (a == a_back) ? "Yes" : "No";
    std::cout << "uint8_t: " << uint8_worked << std::endl;
    std::cout << "int: " << int_worked << std::endl;
    std::cout << "float: " << float_worked << std::endl;

    return 0;
}