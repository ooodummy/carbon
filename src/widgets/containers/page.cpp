#include "carbon/widgets/containers/page.hpp"

carbon::page::page(const std::u32string& name) : widget(), label_(name) {
	YGNodeStyleSetFlexDirection(node_, YGFlexDirectionRow);
	YGNodeStyleSetFlexWrap(node_, YGWrapWrap);
	YGNodeStyleSetFlexGrow(node_, 1.0f);

	YGNodeStyleSetPadding(node_, YGEdgeHorizontal, theme.page_padding.x);
	YGNodeStyleSetPadding(node_, YGEdgeVertical, theme.page_padding.y);

	YGNodeStyleSetGap(node_, YGGutterRow, theme.page_gap.x);
	YGNodeStyleSetGap(node_, YGGutterColumn, theme.page_gap.y);
}

void carbon::page::draw() {
	if (YGNodeStyleGetDisplay(node_) == YGDisplayNone)
		return;

	// TODO: Scissor page bounds
	//  add scrolling for pages if the groupboxes break out vertically
	const auto layout = get_absolute_layout();
	//buf->draw_rect_filled(layout, COLOR_RED.alpha(150));
	buf->push_scissor(layout);

	for (auto& child : get_children()) {
		child->draw();
	}

	buf->pop_scissor();
}

std::shared_ptr<carbon::groupbox> carbon::page::add_groupbox(const std::wstring& name) {
	return add_child<groupbox>(name);
}
