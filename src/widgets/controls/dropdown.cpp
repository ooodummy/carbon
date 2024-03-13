#include "carbon/widgets/controls/dropdown.hpp"

carbon::dropdown::dropdown(const std::wstring& name, std::vector<std::wstring> items, int* selected) :
	widget(), name_(name), items_(items), selected_(selected) {
    const auto text_size = main_font->calc_text_size(name);
	set_min_width(text_size.x);
	set_height(text_size.y * 2.0f + theme.label_padding.y + theme.dropdown_padding.y * 2.0f);
}

void carbon::dropdown::handle_draw() {
	const auto layout = get_relative_layout();
	buf->draw_text(name_, {layout.x, layout.y}, COLOR_WHITE, main_font, renderer::align_top_left);

    const auto text_size = main_font->calc_text_size(name_);
	const glm::vec4 button(layout.x, layout.y + text_size.y + theme.label_padding.y, layout.z,
	                       text_size.y + theme.dropdown_padding.y * 2.0f);
	buf->draw_rect(button, theme.primary);
	buf->draw_text(std::format("{}", animation_time_), {button.x + theme.dropdown_padding.x, button.y + theme.dropdown_padding.y}, COLOR_WHITE, main_font, renderer::align_top_left);
}

void carbon::dropdown::handle_input() {
	if (!active_)
		return;

	const auto hovered = is_hovered();
	const auto animation_speed = performance.get_dt();
	hovered ? animation_time_ += animation_speed : animation_time_ -= animation_speed;
	animation_time_ = std::clamp(animation_time_, 0.0f, 1.0f);
}
