#ifndef CARBON_LAYOUT_CONTAINERS_BASE_CONTAINER_HPP
#define CARBON_LAYOUT_CONTAINERS_BASE_CONTAINER_HPP

#include "carbon/layout/item.hpp"

#include <memory>
#include <vector>

namespace carbon {
	class base_flex_container : public flex_item {
	public:
		friend class flex_container;

		void draw() override;

		template<typename T, typename... Args>
		std::shared_ptr<T> add_child(Args&&... args) {
			auto item = std::make_shared<T>(std::forward<Args>(args)...);
			item->parent = this;
			children_.emplace_back(item);
			return item;
		}

		std::shared_ptr<carbon::flex_item> add_child(std::shared_ptr<flex_item> item);

		std::vector<std::shared_ptr<flex_item>>& get_children();

		const flex_flow& get_flow() const;

		void set_flow(flex_direction axis);
		void set_flow(flex_wrap wrap);
		void set_flow(flex_direction axis, flex_wrap wrap);

		void set_justify_content(flex_justify_content justify_content);

	protected:
		void measure_contents() override;
		void decorate() override;

		axes_vec2 content_min_axes_;

		axes_vec4 get_axes(glm::vec4 src) const;
		axes_vec2 get_axes(glm::vec2 src) const;

		glm::vec2 get_main(glm::vec4 src) const;
		float get_main(glm::vec2 src) const;
		glm::vec2 get_cross(glm::vec4 src) const;
		float get_cross(glm::vec2 src) const;

		void set_main(glm::vec2& dst, float src) const;

		flex_flow flow_;

		std::vector<std::shared_ptr<flex_item>> children_;
	};
}// namespace carbon

#endif