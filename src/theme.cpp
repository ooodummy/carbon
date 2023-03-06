#include "carbon/theme.hpp"

#include "carbon/globals.hpp"

void carbon::init_default_theme() {
	theme.body = { 26, 26, 28 };
	theme.primary = { 66, 132, 245 };
	theme.title_bar = { 36, 34, 37 };
	theme.border = { 61, 59, 61 };

	theme.window_rounding = 5.0f;

	theme.page_padding = { 10.0f, 10.0f };
	theme.page_gap = { 10.0f, 10.0f };

	theme.groupbox_rounding = 4.0f;
	theme.groupbox_title_size = 20.0f;
	theme.groupbox_padding = { 8.0f, 8.0f };
	theme.groupbox_item_gap = { 7.0f, 5.0f };
	theme.groupbox_title_margin = { 0.0f, 0.0f };

	theme.label_padding = { 6.0f, 6.0f };

	theme.button_rounding = 5.0f;
	theme.button_padding = { 4.0f, 4.0f };

	theme.separator_spacing = 2.0f;

	theme.checkbox_rounding = 4.0f;
	theme.checkbox_size = 15.0f;

	theme.slider_size = { 125.0f, 5.0f };

	theme.textbox_padding = { 4.0f, 4.0f };

	theme.dropdown_padding = { 4.0f, 4.0f };
}
