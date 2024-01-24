#include "carbon/layout/paint.hpp"

namespace carbon {
    void paint(const std::shared_ptr<node>& node) {
        if (node->layout.display == display_none)
            return;

        node->paint();

        auto child = node->first_child;
        while (child != nullptr) {
            paint(child);
            child = child->next;
        }
    }
}