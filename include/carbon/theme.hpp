#ifndef CARBON_THEME_HPP
#define CARBON_THEME_HPP

#include "renderer/color.hpp"

namespace carbon {
	struct style_sheet {
		renderer::color_rgba body;
		renderer::color_rgba primary;
		renderer::color_rgba title_bar;
		renderer::color_rgba border;

		float window_rounding;

		glm::vec2 page_padding;
		glm::vec2 page_gap;

		float groupbox_rounding;
		glm::vec2 groupbox_padding;
		glm::vec2 groupbox_title_margin;
		glm::vec2 groupbox_item_gap;

		glm::vec2 label_padding;

		float button_rounding;
		glm::vec2 button_padding;

		float separator_spacing;

		float checkbox_rounding;
		float checkbox_size;
	};

	void init_default_theme();
}// namespace carbon

#endif