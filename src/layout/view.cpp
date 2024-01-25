#include "carbon/layout/view.hpp"

namespace carbon {
    void view::paint() const {
        if (layout.display == display_none)
            return;

        base_view::paint();

        const glm::vec2 position = {state.x - state.total_scroll_x, state.y - state.total_scroll_y};
        const glm::vec2 size = {state.client_width, state.client_height};

        const auto end = position + size;

        const auto scrollbar_radius = CROSS_AXIS_SIZE / 2.0f;

        // Corner resize handle
        if (state.has_vertical_scrollbar && state.has_horizontal_scrollbar) {
            buf->draw_triangle_filled(end - glm::vec2 {CROSS_AXIS_SIZE, 0.0f}, end - glm::vec2 {0.0f, CROSS_AXIS_SIZE}, end, style.border_color);
        }
        if (state.has_vertical_scrollbar) {
            const auto scrollbar_size = size.y;
            const auto scrollbar_track_color = scrolling_.y_hovered || scrolling_.y_active ? SCROLLBAR_TRACK_HOVER_COLOR : SCROLLBAR_TRACK_COLOR;

            buf->draw_rect_filled(position + glm::vec2 {size.x, 0.0f}, position + glm::vec2 {CROSS_AXIS_SIZE, scrollbar_size}, SCROLLBAR_COLOR);

            const auto scroll_track_size = (state.client_height / state.scroll_height) * scrollbar_size;
            const auto scroll_track_position = (state.scroll_y / state.scroll_height) * scrollbar_size;

            buf->draw_rect_filled(position + glm::vec2 {size.x, scroll_track_position}, position + glm::vec2 {CROSS_AXIS_SIZE, scroll_track_size}, scrollbar_track_color, scrollbar_radius, renderer::edge_all);
        }
        if (state.has_horizontal_scrollbar) {
            const auto scrollbar_size = size.x;
            const auto scrollbar_track_color = scrolling_.x_hovered || scrolling_.x_active ? SCROLLBAR_TRACK_HOVER_COLOR : SCROLLBAR_TRACK_COLOR;

            buf->draw_rect_filled(position + glm::vec2 {0.0f, size.y}, position + glm::vec2 {scrollbar_size, CROSS_AXIS_SIZE}, SCROLLBAR_COLOR);

            const auto scroll_track_size = (state.client_width / state.scroll_width) * scrollbar_size;
            const auto scroll_track_position = (state.scroll_x / state.scroll_width) * scrollbar_size;

            buf->draw_rect_filled(position + glm::vec2 {scroll_track_position, size.y}, position + glm::vec2 {scroll_track_size, CROSS_AXIS_SIZE}, scrollbar_track_color, scrollbar_radius, renderer::edge_all);
        }
    }
}