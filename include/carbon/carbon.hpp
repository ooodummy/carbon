#ifndef CARBON_CARBON_HPP
#define CARBON_CARBON_HPP

#include "carbon/widgets/containers/window.hpp"
#include "carbon/widgets/controls/checkbox.hpp"
#include "carbon/widgets/controls/dropdown.hpp"
#include "carbon/widgets/controls/label.hpp"
#include "carbon/widgets/controls/slider.hpp"
#include "widgets/controls/textbox.hpp"
#include "widgets/widget.hpp"

#include "carbon/globals.hpp"
#include "carbon/input.hpp"
#include "carbon/theme.hpp"

// https://pugixml.org/
namespace carbon {
    // Was planning on using this for the layout system, but I'm not sure if I want to use it anymore.
    void initialize();
    void begin();
    void end();

    void debug_info();
}// namespace carbon

#endif