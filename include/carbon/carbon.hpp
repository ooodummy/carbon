#ifndef CARBON_CARBON_HPP
#define CARBON_CARBON_HPP

#include "carbon/widgets/controls/label.hpp"
#include "widgets/containers/groupbox.hpp"
#include "widgets/containers/panel.hpp"
#include "widgets/containers/tab_bar.hpp"
#include "widgets/containers/window.hpp"
#include "widgets/controls/button.hpp"
#include "widgets/controls/label.hpp"

// https://pugixml.org/

namespace carbon {
	// Was planning on using this for the layout system, but I'm not sure if I want to use it anymore.
	extern panel window_panel;

	void init();
	void begin();
	void end();
}// namespace carbon

#endif