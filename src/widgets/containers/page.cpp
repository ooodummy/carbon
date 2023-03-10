#include "carbon/widgets/containers/page.hpp"

carbon::page::page(const std::u32string& name) : widget(), label_(name) {
	set_flex_direction(YGFlexDirectionRow);
	set_flex_wrap(YGWrapWrap);
	set_flex_grow(1.0f);

	set_padding(YGEdgeHorizontal, theme.page_padding.x);
	set_padding(YGEdgeVertical, theme.page_padding.y);

	set_gap(YGGutterRow, theme.page_gap.x);
	set_gap(YGGutterColumn, theme.page_gap.y);

	set_width_percent(1.0f);
	set_height_percent(1.0f);
}

void carbon::page::handle_draw() {
	// TODO: Scissor page bounds
	//  add scrolling for pages if the groupboxes break out vertically
	/*buf->draw_rect_filled(get_relative_layout(), COLOR_RED);

	bool broken = false;

	for (auto& child : get_children()) {

	}*/
}