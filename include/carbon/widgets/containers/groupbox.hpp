#ifndef CARBON_WIDGETS_CONTAINERS_GROUPBOX_HPP
#define CARBON_WIDGETS_CONTAINERS_GROUPBOX_HPP

#include "carbon/widgets/controls/separator.hpp"

namespace carbon {
	// TODO: Scrolling support
	class groupbox : public widget {
	public:
		groupbox(const std::wstring& name) : widget(), label_(name) {
			set_flex_direction(YGFlexDirectionColumn);
			set_flex_grow(1.0f);
			set_padding(YGEdgeHorizontal, theme.groupbox_padding.x);
			set_padding(YGEdgeVertical, theme.groupbox_padding.y);
			set_padding(YGEdgeTop, theme.groupbox_title_size + theme.groupbox_padding.y * 2.0f);

			set_min_width(150.0f);
			set_max_width(250.0f);

			set_gap(YGGutterRow, theme.groupbox_item_gap.x);
			set_gap(YGGutterColumn, theme.groupbox_item_gap.y);
		}

		void handle_draw() override {
			const auto layout = get_relative_layout();

			buf->draw_rect_filled(layout, theme.body);

			const glm::vec4 title(layout.x, layout.y, layout.z, theme.groupbox_title_size + theme.groupbox_padding.y);
			buf->draw_rect_rounded_filled(title, theme.groupbox_rounding, theme.title_bar, renderer::edge_top);
			buf->draw_text({title.x + theme.groupbox_padding.x, title.y + theme.groupbox_padding.y}, label_, segoe_font,
						   COLOR_WHITE, renderer::text_align_left, renderer::text_align_top);

			// Separator
			buf->draw_line({title.x, title.y + title.w}, {title.x + title.z, title.y + title.w}, theme.border, 1.0f);

			// Outline
			buf->draw_rect_rounded(layout, theme.groupbox_rounding, theme.border, 1.0f);

			// TODO: Resize grip
		}

	private:
		std::wstring label_;
	};
}

#endif