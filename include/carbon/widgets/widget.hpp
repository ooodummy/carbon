#ifndef CARBON_WIDGETS_WIDGET_HPP
#define CARBON_WIDGETS_WIDGET_HPP

#include "carbon/globals.hpp"
#include "carbon/input.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <yoga/Yoga.h>

namespace carbon {
	// Wrapper for Yoga Layout YGNode, since I don't like C-style stuff.
	class widget : std::enable_shared_from_this<widget> {
	public:
		widget();
		explicit widget(YGConfigRef config);

		~widget();

		// Only should be used by top parents
		virtual void draw();
		virtual void input();

		virtual bool is_hovered() const;

	protected:
		virtual void handle_draw();
		virtual void handle_input();

	public:
		void set_parent(widget* parent);
		widget* get_top_parent() const;
		widget* get_parent() const;

		template<typename T, typename... Args>
		std::shared_ptr<T> add_child(Args&&... args) {
			auto item = std::make_shared<T>(std::forward<Args>(args)...);
			item->set_parent(this);
			YGNodeInsertChild(node_, item->node_, YGNodeGetChildCount(node_));
			children_.emplace_back(item);
			return item;
		}

		std::vector<std::shared_ptr<widget>> get_children() const;

		void set_active(bool active);
		void set_active_callbacks(bool active);

		void remove_child(const std::shared_ptr<widget>& child);

		glm::vec4 get_computed_layout() const;

		void update_absolute_position(const glm::vec2& parent_position);
		glm::vec2 get_absolute_position() const;

		glm::vec4 get_absolute_layout() const;

		YGNodeRef get_node() const;

	protected:
		bool active_ = true;

		YGNodeRef node_;

	private:
		widget* parent_ = nullptr;
		std::vector<std::shared_ptr<widget>> children_;

		glm::vec2 absolute_position_;
	};
}// namespace carbon

#endif