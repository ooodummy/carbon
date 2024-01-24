#ifndef CARBON_LAYOUT_BASE_VIEW_HPP
#define CARBON_LAYOUT_BASE_VIEW_HPP

#include "carbon/layout/node.hpp"

namespace carbon {
    class base_view : public node {
    public:
        base_view(const layout_properties& layout, const std::string& test_id = "") {
            this->test_id = test_id;
            this->layout = normalize_layout_properties(layout);
        }

        std::shared_ptr<node> add(std::shared_ptr<node> child) {
            if (first_child == nullptr) {
                first_child = child;
                last_child = child;
            } else {
                assert(last_child != nullptr);

                child->prev = last_child;
                last_child->next = child;
                last_child = child;
            }

            child->parent = shared_from_this();

            return child;
        }

        void remove(std::shared_ptr<node> child) {
            if (child->parent != shared_from_this()) {
                // TODO: Warning
                return;
            }

            if (child->prev != nullptr)
                child->prev->next = child->next;

            if (child->next != nullptr)
                child->next->prev = child->prev;

            if (first_child == child)
                first_child = child->next;

            if (last_child == child)
                last_child = child->prev;

            child->prev = nullptr;
            child->next = nullptr;
            child->parent = nullptr;
        }
    };
}

#endif