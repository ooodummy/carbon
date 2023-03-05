#include "carbon/widgets/controls/separator.hpp"

carbon::separator::separator() : widget() {
	set_flex_basis(1.0f);

	set_margin(YGEdgeTop, theme.separator_spacing);
	set_margin(YGEdgeBottom, theme.separator_spacing);
}

void carbon::separator::handle_draw() {
	const auto layout = get_relative_layout();

	buf->draw_line({ layout.x, layout.y }, { layout.x + layout.z, layout.y }, theme.border);
}
