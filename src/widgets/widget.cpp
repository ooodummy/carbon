#include "carbon/widgets/widget.hpp"

carbon::widget::widget() {
	node_ = YGNodeNew();
}

carbon::widget::widget(YGConfigRef config) {
	node_ = YGNodeNewWithConfig(config);
}

carbon::widget::~widget() {
	if (!YGNodeGetParent(node_))
		YGNodeFreeRecursive(node_);
}

void carbon::widget::draw() {
	if (YGNodeStyleGetDisplay(node_) == YGDisplayNone)
		return;

	handle_draw();

	for (auto& child : children_)
		child->draw();
}

void carbon::widget::input() {
	if (YGNodeStyleGetDisplay(node_) == YGDisplayNone)
		return;

	handle_input();

	for (auto& child : children_)
		child->input();
}

bool carbon::widget::is_hovered() const {
	return is_mouse_over(get_absolute_layout());
}

void carbon::widget::handle_draw() {}

void carbon::widget::handle_input() {}

void carbon::widget::set_parent(widget* parent) {
	parent_ = parent;
}

carbon::widget* carbon::widget::get_top_parent() const {
	if (!parent_)
		return nullptr;

	for (auto item = parent_;; item = item->get_parent()) {
		if (!item->get_parent())
			return item;
	}
}

carbon::widget* carbon::widget::get_parent() const {
	return parent_;
}

std::vector<std::shared_ptr<carbon::widget>> carbon::widget::get_children() const {
	return children_;
}

void carbon::widget::remove_child(const std::shared_ptr<widget>& child) {
	YGNodeRemoveChild(node_, child->node_);
	children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
}

void carbon::widget::set_active(bool active) {
	active_ = active;

	for (auto& child : children_)
		child->set_active(active_);
}

void carbon::widget::set_active_callbacks(bool active) {
	get_top_parent()->set_active(active);
}

glm::vec4 carbon::widget::get_computed_layout() const {
	return { YGNodeLayoutGetLeft(node_), YGNodeLayoutGetTop(node_), YGNodeLayoutGetWidth(node_), YGNodeLayoutGetHeight(node_) };
}

void carbon::widget::update_absolute_position(const glm::vec2& parent_position) {
	absolute_position_ = {
		YGNodeLayoutGetLeft(node_) + parent_position.x,
		YGNodeLayoutGetTop(node_) + parent_position.y
	};

	for (auto& child : children_) {
		child->update_absolute_position(absolute_position_);
	}
}

glm::vec2 carbon::widget::get_absolute_position() const {
	return absolute_position_;
}

glm::vec4 carbon::widget::get_absolute_layout() const {
	return {
		absolute_position_.x,
		absolute_position_.y,
		YGNodeLayoutGetWidth(node_),
		YGNodeLayoutGetHeight(node_)
	};
}

YGNodeRef carbon::widget::get_node() const {
	return node_;
}
