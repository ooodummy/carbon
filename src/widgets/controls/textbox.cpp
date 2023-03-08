#include "carbon/widgets/controls/textbox.hpp"

carbon::textbox::textbox(const std::wstring& name, const std::wstring& placeholder, size_t max_length,
						 std::string* value, bool hidden) :
	widget(), name_(name), placeholder_(placeholder), max_length_(max_length), value_(value), hidden_(hidden) {
	const auto text_size = dx11->get_text_size(name, segoe_ui);
	set_min_width(text_size.x);
	set_height(text_size.y * 2.0f + theme.label_padding.y + theme.textbox_padding.y * 2.0f);
}

void carbon::textbox::handle_draw() {
	const auto layout = get_relative_layout();
	buf->draw_text({ layout.x, layout.y }, name_, segoe_ui, COLOR_WHITE, renderer::text_align_left,
	               renderer::text_align_top);

	const auto text_size = dx11->get_text_size(name_, segoe_ui);
	const glm::vec4 textbox(layout.x, layout.y + text_size.y + theme.label_padding.y, layout.z,
	                        layout.w - text_size.y - theme.label_padding.y);

	buf->draw_rect_filled(textbox, theme.body);
	buf->draw_rect_filled(textbox, theme.primary.alpha(static_cast<uint8_t>(animation_time_ * 255.0f)));
	buf->draw_rect(textbox, theme.border);

	// TODO: Blinking cursor when active, and blinking cursor when inactive. Should we get the blink rate from registry?

	bool use_placeholder = value_->length() == 0;
	std::wstring text;
	if (use_placeholder) {
		text = placeholder_;
	}
	else {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		text = converter.from_bytes(value_->c_str());

		if (hidden_) {
			text = std::wstring(text.length(), L'*');
		}
	}

	buf->draw_text({ textbox.x + theme.textbox_padding.x, textbox.y + theme.textbox_padding.y }, text, segoe_ui,
				   use_placeholder ? renderer::color_rgba(210, 210, 210, 255) : COLOR_WHITE, renderer::text_align_left, renderer::text_align_top);
}

void carbon::textbox::handle_input() {
	const auto hovered = is_hovered();

	const auto animation_speed = timer.get_dt() / 0.5f;
	hovered || held_ ? animation_time_ += animation_speed : animation_time_ -= animation_speed;
	animation_time_ = std::clamp(animation_time_, 0.0f, held_ ? 0.2f : (selected_ ? 0.3f : 0.1f));

	if (!held_ && hovered && is_key_pressed(VK_LBUTTON)) {
		held_ = true;
	}
	else if (held_ && is_key_released(VK_LBUTTON)) {
		if (hovered) {
			selected_ = true;
		}
		held_ = false;
	}

	if (!selected_)
		return;

	const auto last_key = get_last_key_down();

	if (last_key == 0)
		return;

	switch (last_key) {
	case VK_BACK:
		if (value_->length() > 0) {
			value_->pop_back();
		}
		break;
	case VK_RETURN:
		selected_ = false;
		break;
	default:
		if (last_key < 32 || last_key > 126)
			break;
		if (value_->length() < max_length_) {
			value_->push_back(static_cast<char>(last_key));
		}
		break;
	}
}