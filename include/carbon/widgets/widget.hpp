#ifndef CARBON_WIDGETS_WIDGET_HPP
#define CARBON_WIDGETS_WIDGET_HPP

#include "carbon/globals.hpp"

#include <memory>

#include <glm/glm.hpp>
#include <yoga/Yoga.h>

namespace carbon {
	// Wrapper for Yoga Layout YGNode, since I don't like C-style stuff.
	class widget : std::enable_shared_from_this<widget> {
	public:
		widget();
		explicit widget(YGConfigRef config);

		~widget();

		virtual void draw();
		virtual void input();

		void set_parent(widget* parent);
		widget* get_top_parent() const;
		widget* get_parent() const;

		template<typename T, typename... Args>
		std::shared_ptr<T> add_child(Args&&... args) {
			auto item = std::make_shared<T>(std::forward<Args>(args)...);
			item->set_parent(this);
			YGNodeInsertChild(layout_, item->layout_, YGNodeGetChildCount(layout_));
			children_.emplace_back(item);
			return item;
		}

		void remove_child(const std::shared_ptr<widget>& child);

		void calculate_layout(float available_width, float available_height, YGDirection direction = YGDirectionLTR);
		void calculate_layout(glm::vec2 available_size, YGDirection direction = YGDirectionLTR);

		widget* set_direction(YGDirection direction);
		YGDirection get_direction();

		widget* set_flex_direction(YGFlexDirection flex_direction);
		YGFlexDirection get_flex_direction();

		widget* set_justify_content(YGJustify justify_content);
		YGJustify get_justify_content();

		widget* set_align_content(YGAlign align_content);
		YGAlign get_align_content();

		widget* set_align_items(YGAlign align_items);
		YGAlign get_align_items();

		widget* set_align_self(YGAlign align_self);
		YGAlign get_align_self();

		widget* set_position_type(YGPositionType position_type);
		YGPositionType get_position_type();

		widget* set_flex_wrap(YGWrap flex_wrap);
		YGWrap get_flex_wrap();

		widget* set_overflow(YGOverflow overflow);
		YGOverflow get_overflow();

		widget* set_display(YGDisplay display);
		YGDisplay get_display();

		widget* set_flex(float flex);
		float get_flex();

		widget* set_flex_grow(float flex_grow);
		float get_flex_grow();

		widget* set_flex_shrink(float flex_shrink);
		float get_flex_shrink();

		widget* set_flex_basis(float flex_basis);
		widget* set_flex_basis_percent(float flex_basis_percent);
		widget* set_flex_basis_auto();
		YGValue get_flex_basis();

		widget* set_position(YGEdge edge, float position);
		widget* set_position_percent(YGEdge edge, float position_percent);
		YGValue get_position(YGEdge edge);

		widget* set_margin(YGEdge edge, float margin);
		widget* set_margin_percent(YGEdge edge, float margin_percent);
		widget* set_margin_auto(YGEdge edge);
		YGValue get_margin(YGEdge edge);

		widget* set_padding(YGEdge edge, float padding);
		widget* set_padding_percent(YGEdge edge, float padding_percent);
		YGValue get_padding(YGEdge edge);

		widget* set_border(YGEdge edge, float border);
		float get_border(YGEdge edge);

		widget* set_gap(YGGutter gutter, float gap_length);
		float get_gap(YGGutter gutter);

		widget* set_width(float width);
		widget* set_width_percent(float width_percent);
		widget* set_width_auto();
		YGValue get_width();

		widget* set_height(float height);
		widget* set_height_percent(float height_percent);
		widget* set_height_auto();
		YGValue get_height();

		widget* set_min_width(float min_width);
		widget* set_min_width_percent(float min_width_percent);
		YGValue get_min_width();

		widget* set_min_height(float min_height);
		widget* set_min_height_percent(float min_height_percent);
		YGValue get_min_height();

		widget* set_max_width(float max_width);
		widget* set_max_width_percent(float max_width_percent);
		YGValue get_max_width();

		widget* set_max_height(float max_height);
		widget* set_max_height_percent(float max_height_percent);
		YGValue get_max_height();

		widget* set_aspect_ratio(float aspect_ratio);
		float get_aspect_ratio();

		glm::vec4 get_layout();
		YGDirection get_layout_direction();

		bool had_overflow();

		glm::vec4 get_layout_margin();
		glm::vec4 get_layout_border();
		glm::vec4 get_layout_padding();

		glm::vec4 get_relative_layout();

	private:
		widget* parent_;
		std::vector<std::shared_ptr<widget>> children_;

		YGNodeRef layout_;
	};
}

#endif