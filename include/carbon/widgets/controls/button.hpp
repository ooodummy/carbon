#ifndef CARBON_WIDGETS_CONTROLS_BUTTON_HPP
#define CARBON_WIDGETS_CONTROLS_BUTTON_HPP

#include "carbon/widgets/widget.hpp"

#include "carbon/widgets/controls/label.hpp"

#include <functional>

namespace carbon {
	template <typename T>
	class button : public widget_flex_container {
	public:
		button(const T& label, size_t font_id = 0) {
			label_ = add_child<carbon::label<T>>(label, font_id);

			set_flex(0.0f, 0.0f);
			set_basis(flex_unit::unit_pixel);
			set_max_width(50.0f);
		}

		void set_label(const T& label) {
			label_->set_label(label);
		}

		void set_callback(const std::function<void()>& callback) {
			callback_ = callback;
		}

		void decorate() override {
			auto bounds = get_bounds();

			buf->draw_rect_filled(bounds, theme.border);
			buf->draw_rect(bounds, theme.primary);
		}

	private:
		std::shared_ptr<carbon::label<T>> label_;

		std::function<void()> callback_;
	};
}// namespace carbon

#endif