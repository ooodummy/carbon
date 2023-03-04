#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdio>
#include <random>

namespace util {
	constexpr uint32_t cx_hash(const char* input) {
		uint32_t hash = 0xcbf29ce484222325;
		constexpr uint32_t prime = 0x00000100000001b3;

		while (*input) {
			hash ^= static_cast<uint32_t>(*input);
			hash *= prime;
			++input;
		}

		return hash;
	}

	template <typename T, size_t N>
	std::vector<T> create_filled_vector(const T& fill) {
		auto vec = std::vector<T>(N);
		std::fill(vec.begin(), vec.begin() + N, fill);
		return vec;
	}
}

#endif