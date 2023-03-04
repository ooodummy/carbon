#ifndef CARBON_WIDGETS_SEPARATOR_HPP
#define CARBON_WIDGETS_SEPARATOR_HPP

#include "widget.hpp"

namespace carbon {
	class separator : public widget {
	public:
		separator() : widget() {
			set_flex_basis(1.0f);
			set_margin(YGEdgeTop, theme.separator_spacing);
			set_margin(YGEdgeBottom, theme.separator_spacing);
		}

		void handle_draw() override {
			const auto layout = get_relative_layout();

			buf->draw_line({layout.x, layout.y}, {layout.x + layout.z, layout.y + layout.w}, theme.border);
		}
	};
}

#endif