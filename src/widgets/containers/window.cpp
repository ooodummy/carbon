#include "carbon/widgets/containers/window.hpp"

carbon::window::window(const std::wstring& name) : widget(YGConfigNew()), title_(name) {
	YGNodeStyleSetFlexDirection(node_, YGFlexDirectionColumn);

	YGNodeStyleSetWidth(node_, 500.0f);
	YGNodeStyleSetHeight(node_, 300.0f);

	YGNodeStyleSetMinWidth(node_, 200.0f);
	YGNodeStyleSetMinHeight(node_, 100.0f);

	title_bar_ = add_child<widget>();
	YGNodeStyleSetFlexDirection(title_bar_->get_node(), YGFlexDirectionRowReverse);
	YGNodeStyleSetAlignItems(title_bar_->get_node(), YGAlignCenter);
	YGNodeStyleSetHeight(title_bar_->get_node(), 38.0f);

	exit_button_ = title_bar_->add_child<widget>();
	YGNodeStyleSetWidth(exit_button_->get_node(), 45.0f);
	YGNodeStyleSetWidth(exit_button_->get_node(), 38.0f);

	// sub_tab_sheet_ = add_child<carbon::sub_tab_sheet>();
}

void carbon::window::handle_draw() {
	const auto layout = get_absolute_layout();
	const auto title_layout = title_bar_->get_absolute_layout();
	const auto exit_button_layout = exit_button_->get_absolute_layout();

	//buf->draw_rect_rounded(layout + glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), theme.window_rounding, theme.primary);
	//buf->draw_rect_rounded_filled(layout, theme.window_rounding, theme.body);
	//buf->draw_rect_rounded_filled(title_layout, theme.window_rounding, theme.title_bar, renderer::edge_top);
	//buf->draw_rect_rounded_filled(exit_button_layout, theme.window_rounding, theme.border.alpha(45), renderer::edge_top_right);

	buf->draw_rect(layout + glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), theme.primary);
	buf->draw_rect_filled(layout, theme.body);

	// TODO: Shadow
	buf->draw_rect_filled(title_layout, theme.title_bar);
	buf->draw_rect_filled(exit_button_layout, theme.border.alpha(45));

	buf->draw_text({ title_layout.x + 10.0f, title_layout.y + title_layout.w / 2.0f }, title_, segoe_ui, COLOR_WHITE,
				   renderer::align_center_left);
}

void carbon::window::handle_input() {
	const auto layout = get_absolute_layout();

	// Dragging
	if (!dragging_title_bar_ && is_mouse_over(title_bar_->get_absolute_layout()) &&
	    is_key_pressed(VK_LBUTTON)) {
		dragging_title_bar_ = true;
		drag_offset_ = get_mouse_pos() - glm::vec2(layout.x, layout.y);
	}
	else if (dragging_title_bar_ && !is_key_down(VK_LBUTTON))
		dragging_title_bar_ = false;

	if (dragging_title_bar_) {
		const auto mouse_pos = get_mouse_pos();
		YGNodeStyleSetPosition(node_, YGEdgeLeft, mouse_pos.x - drag_offset_.x);
		YGNodeStyleSetPosition(node_, YGEdgeTop, mouse_pos.y - drag_offset_.y);
	}

	// Resizing
	const bool hovered_corner =
	is_mouse_over({ layout.x + layout.z - theme.page_padding.x, layout.y + layout.w - theme.page_padding.y,
					theme.page_padding.x * 2.0f, theme.page_padding.y * 2.0f });

	if (!resizing_ && hovered_corner && is_key_pressed(VK_LBUTTON))
		resizing_ = true;
	else if (resizing_ && !is_key_down(VK_LBUTTON))
		resizing_ = false;

	if (resizing_) {
		const auto mouse_pos = get_mouse_pos();
		YGNodeStyleSetWidth(node_, mouse_pos.x - layout.x);
		YGNodeStyleSetHeight(node_, mouse_pos.y - layout.y);
	}
}
