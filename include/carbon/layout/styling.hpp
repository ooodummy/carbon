#ifndef CARBON_LAYOUT_STYLING_HPP
#define CARBON_LAYOUT_STYLING_HPP

#include <renderer/font.hpp>
#include <renderer/color.hpp>

#include <memory>
#include <optional>
#include <vector>

namespace carbon {
    class node;

    struct layout_node_state {
        std::vector<std::vector<std::shared_ptr<node>>> children = {};
        float client_height = 0.0f;
        float client_width = 0.0f;
        glm::vec2 clip_size = {0.0f, 0.0f};
        glm::vec2 clip_start = {0.0f, 0.0f};
        bool has_horizontal_scrollbar = false;
        bool has_vertical_scrollbar = false;
        float scroll_height = 0.0f;
        float scroll_width = 0.0f;
        float scroll_x = 0.0f;
        float scroll_y = 0.0f;
        std::optional<float> text_width_limit = std::nullopt;
        float total_scroll_x = 0.0f;
        float total_scroll_y = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
    };

    // Controls positioning of children on the cross-axis
    enum e_align_items {
        align_items_center,
        align_items_end,
        align_items_start,
        align_items_stretch // Stretches to fill the parent's cross-axis
    };

    // Controls positioning of children on the main-axis
    enum e_justify_content {
        justify_content_center,
        justify_content_end,
        justify_content_space_around,   // Equal gaps between all elements and half that gap before the first and after the last
        justify_content_space_between,  // Does not leave any gap before the first element or after the last element
        justify_content_space_evenly,   // Uses equal gaps between all elements and before the first and after the last
        justify_content_start
    };

    // Controls positions and sizes of lines in the flex wrap, does not have any effect if flex_wrap is no_wrap
    enum e_align_content {
        align_content_center,
        align_content_end,
        align_content_space_around,   // Equal gaps between all elements and half that gap before the first and after the last
        align_content_space_between,  // Does not leave any gap before the first element or after the last element
        align_content_space_evenly,   // Uses equal gaps between all elements and before the first and after the last
        align_content_start,
        align_content_stretch
    };

    enum e_align_self {
        align_self_auto,
        align_self_center,
        align_self_end,
        align_self_start,
        align_self_stretch         // Stretches to fill the parent's cross-axis
    };

    enum e_flex_direction {
        flex_direction_column,         // The main-axis is vertical
        flex_direction_column_reverse, // Reverses order
        flex_direction_row,            // The main axis is the horizontal one
        flex_direction_row_reverse     // Reverses order
    };

    enum e_flex_wrap {
        flex_wrap_no_wrap,        // Does not wrap lines, making them potentially overflow the parent
        flex_wrap_wrap,           // Wraps lines of elements if they exceed available space in the main axis
        flex_wrap_wrap_reverse    // Reverses the order of lines
    };

    enum e_overflow {
        overflow_auto,      // Shows scrollbars if needed
        overflow_hidden,    // Clips content but does not show scrollbars
        overflow_scroll,    // Shows scrollbars always
        overflow_visible    // Lets content overflow the parent
    };

    enum e_display {
        display_flex,           // Behaves similar to css flex
        display_none            // Hides the element from layout and prevents it from participating in layout calculations
    };

    enum e_position {
        // The element will not participate in the layout of other elements within the same parent, coordinates are
        // relative to the parent since every node is either absolute or relative
        position_absolute,
        // Will move the element around but not influence the layout of other elements
        position_relative
    };

    struct value_type {
        float value;
        bool percentage;
    };

    struct layout_properties {
        e_align_content align_content = align_content_start;
        e_align_items align_items = align_items_start;
        e_align_self align_self = align_self_auto;
        std::optional<float> aspect_ratio = std::nullopt;
        std::optional<float> border_bottom_width = std::nullopt;
        std::optional<float> border_left_width = std::nullopt;
        std::optional<float> border_right_width = std::nullopt;
        std::optional<float> border_top_width = std::nullopt;
        std::optional<float> border_width = std::nullopt;
        std::optional<float> bottom = std::nullopt;
        std::optional<float> column_gap = std::nullopt;
        e_display display = display_flex;
        float flex = 0.0f;
        std::optional<value_type> flex_basis = std::nullopt;
        e_flex_direction flex_direction = flex_direction_column;
        float flex_grow = 0.0f;
        float flex_shrink = 0.0f;
        e_flex_wrap flex_wrap = flex_wrap_no_wrap;
        float gap = 0.0f;
        std::optional<value_type> height = std::nullopt;
        e_justify_content justify_content = justify_content_start;
        std::optional<float> left = std::nullopt;
        float margin = 0.0f;
        std::optional<float> margin_bottom = std::nullopt;
        std::optional<float> margin_horizontal = std::nullopt;
        std::optional<float> margin_left = std::nullopt;
        std::optional<float> margin_right = std::nullopt;
        std::optional<float> margin_top = std::nullopt;
        std::optional<float> margin_vertical = std::nullopt;
        std::optional<value_type> max_height = std::nullopt;
        std::optional<value_type> max_width = std::nullopt;
        std::optional<value_type> min_height = std::nullopt;
        std::optional<value_type> min_width = std::nullopt;
        std::optional<e_overflow> overflow = overflow_visible;
        std::optional<e_overflow> overflow_x = overflow_visible;
        std::optional<e_overflow> overflow_y = overflow_visible;
        std::optional<float> padding = 0.0f;
        std::optional<float> padding_bottom = std::nullopt;
        std::optional<float> padding_horizontal = std::nullopt;
        std::optional<float> padding_left = std::nullopt;
        std::optional<float> padding_right = std::nullopt;
        std::optional<float> padding_top = std::nullopt;
        std::optional<float> padding_vertical = std::nullopt;
        e_position position = position_relative;
        std::optional<float> right = std::nullopt;
        std::optional<float> row_gap = std::nullopt;
        std::optional<float> top = std::nullopt;;
        std::optional<value_type> width = std::nullopt;
        std::optional<float> zindex = std::nullopt;

        void normalize();
    };

    struct decorative_properties {
        renderer::color_rgba background_color = {0, 0, 0, 0};
        std::optional<float> border_bottom_left_radius = std::nullopt;
        std::optional<float> border_bottom_right_radius = std::nullopt;
        renderer::color_rgba border_color = {0, 0, 0, 0};
        std::optional<float> border_radius = std::nullopt;
        std::optional<float> border_top_left_radius = std::nullopt;
        std::optional<float> border_top_right_radius = std::nullopt;
        renderer::color_rgba box_shadow_color = {0, 0, 0, 255};
        float box_shadow_offset_x = 0.0f;
        float box_shadow_offset_y = 0.0f;
        float box_shadow_radius = 0.0f;
        uint8_t opacity = 255;

        void normalize();
    };

    enum e_text_transform {
        text_transform_none,
        text_transform_capitalize,
        text_transform_lowercase,
        text_transform_uppercase
    };

    enum e_text_align {
        text_align_center,
        text_align_left,
        text_align_right
    };

    enum e_whitespace {
        whitespace_normal, // Text will wrap to the next line if it exceeds the width of the parent if defined
        whitespace_no_wrap // Text will never wrap to the next line
    };

    struct text_style_properties {
        renderer::color_rgba color;
        renderer::text_font* font;
        e_text_align align = text_align_left;
        e_text_transform transform = text_transform_none;
        e_whitespace whitespace = whitespace_normal;
    };

    // Base case for recursion: return non-optional value
    template <typename T>
    T coalesce(T value) {
        return value;
    }

    // Overload for std::optional: if value exists, return it; otherwise, recurse
    template <typename T, typename... Args>
    T coalesce(const std::optional<T>& first, Args... args) {
        return first.has_value() ? first.value() : coalesce(args...);
    }
}

#endif