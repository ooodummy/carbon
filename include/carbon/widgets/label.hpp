#ifndef CARBON_WIDGETS_LABEL_HPP
#define CARBON_WIDGETS_LABEL_HPP

#include "widget.hpp"

namespace carbon {
	template <typename T>
	class label : public widget {
	public:
		label() : widget() {}

		explicit label(T text) : widget() {
			set_text(text);
		}

		void set_text(T text) {
			text_ = text;

			const auto text_size = dx11->get_text_size(text_, segoe_font);
			set_width(text_size.x);
			set_height(text_size.y);
		}

		void handle_draw() override {
			const auto layout = get_relative_layout();

			buf->draw_text({layout.x, layout.y}, text_, segoe_font, COLOR_WHITE, renderer::text_align_left, renderer::text_align_top);
			buf->draw_rect(layout, COLOR_WHITE);
		}

	private:
		T text_;
	};
}

#endif