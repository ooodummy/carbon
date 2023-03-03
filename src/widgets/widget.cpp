#include "carbon/widgets/widget.hpp"
carbon::widget::widget() {
	layout_ = YGNodeNew();
}
carbon::widget::widget(YGConfigRef config) {
	layout_ = YGNodeNewWithConfig(config);
}
carbon::widget::~widget() {
	if (!YGNodeGetParent(layout_))
		YGNodeFreeRecursive(layout_);
}
void carbon::widget::draw() {
	handle_draw();

	for (auto& child : children_)
		child->draw();
}
void carbon::widget::input() {
	handle_input();

	for (auto& child : children_)
		child->input();
}
void carbon::widget::handle_draw() {}
void carbon::widget::handle_input() {}
void carbon::widget::set_parent(widget* parent) {
	parent_ = parent;
}
carbon::widget* carbon::widget::get_top_parent() const {
	if (!parent_)
		return nullptr;

	for (auto item = parent_;; item = item->get_parent()) {
		if (!item->get_parent())
			return item;
	}
}
carbon::widget* carbon::widget::get_parent() const {
	return parent_;
}
void carbon::widget::remove_child(const std::shared_ptr<widget>& child) {
	YGNodeRemoveChild(layout_, child->layout_);
	children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
}
void carbon::widget::calculate_layout(float available_width, float available_height, YGDirection direction) {
	YGNodeCalculateLayout(layout_, available_width, available_height, direction);
}
void carbon::widget::calculate_layout(glm::vec2 available_size, YGDirection direction) {
	calculate_layout(available_size.x, available_size.y, direction);
}
carbon::widget* carbon::widget::set_direction(YGDirection direction) {
	YGNodeStyleSetDirection(layout_, direction);
	return this;
}
YGDirection carbon::widget::get_direction() {
	return YGNodeStyleGetDirection(layout_);
}
carbon::widget* carbon::widget::set_flex_direction(YGFlexDirection flex_direction) {
	YGNodeStyleSetFlexDirection(layout_, flex_direction);
	return this;
}
YGFlexDirection carbon::widget::get_flex_direction() {
	return YGNodeStyleGetFlexDirection(layout_);
}
carbon::widget* carbon::widget::set_justify_content(YGJustify justify_content) {
	YGNodeStyleSetJustifyContent(layout_, justify_content);
	return this;
}
YGJustify carbon::widget::get_justify_content() {
	return YGNodeStyleGetJustifyContent(layout_);
}
carbon::widget* carbon::widget::set_align_content(YGAlign align_content) {
	YGNodeStyleSetAlignContent(layout_, align_content);
	return this;
}
YGAlign carbon::widget::get_align_content() {
	return YGNodeStyleGetAlignContent(layout_);
}
carbon::widget* carbon::widget::set_align_items(YGAlign align_items) {
	YGNodeStyleSetAlignItems(layout_, align_items);
	return this;
}
YGAlign carbon::widget::get_align_items() {
	return YGNodeStyleGetAlignItems(layout_);
}
carbon::widget* carbon::widget::set_align_self(YGAlign align_self) {
	YGNodeStyleSetAlignSelf(layout_, align_self);
	return this;
}
YGAlign carbon::widget::get_align_self() {
	return YGNodeStyleGetAlignSelf(layout_);
}
carbon::widget* carbon::widget::set_position_type(YGPositionType position_type) {
	YGNodeStyleSetPositionType(layout_, position_type);
	return this;
}
YGPositionType carbon::widget::get_position_type() {
	return YGNodeStyleGetPositionType(layout_);
}
carbon::widget* carbon::widget::set_flex_wrap(YGWrap flex_wrap) {
	YGNodeStyleSetFlexWrap(layout_, flex_wrap);
	return this;
}
YGWrap carbon::widget::get_flex_wrap() {
	return YGNodeStyleGetFlexWrap(layout_);
}
carbon::widget* carbon::widget::set_overflow(YGOverflow overflow) {
	YGNodeStyleSetOverflow(layout_, overflow);
	return this;
}
YGOverflow carbon::widget::get_overflow() {
	return YGNodeStyleGetOverflow(layout_);
}
carbon::widget* carbon::widget::set_display(YGDisplay display) {
	YGNodeStyleSetDisplay(layout_, display);
	return this;
}
YGDisplay carbon::widget::get_display() {
	return YGNodeStyleGetDisplay(layout_);
}
carbon::widget* carbon::widget::set_flex(float flex) {
	YGNodeStyleSetFlex(layout_, flex);
	return this;
}
float carbon::widget::get_flex() {
	return YGNodeStyleGetFlex(layout_);
}
carbon::widget* carbon::widget::set_flex_grow(float flex_grow) {
	YGNodeStyleSetFlexGrow(layout_, flex_grow);
	return this;
}
float carbon::widget::get_flex_grow() {
	return YGNodeStyleGetFlexGrow(layout_);
}
carbon::widget* carbon::widget::set_flex_shrink(float flex_shrink) {
	YGNodeStyleSetFlexShrink(layout_, flex_shrink);
	return this;
}
float carbon::widget::get_flex_shrink() {
	return YGNodeStyleGetFlexShrink(layout_);
}
carbon::widget* carbon::widget::set_flex_basis(float flex_basis) {
	YGNodeStyleSetFlexBasis(layout_, flex_basis);
	return this;
}
carbon::widget* carbon::widget::set_flex_basis_percent(float flex_basis_percent) {
	YGNodeStyleSetFlexBasisPercent(layout_, flex_basis_percent);
	return this;
}
carbon::widget* carbon::widget::set_flex_basis_auto() {
	YGNodeStyleSetFlexBasisAuto(layout_);
	return this;
}
YGValue carbon::widget::get_flex_basis() {
	return YGNodeStyleGetFlexBasis(layout_);
}
carbon::widget* carbon::widget::set_position(YGEdge edge, float position) {
	YGNodeStyleSetPosition(layout_, edge, position);
	return this;
}
carbon::widget* carbon::widget::set_position_percent(YGEdge edge, float position_percent) {
	YGNodeStyleSetPositionPercent(layout_, edge, position_percent);
	return this;
}
YGValue carbon::widget::get_position(YGEdge edge) {
	return YGNodeStyleGetPosition(layout_, edge);
}
carbon::widget* carbon::widget::set_margin(YGEdge edge, float margin) {
	YGNodeStyleSetMargin(layout_, edge, margin);
	return this;
}
carbon::widget* carbon::widget::set_margin_percent(YGEdge edge, float margin_percent) {
	YGNodeStyleSetMarginPercent(layout_, edge, margin_percent);
	return this;
}
carbon::widget* carbon::widget::set_margin_auto(YGEdge edge) {
	YGNodeStyleSetMarginAuto(layout_, edge);
	return this;
}
YGValue carbon::widget::get_margin(YGEdge edge) {
	return YGNodeStyleGetMargin(layout_, edge);
}
carbon::widget* carbon::widget::set_padding(YGEdge edge, float padding) {
	YGNodeStyleSetPadding(layout_, edge, padding);
	return this;
}
carbon::widget* carbon::widget::set_padding_percent(YGEdge edge, float padding_percent) {
	YGNodeStyleSetPaddingPercent(layout_, edge, padding_percent);
	return this;
}
YGValue carbon::widget::get_padding(YGEdge edge) {
	return YGNodeStyleGetPadding(layout_, edge);
}
carbon::widget* carbon::widget::set_border(YGEdge edge, float border) {
	YGNodeStyleSetBorder(layout_, edge, border);
	return this;
}
float carbon::widget::get_border(YGEdge edge) {
	return YGNodeStyleGetBorder(layout_, edge);
}
carbon::widget* carbon::widget::set_gap(YGGutter gutter, float gap_length) {
	YGNodeStyleSetGap(layout_, gutter, gap_length);
	return this;
}
float carbon::widget::get_gap(YGGutter gutter) {
	return YGNodeStyleGetGap(layout_, gutter);
}
carbon::widget* carbon::widget::set_width(float width) {
	YGNodeStyleSetWidth(layout_, width);
	return this;
}
carbon::widget* carbon::widget::set_width_percent(float width_percent) {
	YGNodeStyleSetWidthPercent(layout_, width_percent);
	return this;
}
carbon::widget* carbon::widget::set_width_auto() {
	YGNodeStyleSetWidthAuto(layout_);
	return this;
}
YGValue carbon::widget::get_width() {
	return YGNodeStyleGetWidth(layout_);
}
carbon::widget* carbon::widget::set_height(float height) {
	YGNodeStyleSetHeight(layout_, height);
	return this;
}
carbon::widget* carbon::widget::set_height_percent(float height_percent) {
	YGNodeStyleSetHeightPercent(layout_, height_percent);
	return this;
}
carbon::widget* carbon::widget::set_height_auto() {
	YGNodeStyleSetHeightAuto(layout_);
	return this;
}
YGValue carbon::widget::get_height() {
	return YGNodeStyleGetHeight(layout_);
}
carbon::widget* carbon::widget::set_min_width(float min_width) {
	YGNodeStyleSetMinWidth(layout_, min_width);
	return this;
}
carbon::widget* carbon::widget::set_min_width_percent(float min_width_percent) {
	YGNodeStyleSetMinWidthPercent(layout_, min_width_percent);
	return this;
}
YGValue carbon::widget::get_min_width() {
	return YGNodeStyleGetMinWidth(layout_);
}
carbon::widget* carbon::widget::set_min_height(float min_height) {
	YGNodeStyleSetMinHeight(layout_, min_height);
	return this;
}
carbon::widget* carbon::widget::set_min_height_percent(float min_height_percent) {
	YGNodeStyleSetMinHeightPercent(layout_, min_height_percent);
	return this;
}
YGValue carbon::widget::get_min_height() {
	return YGNodeStyleGetMinHeight(layout_);
}
carbon::widget* carbon::widget::set_max_width(float max_width) {
	YGNodeStyleSetMaxWidth(layout_, max_width);
	return this;
}
carbon::widget* carbon::widget::set_max_width_percent(float max_width_percent) {
	YGNodeStyleSetMaxWidthPercent(layout_, max_width_percent);
	return this;
}
YGValue carbon::widget::get_max_width() {
	return YGNodeStyleGetMaxWidth(layout_);
}
carbon::widget* carbon::widget::set_max_height(float max_height) {
	YGNodeStyleSetMaxHeight(layout_, max_height);
	return this;
}
carbon::widget* carbon::widget::set_max_height_percent(float max_height_percent) {
	YGNodeStyleSetMaxHeightPercent(layout_, max_height_percent);
	return this;
}
YGValue carbon::widget::get_max_height() {
	return YGNodeStyleGetMaxHeight(layout_);
}
carbon::widget* carbon::widget::set_aspect_ratio(float aspect_ratio) {
	YGNodeStyleSetAspectRatio(layout_, aspect_ratio);
	return this;
}
float carbon::widget::get_aspect_ratio() {
	return YGNodeStyleGetAspectRatio(layout_);
}
glm::vec4 carbon::widget::get_layout() {
	return {
	YGNodeLayoutGetLeft(layout_),
	YGNodeLayoutGetTop(layout_),
	YGNodeLayoutGetWidth(layout_),
	YGNodeLayoutGetHeight(layout_)
	};
}
YGDirection carbon::widget::get_layout_direction() {
	return YGNodeLayoutGetDirection(layout_);
}
bool carbon::widget::had_overflow() {
	return YGNodeLayoutGetHadOverflow(layout_);
}
glm::vec4 carbon::widget::get_layout_margin() {
	return {
	YGNodeLayoutGetMargin(layout_, YGEdgeLeft),
	YGNodeLayoutGetMargin(layout_, YGEdgeTop),
	YGNodeLayoutGetMargin(layout_, YGEdgeHorizontal),
	YGNodeLayoutGetMargin(layout_, YGEdgeVertical)
	};
}
glm::vec4 carbon::widget::get_layout_border() {
	return {
	YGNodeLayoutGetBorder(layout_, YGEdgeLeft),
	YGNodeLayoutGetBorder(layout_, YGEdgeTop),
	YGNodeLayoutGetBorder(layout_, YGEdgeHorizontal),
	YGNodeLayoutGetBorder(layout_, YGEdgeVertical)
	};
}
glm::vec4 carbon::widget::get_layout_padding() {
	return {
	YGNodeLayoutGetPadding(layout_, YGEdgeLeft),
	YGNodeLayoutGetPadding(layout_, YGEdgeTop),
	YGNodeLayoutGetPadding(layout_, YGEdgeHorizontal),
	YGNodeLayoutGetPadding(layout_, YGEdgeVertical)
	};
}
glm::vec4 carbon::widget::get_relative_layout() {
	auto layout = get_layout();

	if (parent_) {
		auto parent_layout =  parent_->get_relative_layout();
		layout.x += parent_layout.x;
		layout.y += parent_layout.y;
	}

	return layout;
}