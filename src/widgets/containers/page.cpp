#include "carbon/widgets/containers/page.hpp"

carbon::page::page(const std::u32string& name) : widget(), label_(name) {
	YGNodeStyleSetFlexDirection(node_, YGFlexDirectionRow);
	YGNodeStyleSetFlexWrap(node_, YGWrapWrap);
	YGNodeStyleSetFlexGrow(node_, 1.0f);

	YGNodeStyleSetPadding(node_, YGEdgeHorizontal, theme.page_padding.x);
	YGNodeStyleSetPadding(node_, YGEdgeVertical, theme.page_padding.y);

	//YGNodeStyleSetWidthPercent(node_, 1.0f);
	//YGNodeStyleSetHeightPercent(node_, 1.0f);

	YGNodeStyleSetGap(node_, YGGutterRow, theme.page_gap.x);
	YGNodeStyleSetGap(node_, YGGutterColumn, theme.page_gap.y);

	/*inner_container_node_ = add_child<widget>();
	YGNodeStyleSetFlexDirection(inner_container_node_->get_node(), YGFlexDirectionRow);
	YGNodeStyleSetFlexWrap(inner_container_node_->get_node(), YGWrapWrap);
	YGNodeStyleSetFlexGrow(inner_container_node_->get_node(), 1.0f);

	YGNodeStyleSetGap(inner_container_node_->get_node(), YGGutterRow, theme.page_gap.x);
	YGNodeStyleSetGap(inner_container_node_->get_node(), YGGutterColumn, theme.page_gap.y);*/

	//YGNodeStyleSetWidthPercent(inner_container_node_->get_node(), 1.0f);
	//YGNodeStyleSetHeightPercent(inner_container_node_->get_node(), 1.0f);
}

void carbon::page::draw() {
	if (YGNodeStyleGetDisplay(node_) == YGDisplayNone)
		return;

	// TODO: Scissor page bounds
	//  add scrolling for pages if the groupboxes break out vertically
	const auto layout = get_absolute_layout();
	buf->draw_rect_filled(layout, COLOR_RED.alpha(150));
	//buf->push_scissor(layout);

	for (auto& child : get_children()) {
		child->draw();
	}

	//buf->pop_scissor();
}

std::shared_ptr<carbon::groupbox> carbon::page::add_groupbox(const std::wstring& name) {
	return add_child<groupbox>(name);
}
