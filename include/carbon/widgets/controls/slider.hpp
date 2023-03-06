#ifndef CARBON_WIDGETS_CONTROLS_SLIDER_HPP
#define CARBON_WIDGETS_CONTROLS_SLIDER_HPP

#include "carbon/widgets/widget.hpp"

#include <fmt/xchar.h>

namespace carbon {
	template<typename T>
	class slider : public widget {
	public:
		slider(const std::wstring& label,
			   T* value,
			   const T min,
			   const T max,
			   std::optional<std::wstring> format = std::nullopt) :
			widget(),
			label_(label),
			value_(value),
			min_(min),
			max_(max) {
			set_flex_direction(YGFlexDirectionRow);

			const auto text_size = dx11->get_text_size(label, segoe_ui);
			set_min_width(text_size.x);
			set_height(text_size.y + theme.label_padding.y + theme.slider_size.y);
		}

		bool is_hovered() const override {
			return is_mouse_over(get_relative_layout());
		}

		void handle_draw() override {
			const auto layout = get_relative_layout();

			// Label
			buf->draw_text<std::wstring>({ layout.x, layout.y }, label_, segoe_ui, COLOR_WHITE,
										 renderer::text_align_left, renderer::text_align_top);

			std::wstring value;
			if constexpr (std::is_same_v<T, float>)
				value = fmt::format(L"{:.2f}", *value_);
			else
				value = fmt::format(L"{}", *value_);

			if (format_)
				value = fmt::format(*format_, value);

			// Value
			buf->draw_text<std::wstring>({ layout.x + layout.z, layout.y }, value, segoe_ui, COLOR_WHITE,
										 renderer::text_align_right, renderer::text_align_top);

			// Bar
			glm::vec4 bar(layout.x, layout.y + layout.w - theme.slider_size.y, layout.z, theme.slider_size.y);
			buf->draw_rect_filled(bar, theme.border);
			buf->draw_rect_filled(bar, theme.primary.alpha(animation_time_ * 255.0f));

			const auto percentage = (static_cast<float>(*value_) - static_cast<float>(min_)) /
									(static_cast<float>(max_) - static_cast<float>(min_));
			bar.z = std::floorf(layout.z * percentage);
			buf->draw_rect_filled(bar, theme.primary);
		}

		void handle_input() override {
			if (!active_)
				return;

			const auto hovered = is_hovered();

			if (hovered)
				animation_time_ = animation_time_ + timer.get_dt() / 0.5f;
			else
				animation_time_ = animation_time_ - timer.get_dt() / 0.5f;
			animation_time_ = std::clamp(animation_time_, 0.0f, 0.2f);

			if (hovered) {
				if (is_key_pressed(VK_LEFT))
					*value_ = std::clamp<T>(*value_ - (max_ - min_) / 100.0f, min_, max_);
				if (is_key_pressed(VK_RIGHT))
					*value_ = std::clamp<T>(*value_ + (max_ - min_) / 100.0f, min_, max_);
				if (is_key_pressed(VK_UP))
					*value_ = std::clamp<T>(*value_ + (max_ - min_) / 10.0f, min_, max_);
				if (is_key_pressed(VK_DOWN))
					*value_ = std::clamp<T>(*value_ - (max_ - min_) / 10.0f, min_, max_);

				if (!dragging_ && is_key_pressed(VK_LBUTTON))
					dragging_ = true;
			}

			if (dragging_ && !is_key_down(VK_LBUTTON))
				dragging_ = false;

			if (dragging_) {
				const auto layout = get_relative_layout();
				const auto mouse = get_mouse_pos();

				const float x = std::clamp((mouse.x - layout.x) / layout.z, 0.0f, 1.0f);
				*value_ =
				static_cast<T>((static_cast<float>(max_) - static_cast<float>(min_)) * x + static_cast<float>(min_));
			}
		}

	private:
		std::wstring label_;
		T* value_;
		T min_;
		T max_;
		std::optional<std::wstring> format_;

		bool dragging_ = false;
		float animation_time_;
	};
}// namespace carbon

#endif