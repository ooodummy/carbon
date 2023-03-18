#include "carbon/carbon.hpp"

#include <ShlObj.h>

void carbon::initialize() {
	char csidl_fonts[MAX_PATH];
	memset(csidl_fonts, 0, MAX_PATH);
	SHGetFolderPathA(nullptr, CSIDL_FONTS, nullptr, 0, csidl_fonts);

	carbon::segoe_ui = carbon::dx11->register_font(std::string(csidl_fonts) + '\\' + "seguiemj.ttf", 10, FW_THIN, true);

	init_default_theme();
}

void carbon::begin() {

}

void carbon::end() {
	input_end();
}
