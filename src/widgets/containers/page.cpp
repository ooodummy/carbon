#include "carbon/widgets/containers/page.hpp"

carbon::page::page(const std::u32string& name) : widget(), label_(name) {
	YGNodeStyleSetFlexDirection(node_, YGFlexDirectionRow);
	YGNodeStyleSetFlexWrap(node_, YGWrapWrap);
	YGNodeStyleSetFlexGrow(node_, 1.0f);

	YGNodeStyleSetPadding(node_, YGEdgeHorizontal, theme.page_padding.x);
	YGNodeStyleSetPadding(node_, YGEdgeVertical, theme.page_padding.y);

	YGNodeStyleSetGap(node_, YGGutterRow, theme.page_gap.x);
	YGNodeStyleSetGap(node_, YGGutterColumn, theme.page_gap.y);

	// Is setting the max percents needed?
	YGNodeStyleSetWidthPercent(node_, 1.0f);
	YGNodeStyleSetHeightPercent(node_, 1.0f);
}

void carbon::page::handle_draw() {
	// TODO: Scissor page bounds
	//  add scrolling for pages if the groupboxes break out vertically
	/*buf->draw_rect_filled(get_relative_layout(), COLOR_RED);

	bool broken = false;

	for (auto& child : get_children()) {

	}*/
}