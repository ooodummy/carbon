#ifndef CARBON_LAYOUT_COMPOSE_HPP
#define CARBON_LAYOUT_COMPOSE_HPP

#include "carbon/layout/node.hpp"

namespace carbon {
    void compose(const std::shared_ptr<node>& node, glm::vec2 clip_start = {0.0f, 0.0f}, glm::vec2 clip_size = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()}, glm::vec2 scroll_offset = {0.0f, 0.0f});
}

#endif