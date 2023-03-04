#ifndef CARBON_WIDGETS_CHECKBOX_HPP
#define CARBON_WIDGETS_CHECKBOX_HPP

#include "widget.hpp"
#include "label.hpp"

namespace carbon {
	class checkbox : public widget {
	public:
		checkbox() : widget() {
			set_flex_direction(YGFlexDirectionRow);

			label_ = add_child<label<std::string>>("Checkbox");
			label_->set_margin(YGEdgeLeft, theme.checkbox_size + theme.label_padding.x);

			set_width(label_->get_width().value + theme.checkbox_size + theme.label_padding.x);
			set_height(std::min(label_->get_height().value, theme.checkbox_size));
		}

		void handle_draw() override {
			const auto layout = get_relative_layout();
			const glm::vec4 button = { layout.x, layout.y, theme.checkbox_size, theme.checkbox_size };

			buf->draw_rect_rounded_filled(button, theme.checkbox_rounding, theme.body);
			buf->draw_rect_rounded(button, theme.checkbox_rounding, theme.border);
		}

	private:
		std::shared_ptr<label<std::string>> label_;
	};
}

#endif