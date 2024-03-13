#ifndef CARBON_UTIL_HASH_HPP
#define CARBON_UTIL_HASH_HPP

#include <cstdint>

namespace carbon {
    constexpr uint32_t cx_hash(const char* input) {
        uint32_t hash = 0x811C9DC5;
        constexpr uint32_t prime = 0x01000193;

        while (*input) {
            hash ^= static_cast<uint32_t>(*input);
            hash *= prime;
            ++input;
        }

        return hash;
    }
}

#endif