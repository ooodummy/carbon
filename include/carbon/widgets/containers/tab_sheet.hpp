#ifndef CARBON_WIDGETS_CONTAINERS_TAB_SHEET_HPP
#define CARBON_WIDGETS_CONTAINERS_TAB_SHEET_HPP

#include "carbon/widgets/containers/page.hpp"
#include "carbon/widgets/widget.hpp"

namespace carbon {
	// TODO: Inherit from this to style sub tab sheet
	class tab_sheet : public widget {
	public:
		tab_sheet() : widget() {
			set_flex_direction(YGFlexDirectionRow);
			set_flex_grow(1.0f);

			bar_ = add_child<widget>();
			bar_->set_width(112.0f);
			bar_->set_flex_direction(YGFlexDirectionColumn);
		}

		void handle_draw() override {
			const auto bar = bar_->get_relative_layout();
			const auto size = bar.w / (static_cast<float>(get_children().size()) - 1.0f);

			buf->draw_rect_rounded_filled(bar, theme.window_rounding, theme.border, renderer::edge_bottom_left);

			auto children = get_children();
			for (size_t i = 1; i < children.size(); i++) {
				const auto child = reinterpret_cast<page*>(children[i].get());
				const glm::vec4 button(bar.x, bar.y + (i - 1) * size, bar.z, size);

				buf->draw_text({button.x + button.z / 2.0f, button.y + button.w / 2.0f}, child->get_label(), segoe_font,
							   COLOR_WHITE, renderer::text_align_center, renderer::text_align_center);
			}
		}

		void handle_input() override {
			const auto bar = bar_->get_relative_layout();
			const auto size = bar.w / (static_cast<float>(get_children().size()) - 1.0f);

			auto children = get_children();
			for (size_t i = 1; i < children.size(); i++) {
				const auto child = reinterpret_cast<page*>(children[i].get());
				const glm::vec4 button(bar.x, bar.y + (i - 1) * size, bar.z, size);
				bool hovered = is_mouse_over(button);

				if (hovered && is_key_pressed(VK_LBUTTON)) {
					selected_ = i;
					child->set_visible(true);

					get_top_parent()->calculate_layout(carbon::application->get_size());
				}
				else {
					child->set_visible(i == selected_);
				}
			}
		}

	private:
		std::shared_ptr<widget> bar_;
		size_t selected_ = 0;
	};
}

#endif