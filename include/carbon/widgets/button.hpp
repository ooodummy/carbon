#ifndef CARBON_WIDGETS_BUTTON_HPP
#define CARBON_WIDGETS_BUTTON_HPP

#include "label.hpp"

namespace carbon {
	class button : public widget {
	public:
		button() : widget() {
			set_padding(YGEdgeHorizontal, theme.button_padding.x);
			set_padding(YGEdgeVertical, theme.button_padding.y);
			set_flex_direction(YGFlexDirectionRow);
			//set_align_content(YGAlignCenter);

			label_ = add_child<label<std::string>>("Button");
			set_width(label_->get_width().value + theme.button_padding.x * 2.0f);
			set_height(label_->get_height().value + theme.button_padding.y * 2.0f);
		}

		void handle_draw() override {
			const auto layout = get_relative_layout();

			buf->draw_rect_rounded_filled(layout, theme.button_rounding, theme.border);
			buf->draw_rect_rounded(layout, theme.button_rounding, theme.primary);
		}

		void handle_input() override {
			//if (is_mouse_over(get_relative_layout()) && is_key_pressed(VK_LBUTTON))
			//	callback_();
		}

		void set_callback(const std::function<void()>& callback) {
			callback_ = callback;
		}

	private:
		std::shared_ptr<label<std::string>> label_;

		std::function<void()> callback_;
	};
}

#endif