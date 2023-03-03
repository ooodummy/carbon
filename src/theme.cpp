#include "carbon/theme.hpp"

#include "carbon/globals.hpp"

void carbon::init_default_theme() {
	theme.body = { 26, 26, 28 };
	theme.primary = { 233, 109, 109 };
	theme.title_bar = { 36, 34, 37 };
	theme.border = { 61, 59, 61 };

	theme.page_padding = { 10.0f, 10.0f };
	theme.groupbox_padding = { 8.0f, 8.0f };
	theme.groupbox_item_gap = { 5.0f, 5.0f };
	theme.groupbox_title_margin = { 0.0f, 0.0f };

	theme.label_padding = { 8.0f, 12.0f };
	theme.button_padding = { 6.0f, 6.0f };
}
