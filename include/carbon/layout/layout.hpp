#ifndef CARBON_LAYOUT_LAYOUT_HPP
#define CARBON_LAYOUT_LAYOUT_HPP

#include "carbon/layout/base_view.hpp"

#include <deque>

namespace carbon {
    // TODO: Add checks for if the current layout is dirty and needs to be recomputed
    void layout(const std::shared_ptr<node>& tree, glm::vec2 root_size);
}

#endif