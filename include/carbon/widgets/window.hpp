#ifdef CARBON_WIDGETS_WINDOW_HPP
#define CARBON_WIDGETS_WINDOW_HPP

#include "widget.hpp"

namespace carbon {
	class window : public widget {
		window() : widget() {
			set_width(300);
			set_height(250);
			set_direction(YGDirectionColumn);
		}

		void draw() override {
			const auto layout = get_layout();

			buf->draw_rect_filled(layout, theme.body);
			buf->draw_rect(layout, theme.border);
		}
	};
}

#endif