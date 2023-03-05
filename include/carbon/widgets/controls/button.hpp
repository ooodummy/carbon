#ifndef CARBON_WIDGETS_CONTROLS_BUTTON_HPP
#define CARBON_WIDGETS_CONTROLS_BUTTON_HPP

namespace carbon {
	class button : public widget {
	public:
		button(const std::wstring& name, const std::function<void()>& callback) :
			widget(),
			label_(name),
			callback_(callback) {
			set_flex_direction(YGFlexDirectionRow);
			set_padding(YGEdgeHorizontal, theme.button_padding.x);
			set_padding(YGEdgeVertical, theme.button_padding.y);

			const auto text_size = dx11->get_text_size(name, segoe_font);
			set_width(text_size.x + theme.button_padding.x * 2.0f);
			set_height(text_size.y + theme.button_padding.y * 2.0f);
		}

		bool is_hovered() const override {
			return is_mouse_over(get_relative_layout());
		}

		void handle_draw() override {
			const auto layout = get_relative_layout();

			buf->draw_rect_rounded_filled(layout, theme.button_rounding, theme.border);
			buf->draw_rect_rounded(layout, theme.button_rounding, theme.primary);

			buf->draw_text({ layout.x + layout.z / 2.0f, layout.y + layout.w / 2.0f }, label_, segoe_font, COLOR_WHITE,
						   renderer::text_align_center, renderer::text_align_center);
		}

		void handle_input() override {
			if (!active_)
				return;

			if (is_hovered() && is_key_pressed(VK_LBUTTON))
				callback_();
		}

		void set_callback(const std::function<void()>& callback) {
			callback_ = callback;
		}

	private:
		std::wstring label_;

		std::function<void()> callback_;
	};
}// namespace carbon

#endif