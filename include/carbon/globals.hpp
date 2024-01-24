#ifndef CARBON_GLOBAL_HPP
#define CARBON_GLOBAL_HPP

#define NOMINMAX
#include <renderer/buffer.hpp>

#include "theme.hpp"

namespace carbon {
	extern renderer::buffer* buf;
	extern renderer::text_font* main_font;

	extern renderer::performance_counter performance;

	extern style_sheet theme;

	struct benchmark_data {
		renderer::timer timer;
		size_t draw_calls{};
		size_t flex_compute_calls{};
	};

	extern benchmark_data benchmark;
}// namespace carbon

#endif