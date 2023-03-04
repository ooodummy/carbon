#ifndef CARBON_CARBON_HPP
#define CARBON_CARBON_HPP

#include "carbon/widgets/containers/window.hpp"
#include "carbon/widgets/controls/checkbox.hpp"
#include "carbon/widgets/controls/label.hpp"
#include "carbon/widgets/controls/slider.hpp"
#include "widgets/controls/textbox.hpp"
#include "globals.hpp"
#include "input.hpp"
#include "theme.hpp"
#include "widgets/widget.hpp"

// TODO: Custom windows for radar/indicators

// https://pugixml.org/

namespace carbon {
	void initialize();
	void begin();
	void end();
}// namespace carbon

#endif