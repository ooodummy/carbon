#include "carbon/layout/compose.hpp"

namespace carbon {
    glm::vec4 intersection(glm::vec4 a, glm::vec4 b) {
        const float x = std::max(a.x, b.x);
        const float y = std::max(a.y, b.y);
        const float width = std::min(a.x + a.z, b.x + b.z) - x;
        const float height = std::min(a.y + a.w, b.y + b.w) - y;

        if (width <= 0 || height <= 0) {
            return {0.0f, 0.0f, 0.0f, 0.0f};
        }

        return {x, y, width, height};
    }

    void compose(const std::shared_ptr<node>& node, glm::vec2 clip_start, glm::vec2 clip_size, glm::vec2 scroll_offset) {
        if (node->layout.display == display_none)
            return;

        node->state.clip_start = clip_start;
        node->state.clip_size = clip_size;
        node->state.total_scroll_x = scroll_offset.x;
        node->state.total_scroll_y = scroll_offset.y;

        const glm::vec2 next_scroll_offset = scroll_offset + glm::vec2(node->state.scroll_x, node->state.scroll_y);

        const glm::vec4 clipped = intersection({node->state.x - scroll_offset.x, node->state.y - scroll_offset.y, node->state.client_width, node->state.client_height},
                                               {clip_start.x, clip_start.y, clip_size.x, clip_size.y});

        auto c = node->first_child;
        while (c != nullptr) {
            compose(c, {clipped.x, clipped.y}, {clipped.z, clipped.w}, next_scroll_offset);
            c = c->next;
        }
    }
}