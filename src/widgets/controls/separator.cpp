#include "carbon/widgets/controls/separator.hpp"

carbon::separator::separator() : widget() {
	YGNodeStyleSetFlexBasis(node_, 1.0f);

	YGNodeStyleSetMargin(node_, YGEdgeTop, theme.separator_spacing);
	YGNodeStyleSetMargin(node_, YGEdgeBottom, theme.separator_spacing);
}

void carbon::separator::handle_draw() {
	const auto layout = get_absolute_layout();

	buf->draw_line({ layout.x, layout.y }, { layout.x + layout.z, layout.y }, theme.border);
}
