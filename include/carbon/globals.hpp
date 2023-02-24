#ifndef CARBON_GLOBAL_HPP
#define CARBON_GLOBAL_HPP

#define NOMINMAX
#include "renderer/renderer.hpp"
#include "renderer/buffer.hpp"

#include "theme.hpp"

namespace carbon {
	extern std::shared_ptr<renderer::win32_window> application;
	extern std::unique_ptr<renderer::d3d11_renderer> dx11;
	extern size_t segoe_font;

	extern renderer::buffer* buf;
	extern renderer::performance_counter performance;

	extern style_sheet theme;

	struct benchmark_data {
		renderer::timer timer;
		size_t draw_calls;
		size_t flex_compute_calls;
	};

	extern benchmark_data benchmark;
}// namespace carbon

#endif