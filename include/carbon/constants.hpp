#ifndef CARBON_CONSTANTS_HPP
#define CARBON_CONSTANTS_HPP

#include <renderer/color.hpp>

namespace carbon {
    constexpr float CROSS_AXIS_SIZE = 10.0f;
    inline renderer::color_rgba SCROLLBAR_COLOR(25, 25, 25, 255);
    inline renderer::color_rgba SCROLLBAR_CORNER_COLOR = renderer::color_rgba(25, 25, 25, 255);
    inline renderer::color_rgba SCROLLBAR_TRACK_COLOR = renderer::color_rgba(58, 58, 58, 255);
    inline renderer::color_rgba SCROLLBAR_TRACK_HOVER_COLOR = renderer::color_rgba(72, 72, 72, 255);
}

#endif