#ifndef CARBON_WIDGETS_PAGE_HPP
#define CARBON_WIDGETS_PAGE_HPP

namespace carbon {
	class page : public widget {
	public:
		page() : widget() {
			set_flex_direction(YGFlexDirectionRow);
			set_flex_grow(1.0f);
			set_padding(YGEdgeHorizontal, theme.page_padding.x);
			set_padding(YGEdgeVertical, theme.page_padding.y);
		}
	};
}

#endif