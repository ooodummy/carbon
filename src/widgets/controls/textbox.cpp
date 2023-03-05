#include "carbon/widgets/controls/textbox.hpp"

carbon::textbox::textbox(const std::wstring& name, const std::wstring& placeholder, size_t max_length,
						 std::string* value, bool hidden) :
	widget(), name_(name), placeholder_(placeholder), max_length_(max_length), value_(value), hidden_(hidden) {
	const auto text_size = dx11->get_text_size(name, segoe_font);
	set_min_width(text_size.x);
	set_height(text_size.y * 2.0f + theme.label_padding.y + theme.textbox_padding.y * 2.0f);
}

void carbon::textbox::handle_draw() {
	const auto layout = get_relative_layout();
	buf->draw_text({ layout.x, layout.y }, name_, segoe_font, COLOR_WHITE, renderer::text_align_left,
	               renderer::text_align_top);

	const auto text_size = dx11->get_text_size(name_, segoe_font);
	const glm::vec4 textbox(layout.x, layout.y + text_size.y + theme.label_padding.y, layout.z,
	                        layout.w - text_size.y - theme.label_padding.y);

	buf->draw_rect_filled(textbox, theme.body);
	buf->draw_rect(textbox, theme.border);

	std::wstring text;
	if (value_->length() == 0) {
		text = placeholder_;
	}
	else {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		text = converter.from_bytes(value_->c_str());

		if (hidden_) {
			text = std::wstring(text.length(), L'*');
		}
	}

	buf->draw_text({ textbox.x + theme.label_padding.x, textbox.y + theme.label_padding.y }, text, segoe_font,
	               COLOR_WHITE, renderer::text_align_left, renderer::text_align_top);
}
