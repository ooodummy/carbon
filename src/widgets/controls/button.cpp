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

	buf->draw_rect_rounded_filled(layout, theme.button_rounding, theme.body);
	buf->draw_rect_rounded_filled(layout, theme.button_rounding, theme.primary.alpha(animation_time_ * 255.0f));
	buf->draw_rect_rounded(layout, theme.button_rounding, theme.primary);

	buf->draw_text({ layout.x + layout.z / 2.0f, layout.y + layout.w / 2.0f }, label_, segoe_ui, COLOR_WHITE,
	               renderer::text_align_center, renderer::text_align_center);
}

void carbon::button::handle_input() {
	if (!active_)
		return;

	bool hovered = is_hovered();

	if (hovered)
		animation_time_ = animation_time_ + timer.get_dt() / 0.5f;
	else
		animation_time_ = animation_time_ - timer.get_dt() / 0.5f;
	animation_time_ = std::clamp(animation_time_, 0.0f, 0.2f);

	if (!held_ && hovered && is_key_pressed(VK_LBUTTON)) {
		held_ = true;
	}
	else if (held_ && is_key_released(VK_LBUTTON)) {
		if (hovered) {
			callback_();
		}
		held_ = false;
	}

	if (held_)
		animation_time_ = 0.4f;
}
