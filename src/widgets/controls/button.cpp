#include "carbon/widgets/controls/button.hpp"

carbon::button::button(const std::wstring& name, const std::function<void()>& callback) : widget(), label_(name),
	callback_(callback) {
	set_flex_direction(YGFlexDirectionRow);
	set_padding(YGEdgeHorizontal, theme.button_padding.x);
	set_padding(YGEdgeVertical, theme.button_padding.y);

    const auto text_size = main_font->calc_text_size(name);
	set_width(text_size.x + theme.button_padding.x * 2.0f);
	set_height(text_size.y + theme.button_padding.y * 2.0f);
}

void carbon::button::handle_draw() {
	const auto layout = get_relative_layout();

	//buf->draw_rect_rounded_filled(layout, theme.button_rounding, theme.body);
	//buf->draw_rect_rounded_filled(layout, theme.button_rounding, theme.primary.alpha(static_cast<uint8_t>(animation_time_ * 255.0f)));
	//buf->draw_rect_rounded(layout, theme.button_rounding, theme.primary);

	buf->draw_rect_filled(layout, theme.body);
	buf->draw_rect_filled(layout, theme.primary.alpha(animation_time_ * 255.0f));
	buf->draw_rect(layout, theme.primary);

	buf->draw_text(label_, { layout.x + layout.z / 2.0f, layout.y + layout.w / 2.0f }, COLOR_WHITE, main_font, renderer::align_center);
}

void carbon::button::handle_input() {
	if (!active_)
		return;

	bool hovered = is_hovered();

	const auto animation_speed = performance.get_dt() / 0.5f;
	hovered || held_ ? animation_time_ += animation_speed : animation_time_ -= animation_speed;
	animation_time_ = std::clamp(animation_time_, 0.0f, held_ ? 0.4f : 0.2f);

	if (!held_ && hovered && is_key_pressed(VK_LBUTTON)) {
		held_ = true;
	}
	else if (held_ && is_key_released(VK_LBUTTON)) {
		if (hovered) {
			callback_();
		}
		held_ = false;
	}
}
