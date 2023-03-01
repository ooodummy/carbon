#include "carbon/carbon.hpp"

void carbon::initialize() {
	carbon::segoe_font = carbon::dx11->register_font("Segoe UI Emoji", 10, FW_THIN, true);

	init_default_theme();
}

void carbon::begin() {

}

void carbon::end() {
	input_end();
}
