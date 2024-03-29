#include "carbon/widgets/controls/checkbox.hpp"

carbon::checkbox::checkbox(const std::wstring& name, bool* value) : widget(), value_(value), label_(name) {
	YGNodeStyleSetFlexDirection(node_, YGFlexDirectionRow);

	const auto text_size = dx11->get_text_size(name, segoe_ui);
	YGNodeStyleSetWidth(node_, text_size.x + theme.checkbox_size + theme.label_padding.x);
	YGNodeStyleSetHeight(node_, std::max(text_size.y, theme.checkbox_size));
}

void carbon::checkbox::handle_draw() {
	const auto layout = get_absolute_layout();
	const glm::vec4 button = { layout.x, layout.y, theme.checkbox_size, theme.checkbox_size };

	//buf->draw_rect_rounded_filled(button, theme.checkbox_rounding, theme.body);
	//buf->draw_rect_rounded_filled(button, theme.checkbox_rounding,  theme.primary.alpha(static_cast<uint8_t>(animation_time_ * 255.0f)));
	//buf->draw_rect_rounded(button, theme.checkbox_rounding, theme.border);

	buf->draw_rect_filled(button, theme.body);
	buf->draw_rect_filled(button, theme.primary.alpha(animation_time_ * 255.0f));
	buf->draw_rect(button, theme.border);

	if (*value_) {
		// Checkmark
		buf->draw_line({ button.x + button.z * 0.2f, button.y + button.w * 0.5f },
		               { button.x + button.z * 0.45f, button.y + button.w * 0.8f }, theme.primary, 2.0f);
		buf->draw_line({ button.x + button.z * 0.45f, button.y + button.w * 0.8f },
		               { button.x + button.z * 0.8f, button.y + button.w * 0.2f }, theme.primary, 2.0f);

		// buf->draw_rect_rounded_filled({button.x + 2.0f, button.y + 2.0f, button.z - 4.0f, button.w - 4.0f},
		//							  std::max(theme.checkbox_rounding - 1.0f, 0.0f), theme.primary);
	}

	buf->draw_text({ layout.x + theme.checkbox_size + theme.label_padding.x, layout.y }, label_, segoe_ui,
	               COLOR_WHITE, renderer::align_top_left);
}

void carbon::checkbox::handle_input() {
	if (!active_)
		return;

	const auto hovered = is_hovered();

	const auto animation_speed = timer.get_dt() / 0.5f;
	hovered || held_ ? animation_time_ += animation_speed : animation_time_ -= animation_speed;
	animation_time_ = std::clamp(animation_time_, 0.0f, held_ ? 0.3f : 0.1f);

	if (!held_ && hovered && is_key_pressed(VK_LBUTTON)) {
		held_ = true;
	}
	else if (held_ && is_key_released(VK_LBUTTON)) {
		if (hovered) {
			*value_ = !*value_;
		}
		held_ = false;
	}
}