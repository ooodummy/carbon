#ifndef CARBON_WIDGETS_WINDOW_HPP
#define CARBON_WIDGETS_WINDOW_HPP

#include "widget.hpp"

#include "tab_sheet.hpp"
#include "page.hpp"

#include "groupbox.hpp"

#include "button.hpp"

namespace carbon {
	class window : public widget {
	public:
		window() : widget(YGConfigNew()) {
			set_width(500);
			set_height(300);
			set_flex_direction(YGFlexDirectionColumn);

			title_bar_ = add_child<carbon::widget>();
			title_bar_->set_height(38);
			title_bar_->set_flex_direction(YGFlexDirectionRow);
			title_bar_->set_align_items(YGAlignCenter);

			title_ = title_bar_->add_child<carbon::label<std::string>>("Window Title");
			title_->set_margin(YGEdgeLeft, 10.0f);

			//sub_tab_sheet_ = add_child<carbon::sub_tab_sheet>();

			auto area = add_child<carbon::widget>();
			area->set_flex_direction(YGFlexDirectionRow);
			area->set_flex_grow(1.0f);

			tab_sheet_ = area->add_child<carbon::tab_sheet>();
			page_ = area->add_child<carbon::page>();
		}

		void handle_draw() override {
			const auto layout = get_relative_layout();

			buf->draw_rect_rounded(layout + glm::vec4(- 1.0f, -1.0f, 2.0f, 2.0f), 5.0f, theme.primary, 1.0f);
			buf->draw_rect_rounded_filled(layout, 4.0f, theme.body);

			//buf->draw_rect(layout + glm::vec4(- 1.0f, -1.0f, 2.0f, 2.0f), theme.primary);
			//buf->draw_rect_filled(layout, theme.body);

			const auto title_layout = title_bar_->get_relative_layout();
			buf->draw_rect_filled(title_layout, theme.title_bar);
		}

		void handle_input() override {
			static bool dragging_title_bar = false;
			static glm::vec2 drag_offset;

			if (!dragging_title_bar && is_mouse_over(title_bar_->get_relative_layout()) && is_key_pressed(VK_LBUTTON)) {
				dragging_title_bar = true;
				drag_offset = get_mouse_pos() - glm::vec2(get_layout().x, get_layout().y);
			}
			else if (dragging_title_bar && !is_key_down(VK_LBUTTON)) {
				dragging_title_bar = false;
			}

			if (dragging_title_bar) {
				const auto mouse_pos = get_mouse_pos();
				set_position(YGEdgeLeft, mouse_pos.x - drag_offset.x);
				set_position(YGEdgeTop, mouse_pos.y - drag_offset.y);
			}
		}

	private:
		std::shared_ptr<widget> title_bar_;
		std::shared_ptr<label<std::string>> title_;

		std::shared_ptr<tab_sheet> tab_sheet_;

	public:
		std::shared_ptr<page> page_;
	};
}

#endif