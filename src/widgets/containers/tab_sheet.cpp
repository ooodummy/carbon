#include "carbon/widgets/containers/tab_sheet.hpp"

// TODO: Redo tab sheets
carbon::tab_sheet::tab_sheet() : widget() {
	YGNodeStyleSetFlexDirection(node_, YGFlexDirectionRow);
	//YGNodeStyleSetJustifyContent(node_, YGJustifySpaceEvenly);
	YGNodeStyleSetFlexGrow(node_, 1.0f);

	bar_ = add_child<widget>();
	YGNodeStyleSetFlexDirection(bar_->get_node(), YGFlexDirectionColumn);
	YGNodeStyleSetWidth(bar_->get_node(), 112.0f);

	content_ = add_child<widget>();
	YGNodeStyleSetFlexDirection(content_->get_node(), YGFlexDirectionRow);
	YGNodeStyleSetFlexGrow(content_->get_node(), 1.0f);
}

void carbon::tab_sheet::handle_draw() {
	const auto layout = get_absolute_layout();
	buf->draw_rect_filled(layout, COLOR_BLUE.alpha(150));

	const auto bar = bar_->get_absolute_layout();
	const auto content_children = content_->get_children();
	const auto size = bar.w / (static_cast<float>(content_children.size()));

	//buf->draw_rect_rounded_filled(bar, theme.window_rounding, theme.border, renderer::edge_bottom_left);
	buf->draw_rect_filled(bar, theme.border);

	for (size_t i = 0; i < content_children.size(); i++) {
		const auto child = dynamic_cast<page*>(content_children[i].get());
		assert(child);

		const glm::vec4 button(bar.x, bar.y + i * size, bar.z, size);

		buf->draw_text({ button.x + button.z / 2.0f, button.y + button.w / 2.0f }, child->label_, fa_regular,
					   COLOR_WHITE.alpha(child->animation_time_ * 255.0f),
					   renderer::align_center);
	}
}

void carbon::tab_sheet::handle_input() {
	if (!active_)
		return;

	const auto bar = bar_->get_absolute_layout();
	const auto content_children = content_->get_children();
	const auto size = bar.w / (static_cast<float>(content_children.size()));

	for (size_t i = 0; i < content_children.size(); i++) {
		const auto child = dynamic_cast<page*>(content_children[i].get());
		assert(child);

		const glm::vec4 button(bar.x, bar.y + i * size, bar.z, size);
		const bool hovered = is_mouse_over(button);

		const auto animation_speed = timer.get_dt() / 0.25f;
		hovered ? child->animation_time_ += animation_speed : child->animation_time_ -= animation_speed;
		child->animation_time_ = std::clamp(child->animation_time_, 0.3f, 1.0f);

		if (hovered && is_key_pressed(VK_LBUTTON)) {
			selected_ = i;
		}
	}

	for (size_t i = 0; i < content_children.size(); i++) {
		const auto child = dynamic_cast<page*>(content_children[i].get());
		assert(child);

		YGNodeStyleSetDisplay(child->get_node(), i == selected_ ? YGDisplayFlex : YGDisplayNone);

		if (YGNodeStyleGetDisplay(child->get_node()) == YGDisplayFlex)
			child->animation_time_ = 1.0f;
	}
}

std::shared_ptr<carbon::page> carbon::tab_sheet::add_page(const std::u32string& name) {
	return content_->add_child<page>(name);
}
