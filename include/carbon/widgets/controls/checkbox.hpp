#ifndef CARBON_WIDGETS_CONTROLS_CHECKBOX_HPP
#define CARBON_WIDGETS_CONTROLS_CHECKBOX_HPP

#include "carbon/widgets/widget.hpp"
#include "label.hpp"

namespace carbon {
	class checkbox : public widget {
	public:
		checkbox(const std::wstring& name, bool* value) : widget(), value_(value), label_(name) {
			set_flex_direction(YGFlexDirectionRow);

			const auto text_size = dx11->get_text_size(name, segoe_font);
			set_width(text_size.x + theme.checkbox_size + theme.label_padding.x);
			set_height(std::max(text_size.y, theme.checkbox_size));
		}

		bool is_hovered() const override {
			return is_mouse_over(get_relative_layout());
		}

		void handle_draw() override {
			const auto layout = get_relative_layout();
			const glm::vec4 button = { layout.x, layout.y, theme.checkbox_size, theme.checkbox_size };

			buf->draw_rect_rounded_filled(button, theme.checkbox_rounding, theme.body);
			buf->draw_rect_rounded(button, theme.checkbox_rounding, theme.border);

			if (*value_) {
				// Checkmark
				buf->draw_line({ button.x + button.z * 0.2f, button.y + button.w * 0.5f },
							   { button.x + button.z * 0.45f, button.y + button.w * 0.8f }, theme.primary, 2.0f);
				buf->draw_line({ button.x + button.z * 0.45f, button.y + button.w * 0.8f },
							   { button.x + button.z * 0.8f, button.y + button.w * 0.2f }, theme.primary, 2.0f);

				//buf->draw_rect_rounded_filled({button.x + 2.0f, button.y + 2.0f, button.z - 4.0f, button.w - 4.0f},
				//							  std::max(theme.checkbox_rounding - 1.0f, 0.0f), theme.primary);
			}

			buf->draw_text({layout.x + theme.checkbox_size + theme.label_padding.x, layout.y}, label_, segoe_font,
						   COLOR_WHITE, renderer::text_align_left, renderer::text_align_top);
		}

		void handle_input() override {
			if (!active_)
				return;

			if (is_hovered() && is_key_pressed(VK_LBUTTON))
				*value_ = !*value_;
		}

	private:
		bool* value_;

		std::wstring label_;
	};
}

#endif