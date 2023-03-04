#ifndef CARBON_WIDGETS_GROUPBOX_HPP
#define CARBON_WIDGETS_GROUPBOX_HPP

#include "label.hpp"
#include "separator.hpp"

namespace carbon {
	class groupbox : public widget {
	public:
		groupbox() : widget() {
			set_flex_direction(YGFlexDirectionColumn);
			set_flex_grow(1.0f);
			set_padding(YGEdgeHorizontal, theme.groupbox_padding.x);
			set_padding(YGEdgeVertical, theme.groupbox_padding.y);

			label_ = add_child<label<std::string>>("Groupbox");
			label_->set_margin(YGEdgeHorizontal, theme.groupbox_title_margin.x);
			label_->set_margin(YGEdgeVertical, theme.groupbox_title_margin.y);

			add_child<separator>();

			set_gap(YGGutterRow, theme.groupbox_item_gap.x);
			set_gap(YGGutterColumn, theme.groupbox_item_gap.y);
		}

		void handle_draw() override {
			const auto layout = get_relative_layout();

			buf->draw_rect_filled(layout, theme.body);
			buf->draw_rect_rounded(layout, theme.groupbox_rounding, theme.border, 1.0f);
			//buf->draw_rect(layout, theme.border);
			//auto test = label_->get_layout_margin().y;
			//buf->draw_line({layout.x, layout.y + test}, {layout.x + layout.z, layout.y + test}, theme.primary);
		}

	private:
		std::shared_ptr<label<std::string>> label_;
	};
}

#endif