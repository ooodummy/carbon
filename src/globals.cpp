#include "carbon/globals.hpp"

namespace carbon {
	std::shared_ptr<renderer::win32_window> application;
	std::unique_ptr<renderer::d3d11_renderer> dx11;
	size_t segoe_ui;
	size_t fa_regular;

	renderer::buffer* buf;
	renderer::performance_counter timer;

	style_sheet theme;
}// namespace carbon