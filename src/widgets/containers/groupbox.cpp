#include "carbon/widgets/containers/groupbox.hpp"

carbon::groupbox::groupbox(const std::wstring& name) : widget(), label_(name) {
	set_flex_direction(YGFlexDirectionColumn);
	set_flex_grow(1.0f);

	const auto text_size = dx11->get_text_size(label_, segoe_ui);
	set_padding(YGEdgeHorizontal, theme.groupbox_padding.x);
	set_padding(YGEdgeVertical, theme.groupbox_padding.y);
	set_padding(YGEdgeTop, theme.groupbox_title_size + theme.groupbox_padding.y * 2.0f);

	set_min_width(text_size.x + theme.groupbox_padding.x * 2.0f);

	// Causes issues with positioning
	//set_min_width(150.0f);
	//set_max_width(250.0f);

	set_gap(YGGutterRow, theme.groupbox_item_gap.x);
	set_gap(YGGutterColumn, theme.groupbox_item_gap.y);
}

void carbon::groupbox::handle_draw() {
	const auto layout = get_relative_layout();
	const glm::vec4 title(layout.x, layout.y, layout.z, theme.groupbox_title_size + theme.groupbox_padding.y);

	//buf->draw_rect_rounded_filled(layout, theme.groupbox_rounding, theme.body);
	//buf->draw_rect_rounded_filled(title, theme.groupbox_rounding, theme.title_bar, renderer::edge_top);
	//buf->draw_rect_rounded(layout, theme.groupbox_rounding, theme.border);

	buf->draw_rect_filled(layout, theme.body);
	buf->draw_rect_filled(title, theme.title_bar);
	buf->draw_rect(layout, theme.border);

	buf->draw_line({ title.x, title.y + title.w }, { title.x + title.z, title.y + title.w }, theme.border, 1.0f);

	buf->draw_text({ title.x + theme.groupbox_padding.x, title.y + theme.groupbox_padding.y }, label_, segoe_ui,
				   COLOR_WHITE, renderer::align_left | renderer::align_top);

	// TODO: Resize grip, minimize, scrolling, clipping, dragging
}
