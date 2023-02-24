#include "carbon/layout/containers/base_flex_container.hpp"

#include "carbon/globals.hpp"

void carbon::base_flex_container::measure_contents() {
	compute_box_model();

	// All content min is always at least the total padding added up
	const auto total_padding_axes = get_axes(get_total_padding());
	content_min_axes_ = total_padding_axes;

	auto cross_min = 0.0f;

	for (auto& child : children_) {
		// Calculate minimum content size in child
		child->measure_contents();

		// Clamp main axis child content min by its width
		auto child_content_min_axes = get_axes(child->content_min_);
		child_content_min_axes.main = std::max(child_content_min_axes.main, child->min_width_);

		// Update it for the child
		// because it's later used inside of clamp when a flex container
		// is clamping its children
		set_main(child->content_min_, child_content_min_axes.main);

		// Add up minimum main axis side and store largest cross axis
		content_min_axes_.main += child_content_min_axes.main;
		cross_min = std::max(cross_min, child_content_min_axes.cross);
	}

	content_min_axes_.cross += cross_min;
	content_min_ = glm::vec2(content_min_axes_);
}

void carbon::base_flex_container::draw() {
	decorate();

	for (auto& child : children_) {
		child->draw();
	}
}

std::shared_ptr<carbon::flex_item> carbon::base_flex_container::add_child(std::shared_ptr<flex_item> item) {
	item->parent = this;
	children_.push_back(std::move(item));
	return children_.back();
}

std::vector<std::shared_ptr<carbon::flex_item>>& carbon::base_flex_container::get_children() {
	return children_;
}

void carbon::base_flex_container::decorate() {
	buf->draw_rect_filled(margin_.get_edge(), { 153, 93, 181 });
	buf->draw_rect(margin_.get_edge(), { 24, 26, 27 });
	buf->draw_rect_filled(content_, { 247, 148, 31 });

	auto margin = get_axes(margin_.get_edge());

	auto start = get_axes(carbon::get_pos(margin_.get_edge()));
	start.cross += margin.cross.y / 2.0f;
	auto end = start;
	end.main += margin.main.y;

	buf->draw_line(glm::vec2(start), glm::vec2(end), COLOR_WHITE);
}

const carbon::flex_flow& carbon::base_flex_container::get_flow() const {
	return flow_;
}

void carbon::base_flex_container::set_flow(flex_direction axis) {
	mark_dirty_and_propagate();
	flow_.main = axis;
	flow_.cross = (axis == row || axis == row_reversed) ? column : row;
}

void carbon::base_flex_container::set_flow(flex_wrap wrap) {
	mark_dirty_and_propagate();
	flow_.wrap = wrap;
}

void carbon::base_flex_container::set_flow(flex_direction axis, flex_wrap wrap) {
	mark_dirty_and_propagate();
	flow_.main = axis;
	flow_.cross = (axis == row || axis == row_reversed) ? column : row;
	flow_.wrap = wrap;
}

carbon::axes_vec4 carbon::base_flex_container::get_axes(glm::vec4 src) const {
	if (flow_.main == row || flow_.main == row_reversed) {
		return {
			{src.x,	 src.z},
			{ src.y, src.w},
			flow_.main
		};
	}
	else {
		return {
			{src.y,	 src.w},
			{ src.x, src.z},
			flow_.main
		};
	}
}

carbon::axes_vec2 carbon::base_flex_container::get_axes(glm::vec2 src) const {
	if (flow_.main == row || flow_.main == row_reversed) {
		return { src.x, src.y, flow_.main };
	}
	else {
		return { src.y, src.x, flow_.main };
	}
}

glm::vec2 carbon::base_flex_container::get_main(glm::vec4 src) const {
	return get_axis(src, flow_.main);
}

float carbon::base_flex_container::get_main(glm::vec2 src) const {
	return get_axis(src, flow_.main);
}

glm::vec2 carbon::base_flex_container::get_cross(glm::vec4 src) const {
	return get_axis(src, flow_.cross);
}

float carbon::base_flex_container::get_cross(glm::vec2 src) const {
	return get_axis(src, flow_.cross);
}

void carbon::base_flex_container::set_main(glm::vec2& dst, float src) const {
	if (flow_.main == row || flow_.main == row_reversed) {
		dst.x = src;
	}
	else {
		dst.y = src;
	}
}

void carbon::base_flex_container::set_justify_content(const flex_justify_content justify_content) {
	mark_dirty_and_propagate();
	flow_.justify_content = justify_content;
}