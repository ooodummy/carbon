#ifndef CARBON_WIDGETS_CONTAINERS_GROUPBOX_HPP
#define CARBON_WIDGETS_CONTAINERS_GROUPBOX_HPP

#include "carbon/widgets/controls/label.hpp"
#include "carbon/widgets/controls/separator.hpp"

namespace carbon {
	class groupbox : public widget {
	public:
		groupbox(const std::wstring& name) : widget() {
			set_flex_direction(YGFlexDirectionColumn);
			set_flex_grow(1.0f);
			set_padding(YGEdgeHorizontal, theme.groupbox_padding.x);
			set_padding(YGEdgeVertical, theme.groupbox_padding.y);

			set_min_width(150.0f);
			set_max_width(250.0f);

			label_ = add_child<label<std::wstring>>(name);
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
		std::shared_ptr<label<std::wstring>> label_;
	};
}

#endif