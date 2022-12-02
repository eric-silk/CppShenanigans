#include <arpa/inet.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>

#define TEST_MEMCPY 1
#define TEST_C_LOOP 0
#define TEST_C      0

using header_buffer      = std::array<std::byte, 2 * sizeof(uint32_t)>;
using body_buffer        = std::vector<std::byte>;
constexpr auto BYTE_SIZE = sizeof(std::byte);

template <class input_type>
const std::array<std::byte, sizeof(input_type)> memcpy_long_to_bytes(const input_type x) {
    const auto x_tmp = htonl(x);
    static_assert(sizeof(input_type) == sizeof(x_tmp));
    std::array<std::byte, sizeof(input_type)> buf;
    std::memcpy(buf.data(), &x_tmp, sizeof(x_tmp));

    return buf;
};

template <class input_type>
const std::array<std::byte, sizeof(input_type)> c_loop_long_to_bytes(const input_type x) {
    const auto x_tmp = htonl(x);
    static_assert(sizeof(input_type) == sizeof(x_tmp));
    std::array<std::byte, sizeof(input_type)> buf;
    for (size_t i = sizeof(input_type); i > 0; --i) {
        buf[sizeof(input_type) - i] = std::byte(x_tmp >> i * sizeof(std::byte));
    }

    return buf;
};

template <class input_type>
const std::array<std::byte, sizeof(input_type)> c_long_to_bytes(const input_type x) {
    const auto x_tmp = htonl(x);
    static_assert(sizeof(input_type) == sizeof(x_tmp));
    std::array<std::byte, sizeof(input_type)> buf;
    buf[0] = std::byte(x_tmp >> 24);
    buf[1] = std::byte(x_tmp >> 16);
    buf[2] = std::byte(x_tmp >> 8);
    buf[3] = std::byte(x_tmp >> 0);

    return buf;
};

template <class input_type>
const std::array<std::byte, sizeof(input_type)> long_to_bytes(input_type x) {
#if TEST_MEMCPY
    return memcpy_long_to_bytes(x);
#elif TEST_C_LOOP
    return c_loop_long_to_bytes(x);
#elif TEST_C
    return c_long_to_bytes(x);
#endif
}

class EphemeralHeader {
    public:
    EphemeralHeader() = delete;

    EphemeralHeader(const uint32_t address, const uint32_t packet_size)
        : address(address)
        , packet_size(packet_size) {
        // NTD
    }

    const header_buffer get_bytes() {
        header_buffer buffer;
        auto          tmp  = long_to_bytes(address);
        auto          iter = std::copy(tmp.begin(), tmp.end(), buffer.begin());
        tmp                = long_to_bytes(packet_size);
        iter               = std::copy(tmp.begin(), tmp.end(), iter);
        assert(iter == buffer.end());

        return buffer;
    }

    private:
    const uint32_t address;
    const uint32_t packet_size;
};

class EphemeralBody {
    public:
    EphemeralBody() = delete;

    EphemeralBody(const uint32_t value1, const std::vector<float> value2)
        : value1(value1)
        , value2(value2) {
        // NTD
    }

    const body_buffer get_bytes() {
        body_buffer buffer;
        auto        v1_tmp = long_to_bytes(this->value1);
        buffer.insert(buffer.end(), v1_tmp.begin(), v1_tmp.end());
        for (auto i : value2) {
            auto v2_tmp = long_to_bytes(i);
            buffer.insert(buffer.end(), v2_tmp.begin(), v2_tmp.end());
        }

        return buffer;
    }


    private:
    const uint32_t           value1;
    const std::vector<float> value2;
};

class FixedHeader {
    public:
    FixedHeader() { buffer.fill(std::byte(0)); }

    const header_buffer get_bytes(const uint32_t address, const uint32_t packet_size) {
        auto tmp  = long_to_bytes(address);
        auto iter = std::copy(tmp.begin(), tmp.end(), buffer.begin());
        tmp       = long_to_bytes(packet_size);
        iter      = std::copy(tmp.begin(), tmp.end(), iter);
        assert(iter == buffer.end());

        return buffer;
    }

    const header_buffer operator()(const uint32_t address, const uint32_t packet_size) {
        return this->get_bytes(address, packet_size);
    }

    private:
    header_buffer buffer;
};

class FixedBody {
    public:
    FixedBody() = default;

    const body_buffer get_bytes(const uint32_t value1, const std::vector<float>& value2) {
        buffer.clear();
        auto tmp = long_to_bytes(value1);
        buffer.insert(buffer.begin(), tmp.begin(), tmp.end());
        for (auto i : value2) {
            tmp = long_to_bytes(i);
            buffer.insert(buffer.begin(), tmp.begin(), tmp.end());
        }

        return buffer;
    }

    const body_buffer operator()(const uint32_t value1, const std::vector<float>& value2) {
        return this->get_bytes(value1, value2);
    }

    private:
    body_buffer buffer;
};

template <class T>
auto random_vec(const size_t max_vec_size) {
    auto ret = std::vector<T>(std::rand() / (RAND_MAX + 1u) / max_vec_size);
    for (auto i : ret) {
        i = static_cast<T>(std::rand());
    }
    return ret;
}

int main(void) {
#if TEST_MEMCPY
    std::cout << "Testing memcpy:";
#elif TEST_C_LOOP
    std::cout << "Testing C-style w/ loop:";
#elif TEST_C
    std::cout << "Testing fixed size C-style:";
#else
    std::cout << "Shits fucked, yo.";
#endif
    std::cout << std::endl;


    constexpr size_t NUM_TESTS    = 1000000;
    constexpr size_t MAX_VEC_SIZE = 10000;
    std::srand(std::time(nullptr));

    std::cout << "Fixed tests..." << std::endl;
    auto fixed_header       = FixedHeader();
    auto fixed_header_start = std::chrono::steady_clock::now();
    for (uint32_t i = 0; i < NUM_TESTS; ++i) {
        auto          tmp  = fixed_header(i, i + 1);
        volatile auto tmp2 = tmp;
    }
    auto fixed_header_time = std::chrono::steady_clock::now() - fixed_header_start;

    auto fixed_body       = FixedBody();
    auto fixed_body_start = std::chrono::steady_clock::now();
    for (uint32_t i = 0; i < NUM_TESTS; ++i) {
        auto          tmp  = fixed_body(i, random_vec<float>(MAX_VEC_SIZE));
        volatile auto tmp2 = tmp;
    }
    auto fixed_body_time = std::chrono::steady_clock::now() - fixed_body_start;

    std::cout << "Ephemeral tests..." << std::endl;
    auto eph_header_start = std::chrono::steady_clock::now();
    for (uint32_t i = 0; i < NUM_TESTS; ++i) {
        auto          tmp  = EphemeralHeader(i, i + 1).get_bytes();
        volatile auto tmp2 = tmp;
    }
    auto eph_header_time = std::chrono::steady_clock::now() - eph_header_start;

    auto eph_body_start = std::chrono::steady_clock::now();
    for (uint32_t i = 0; i < NUM_TESTS; ++i) {
        auto          tmp  = EphemeralBody(i, random_vec<float>(MAX_VEC_SIZE)).get_bytes();
        volatile auto tmp2 = tmp;
    }
    auto eph_body_time = std::chrono::steady_clock::now() - eph_body_start;

    std::cout << "Fixed header time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(fixed_header_time).count()
              << std::endl;
    std::cout << "Fixed body time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(fixed_body_time).count()
              << std::endl;
    std::cout << "Ephemeral header time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(eph_header_time).count()
              << std::endl;
    std::cout << "Ephemeral body time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(eph_body_time).count()
              << std::endl;

    return 0;
}