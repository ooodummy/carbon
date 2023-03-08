#include "carbon/widgets/controls/dropdown.hpp"

carbon::dropdown::dropdown(const std::wstring& name, std::vector<std::wstring> items, int* selected) :
	widget(), name_(name), items_(items), selected_(selected) {
	const auto text_size = dx11->get_text_size(name, segoe_ui);
	set_min_width(text_size.x);
	set_height(text_size.y * 2.0f + theme.label_padding.y + theme.dropdown_padding.y * 2.0f);
}

void carbon::dropdown::handle_draw() {
	const auto layout = get_relative_layout();
	buf->draw_text({layout.x, layout.y}, name_, segoe_ui, COLOR_WHITE, renderer::text_align_left,
	               renderer::text_align_top);

	const auto text_size = dx11->get_text_size(name_, segoe_ui);
	const glm::vec4 button(layout.x, layout.y + text_size.y + theme.label_padding.y, layout.z,
	                       text_size.y + theme.dropdown_padding.y * 2.0f);
	buf->draw_rect(button, theme.primary);
	buf->draw_text({button.x + theme.dropdown_padding.x, button.y + theme.dropdown_padding.y},
	               fmt::format("{}", animation_time_), segoe_ui, COLOR_WHITE, renderer::text_align_left,
	               renderer::text_align_top);
}

void carbon::dropdown::handle_input() {
	if (!active_)
		return;

	const auto hovered = is_hovered();
	const auto animation_speed = timer.get_dt();
	hovered ? animation_time_ += animation_speed : animation_time_ -= animation_speed;
	animation_time_ = std::clamp(animation_time_, 0.0f, 1.0f);
}
