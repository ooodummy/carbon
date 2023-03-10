#include "carbon/widgets/containers/window.hpp"

carbon::window::window(const std::wstring& name) : widget(YGConfigNew()), title_(name) {
	set_flex_direction(YGFlexDirectionColumn);

	set_width(500);
	set_height(300);

	//set_min_width(280);
	//set_min_height(165);

	title_bar_ = add_child<widget>();
	title_bar_->set_flex_direction(YGFlexDirectionRowReverse);
	title_bar_->set_align_items(YGAlignCenter);
	title_bar_->set_height(38);

	exit_button_ = title_bar_->add_child<widget>();
	exit_button_->set_width(45);
	exit_button_->set_height(38);

	// sub_tab_sheet_ = add_child<carbon::sub_tab_sheet>();
}

void carbon::window::handle_draw() {
	const auto layout = get_relative_layout();
	const auto title_layout = title_bar_->get_relative_layout();
	const auto exit_button_layout = exit_button_->get_relative_layout();

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
	// Dragging
	if (!dragging_title_bar_ && is_mouse_over(title_bar_->get_relative_layout()) &&
	    is_key_pressed(VK_LBUTTON)) {
		dragging_title_bar_ = true;
		drag_offset_ = get_mouse_pos() - glm::vec2(get_layout().x, get_layout().y);
	}
	else if (dragging_title_bar_ && !is_key_down(VK_LBUTTON))
		dragging_title_bar_ = false;

	if (dragging_title_bar_) {
		const auto mouse_pos = get_mouse_pos();
		set_position(YGEdgeLeft, mouse_pos.x - drag_offset_.x);
		set_position(YGEdgeTop, mouse_pos.y - drag_offset_.y);
	}

	const auto layout = get_relative_layout();

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
		set_width(std::max(mouse_pos.x - get_layout().x, get_min_width().value));
		set_height(std::max(mouse_pos.y - get_layout().y, get_min_height().value));
	}
}
