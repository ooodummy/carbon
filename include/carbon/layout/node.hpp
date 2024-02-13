#ifndef CARBON_LAYOUT_NODE_HPP
#define CARBON_LAYOUT_NODE_HPP

#include "carbon/layout/styling.hpp"
#include "carbon/constants.hpp"
#include "carbon/globals.hpp"

#include <string>
#include <memory>

namespace carbon {
    // Basic node in the layout tree
    class node : public std::enable_shared_from_this<node> {
    public:
        virtual void paint() const;

        // State of the node updated by layout engine
        layout_node_state state;
        layout_properties layout;

        std::shared_ptr<node> first_child;
        std::shared_ptr<node> last_child;
        std::shared_ptr<node> next;
        node* parent = nullptr;
        std::shared_ptr<node> prev;

        std::string test_id;
    };
}

#endif