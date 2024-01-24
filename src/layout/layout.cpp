#include "carbon/layout/layout.hpp"

namespace carbon {
    void layout(std::shared_ptr<node> tree, glm::vec2 root_size) {
        std::deque<std::shared_ptr<node>> traversal_queue;

        const auto root = std::make_shared<base_view>(layout_properties {.height = value_type {root_size.y, false}, .width = value_type {root_size.x, false}}, "layout##root");
        root->add(tree);

        std::vector<std::shared_ptr<node>> nodes_in_level_order = {root};

        // Traverse the tree in level order and generate the reverse queue
        traversal_queue.push_back(root);
        while (!traversal_queue.empty()) {
            const auto e = traversal_queue.front();
            traversal_queue.pop_front();

            nodes_in_level_order.push_back(e);

            const auto is_horizontal = e->parent->layout.flex_direction == flex_direction_row ||
                                       e->parent->layout.flex_direction == flex_direction_row_reverse;

            auto c = e->first_child;
            while (c != nullptr) {
                traversal_queue.push_back(c);
                c = c->next;
            }

            // Reset output layout state
            e->state.x = 0.0f;
            e->state.y = 0.0f;
            e->state.client_width = 0.0f;
            e->state.client_height = 0.0f;
            e->state.scroll_width = 0.0f;
            e->state.scroll_height = 0.0f;
            e->state.scroll_x = 0.0f;
            e->state.scroll_y = 0.0f;
            e->state.has_horizontal_scrollbar = e->layout.overflow_x == overflow_scroll;
            e->state.has_vertical_scrollbar = e->layout.overflow_y == overflow_scroll;

            if (e->layout.width.has_value() && !e->layout.width->percentage)
                e->state.client_width = e->layout.width.value().value;
            if (e->layout.height.has_value() && !e->layout.height->percentage)
                e->state.client_height = e->layout.height.value().value;

            if (e->layout.flex_basis.has_value() && !e->layout.flex_basis->percentage) {
                if (is_horizontal) {
                    e->state.client_width = e->layout.flex_basis.value().value;
                } else {
                    e->state.client_height = e->layout.flex_basis.value().value;
                }
            }

            // Resolve percentage scales
            if (e->layout.width.has_value() && e->layout.width->percentage) {
                std::optional<float> defined_width = std::nullopt;
                float accumulated_multiplier = 1.0f;

                auto p = e->parent;
                while (!defined_width.has_value() && p != nullptr) {
                    if (p->layout.width.has_value()) {
                        if (p->layout.width.value().percentage) {
                            accumulated_multiplier *= p->layout.width.value().value;
                        } else {
                            defined_width = p->layout.width.value().value;
                        }
                    }

                    p = p->parent;
                }

                e->state.client_width = e->layout.width.value().value * accumulated_multiplier * coalesce(defined_width, 0.0f);
            }

            if (e->layout.height.has_value() && e->layout.height->percentage) {
                std::optional<float> defined_height = std::nullopt;
                float accumulated_multiplier = 1.0f;

                auto p = e->parent;
                while (!defined_height.has_value() && p != nullptr) {
                    if (p->layout.height.has_value()) {
                        if (p->layout.height.value().percentage) {
                            accumulated_multiplier *= p->layout.height.value().value;
                        } else {
                            defined_height = p->layout.height.value().value;
                        }
                    }

                    p = p->parent;
                }

                e->state.client_height = e->layout.height.value().value * accumulated_multiplier * coalesce(defined_height, 0.0f);
            }

            // Resolve flex-basis
            if (e->layout.flex_basis.has_value() && e->layout.flex_basis.value().percentage) {
                if (is_horizontal) {
                    e->state.client_width = e->layout.flex_basis.value().value * e->parent->state.client_width;
                } else {
                    e->state.client_height = e->layout.flex_basis.value().value * e->parent->state.client_height;
                }
            }

            // TODO: Resolve text size
        }

        // Second tree pass, resolve wrapping children.
        // Going bottom-up level order.
        //for (size_t i = nodes_in_level_order.size() - 1; i >= 0; i--) {
        //    const auto e = nodes_in_level_order[i];
        //}
    }
}