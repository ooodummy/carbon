#include "carbon/layout/base_view.hpp"

namespace carbon {
    base_view::base_view(const layout_properties& layout, const decorative_properties& style, const std::string& test_id) {
        this->test_id = test_id;
        this->layout = layout;
        this->layout.normalize();
        this->style = style;
    }

    void base_view::paint() const {
        if (layout.display == display_none)
            return;

        glm::vec2 position = {state.x - state.total_scroll_x, state.y - state.total_scroll_y};
        glm::vec2 size = {state.client_width, state.client_height};

        // TODO: Handle unique border edge thicknesses and render a border too
        buf->draw_rect_filled(position, position + size, style.background_color, coalesce(style.border_radius, 0.0f), renderer::edge_all);
        buf->draw_rect(position, position + size, style.border_color, coalesce(style.border_radius, 0.0f), renderer::edge_all, coalesce(layout.border_width, 1.0f));
    }

    std::shared_ptr<node> base_view::add(const std::shared_ptr<node>& child) {
        child->parent = shared_from_this();

        if (first_child == nullptr) {
            first_child = child;
            last_child = child;
        } else {
            assert(last_child != nullptr);

            child->prev = last_child;
            last_child->next = child;
            last_child = child;
        }

        return child;
    }

    void base_view::remove(const std::shared_ptr<node>& child) {
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
}