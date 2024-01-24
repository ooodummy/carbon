#ifndef CARBON_LAYOUT_PAINT_HPP
#define CARBON_LAYOUT_PAINT_HPP

#include "carbon/layout/node.hpp"

namespace carbon {
    // TODO: Maybe add the target renderer buffer as a parameter but I don't see many situations where we would need it.
    void paint(std::shared_ptr<node> node) {
        if (node->style.display == display_none)
            return;

        node->paint();

        auto child = node->first_child;
        while (child != nullptr) {
            paint(child);
            child = child->next;
        }
    }
}

#endif