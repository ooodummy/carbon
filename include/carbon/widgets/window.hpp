#ifndef CARBON_WIDGETS_WINDOW_HPP
#define CARBON_WIDGETS_WINDOW_HPP

#include "carbon/layout/base_view.hpp"

namespace carbon {
    // Windows are base views since they are not scrollable only the groupboxes and pages will be scrollable.
    class window : public base_view {
    public:
        window(const std::string& test_id = "") : base_view(layout_properties{
                                                                .border_width = 2.0f,
                                                                .height = carbon::value_type(1.0f, true),
                                                                .width = carbon::value_type(1.0f, true)
                                                            },
                                                            decorative_properties{
                                                                .background_color = {23, 23, 30},
                                                                .border_color = {255, 0, 0, 255},
                                                                .border_radius = 8.0f
                                                            },
                                                            test_id) {
        }

        void paint() const override {
            if (layout.display == display_none)
                return;

            glm::vec2 position = {state.x - state.total_scroll_x, state.y - state.total_scroll_y};
            glm::vec2 size = {state.client_width, state.client_height};

            buf->draw_rect_filled(position, position + size, style.background_color, coalesce(style.border_radius, 0.0f), renderer::edge_all);
            buf->draw_rect(position, position + size, style.border_color, coalesce(style.border_radius, 0.0f), renderer::edge_all, coalesce(layout.border_width, 1.0f));
        }
    };
}

#endif