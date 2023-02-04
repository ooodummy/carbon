#ifndef CARBON_WIDGETS_INPUT_LABEL_HPP
#define CARBON_WIDGETS_INPUT_LABEL_HPP

#include "carbon/widgets/widget.hpp"

namespace carbon {
	// TODO: Optional label alignment
	template <typename T>
	class label : public widget_flex_container {
	public:
		label(const T& label, size_t font_id = 0, renderer::color_rgba color = COLOR_WHITE) : font_id_
			(font_id),
			color_
			(color) {
			label_ = label;
			set_flex(0.0f, 0.0f);
			set_basis(flex_unit::unit_pixel);

			container_ = add_child<flex_container>();
			container_->set_flex(0.0f, 0.0f);
			container_->set_basis(flex_unit::unit_pixel);

			set_min_width(0.0f);
			set_basis(0.0f);
			container_->set_min_width(0.0f);
			container_->set_basis(0.0f);
		}

		void set_label(const T& label) {
			label_ = label;
		}

		void decorate() override {
			//buf->draw_rect(get_bounds(), COLOR_GREEN);
			//buf->draw_rect(container_->get_bounds(), COLOR_RED);

			auto size = get_axes(dx11->get_text_size(label_, font_id_));
			size.cross += theme.label_padding.x;
			size.main += theme.label_padding.y;

			if (size != last_size_) {
				set_min_width(size.cross);
				set_basis(size.cross);
				set_flow(parent->get_flow().main == flex_direction::column ? flex_direction::row : flex_direction::column);
				container_->set_min_width(size.main);
				container_->set_basis(size.main);
			}

			auto bounds = container_->get_bounds();
			buf->draw_text<T>({bounds.x + bounds.z / 2.0f, bounds.y + bounds.w / 2.0f}, label_, font_id_, color_,
							  renderer::text_align_center, renderer::text_align_center);

			last_size_ = size;
		}

	private:
		std::shared_ptr<flex_container> container_;

		T label_;

		size_t font_id_;
		renderer::color_rgba color_;

		axes_vec2 last_size_;
	};
}// namespace carbon

#endif