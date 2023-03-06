#include "carbon/widgets/controls/button.hpp"

carbon::button::button(const std::wstring& name, const std::function<void()>& callback) : widget(), label_(name),
	callback_(callback) {
	set_flex_direction(YGFlexDirectionRow);
	set_padding(YGEdgeHorizontal, theme.button_padding.x);
	set_padding(YGEdgeVertical, theme.button_padding.y);

	const auto text_size = dx11->get_text_size(name, segoe_ui);
	set_width(text_size.x + theme.button_padding.x * 2.0f);
	set_height(text_size.y + theme.button_padding.y * 2.0f);
}

bool carbon::button::is_hovered() const {
	return is_mouse_over(get_relative_layout());
}

void carbon::button::handle_draw() {
	const auto layout = get_relative_layout();

	buf->draw_rect_rounded_filled(layout, theme.button_rounding, theme.border);
	buf->draw_rect_rounded(layout, theme.button_rounding, theme.primary);

	buf->draw_text({ layout.x + layout.z / 2.0f, layout.y + layout.w / 2.0f }, label_, segoe_ui, COLOR_WHITE,
	               renderer::text_align_center, renderer::text_align_center);
}

void carbon::button::handle_input() {
	if (!active_)
		return;

	if (is_hovered() && is_key_pressed(VK_LBUTTON))
		callback_();
}

void carbon::button::set_callback(const std::function<void()>& callback) {
	callback_ = callback;
}
