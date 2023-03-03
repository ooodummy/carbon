#ifndef CARBON_WIDGETS_TAB_SHEET_HPP
#define CARBON_WIDGETS_TAB_SHEET_HPP

#include "widget.hpp"

#include "page.hpp"

namespace carbon {
	// TODO: Inherit from this to style sub tab sheet
	class tab_sheet : public widget {
	public:
		tab_sheet() : widget() {
			set_width(112.0f);
			set_flex_direction(YGFlexDirectionColumn);
		}

		void handle_draw() override {
			const auto layout = get_relative_layout();

			buf->draw_rect_filled(layout, theme.border);
		}
	};
}

#endif