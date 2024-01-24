#ifndef CARBON_CARBON_HPP
#define CARBON_CARBON_HPP

#include "carbon/layout/view.hpp"
#include "carbon/layout/layout.hpp"
#include "carbon/layout/paint.hpp"

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