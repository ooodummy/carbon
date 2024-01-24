#ifndef CARBON_LAYOUT_NODE_HPP
#define CARBON_LAYOUT_NODE_HPP

#include "carbon/layout/styling.hpp"
#include "carbon/globals.hpp"

#include <string>

namespace carbon {
    // Basic node in the layout tree
    class node : public std::enable_shared_from_this<node> {
    public:
        virtual void paint() const {
            glm::vec2 position = {state.x - state.total_scroll_x, state.y - state.total_scroll_y};
            glm::vec2 size = {state.client_width, state.client_height};

            buf->draw_rect(position, position + size, COLOR_WHITE);
        }

        // State of the node updated by layout engine
        layout_node_state state;
        layout_properties layout;

        std::shared_ptr<node> first_child;
        std::shared_ptr<node> last_child;
        std::shared_ptr<node> next;
        std::shared_ptr<node> parent;
        std::shared_ptr<node> prev;

        std::string test_id;
    };
}

#endif