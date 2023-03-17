#ifndef CARBON_GLOBAL_HPP
#define CARBON_GLOBAL_HPP

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "renderer/buffer.hpp"
#include "renderer/renderer.hpp"
#include "theme.hpp"

namespace carbon {
	extern std::shared_ptr<renderer::win32_window> application;
	extern std::unique_ptr<renderer::d3d11_renderer> dx11;
	extern size_t segoe_ui;
	extern size_t fa_regular;

	extern renderer::buffer* buf;

	// Used for FPS information and animations using delta time
	extern renderer::performance_counter timer;

	extern style_sheet theme;
}// namespace carbon

#endif