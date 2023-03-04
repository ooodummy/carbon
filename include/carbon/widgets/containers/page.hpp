#ifndef CARBON_WIDGETS_CONTAINERS_PAGE_HPP
#define CARBON_WIDGETS_CONTAINERS_PAGE_HPP

namespace carbon {
	class page : public widget {
	public:
		page(const std::wstring& name) : widget(), label_(name) {
			set_flex_direction(YGFlexDirectionRow);
			set_flex_grow(1.0f);
			set_padding(YGEdgeHorizontal, theme.page_padding.x);
			set_padding(YGEdgeVertical, theme.page_padding.y);

			set_gap(YGGutterRow, theme.page_gap.x);
			set_gap(YGGutterColumn, theme.page_gap.y);

			set_width_percent(1.0f);
			set_height_percent(1.0f);

			set_flex_wrap(YGWrapWrap);
		}

		std::wstring get_label() {
			return label_;
		}

	private:
		std::wstring label_;
	};
}

#endif