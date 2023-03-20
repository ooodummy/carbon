#ifndef CARBON_WIDGETS_CONTROLS_LABEL_HPP
#define CARBON_WIDGETS_CONTROLS_LABEL_HPP

#include "carbon/widgets/widget.hpp"

namespace carbon {
	template<typename T>
	class label : public widget {
	public:
		label() : widget() {}

		explicit label(const T& text) : widget() {
			set_text(text);
		}

		void set_text(const T& text) {
			text_ = text;

			const auto text_size = dx11->get_text_size(text_, segoe_ui);
			YGNodeStyleSetWidth(node_, text_size.x);
			YGNodeStyleSetHeight(node_, text_size.y);
		}

		void handle_draw() override {
			const auto layout = get_absolute_layout();

			const auto text_size = dx11->get_text_size(text_, segoe_ui);

			buf->draw_text({ layout.x, layout.y }, text_, segoe_ui, COLOR_WHITE,
						   renderer::align_left, renderer::align_top);
		}

	private:
		T text_;
	};
}// namespace carbon

#endif