#ifndef CARBON_LAYOUT_STYLING_HPP
#define CARBON_LAYOUT_STYLING_HPP

#include <renderer/color.hpp>
#include <glm/glm.hpp>

#include <memory>
#include <optional>
#include <vector>

namespace carbon {
    class layout_node;

    struct layout_node_state {
        std::vector<std::vector<std::shared_ptr<layout_node>>> children;
        float client_height;
        float client_width;
        glm::vec2 clip_size;
        glm::vec2 clip_start;
        bool has_horizontal_scrollbar;
        bool has_vertical_scrollbar;
        float scroll_height;
        float scroll_width;
        float scroll_x;
        float scroll_y;
        std::optional<float> text_width_limit;
        float total_scroll_x;
        float total_scroll_y;
        float x;
        float y;
    };

    static const layout_node_state default_layout_node_state {
        .children = {},
        .client_height = 0.0f,
        .client_width = 0.0f,
        .clip_size = { 0.0f, 0.0f },
        .clip_start = { 0.0f, 0.0f },
        .has_horizontal_scrollbar = false,
        .has_vertical_scrollbar = false,
        .scroll_height = 0.0f,
        .scroll_width = 0.0f,
        .scroll_x = 0.0f,
        .scroll_y = 0.0f,
        .text_width_limit = std::nullopt,
        .total_scroll_x = 0.0f,
        .total_scroll_y = 0.0f,
        .x = 0.0f,
        .y = 0.0f
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
        e_align_content align_content;
        e_align_items align_items;
        e_align_self align_self;
        std::optional<float> aspect_ratio;
        std::optional<float> border_bottom_width;
        std::optional<float> border_left_width;
        std::optional<float> border_right_width;
        std::optional<float> border_top_width;
        std::optional<float> border_width;
        std::optional<float> bottom;
        std::optional<float> column_gap;
        e_display display;
        float flex;
        std::optional<value_type> flex_basis;
        e_flex_direction flex_direction;
        float flex_grow;
        float flex_shrink;
        float flex_wrap;
        std::optional<float> gap;
        std::optional<value_type> height;
        e_justify_content justify_content;
        std::optional<float> left;
        std::optional<float> margin;
        std::optional<float> margin_bottom;
        std::optional<float> margin_horizontal;
        std::optional<float> margin_left;
        std::optional<float> margin_right;
        std::optional<float> margin_top;
        std::optional<float> margin_vertical;
        std::optional<float> max_height;
        std::optional<float> max_width;
        std::optional<float> min_height;
        std::optional<float> min_width;
        std::optional<e_overflow> overflow;
        std::optional<e_overflow> overflow_x;
        std::optional<e_overflow> overflow_y;
        std::optional<float> padding;
        std::optional<float> padding_bottom;
        std::optional<float> padding_horizontal;
        std::optional<float> padding_left;
        std::optional<float> padding_right;
        std::optional<float> padding_top;
        std::optional<float> padding_vertical;
        e_position position;
        std::optional<float> right;
        std::optional<float> row_gap;
        std::optional<float> top;
        std::optional<value_type> width;
        std::optional<float> zindex;
    };

    static const layout_properties default_layout_properties {
        .align_content = align_content_start,
        .align_items = align_items_start,
        .align_self = align_self_auto,
        .aspect_ratio = std::nullopt,
        .border_bottom_width = 0.0f,
        .border_left_width = 0.0f,
        .border_right_width = 0.0f,
        .border_top_width = 0.0f,
        .border_width = 0.0f,
        .bottom = std::nullopt,
        .column_gap = 0.0f,
        .display = display_flex,
        .flex = 0.0f,
        .flex_basis = std::nullopt,
        .flex_direction = flex_direction_column,
        .flex_grow = 0.0f,
        .flex_shrink = 0.0f,
        .flex_wrap = flex_wrap_no_wrap,
        .gap = 0.0f,
        .height = std::nullopt,
        .justify_content = justify_content_start,
        .left = std::nullopt,
        .margin_bottom = 0.0f,
        .margin_left = 0.0f,
        .margin_right = 0.0f,
        .margin_top = 0.0f,
        .max_height = std::nullopt,
        .max_width = std::nullopt,
        .min_height = std::nullopt,
        .min_width = std::nullopt,
        .overflow_x = overflow_visible,
        .overflow_y = overflow_visible,
        .padding_bottom = 0.0f,
        .padding_left = 0.0f,
        .padding_right = 0.0f,
        .padding_top = 0.0f,
        .position = position_relative,
        .right = std::nullopt,
        .row_gap = 0.0f,
        .top = std::nullopt,
        .width = std::nullopt,
        .zindex = std::nullopt
    };

    struct decorative_properties {
        renderer::color_rgba background_color;
        std::optional<float> border_bottom_left_radius;
        std::optional<float> border_bottom_right_radius;
        renderer::color_rgba border_color;
        std::optional<float> border_radius;
        std::optional<float> border_top_left_radius;
        std::optional<float> border_top_right_radius;
        renderer::color_rgba box_shadow_color;
        float box_shadow_offset_x;
        float box_shadow_offset_y;
        float box_shadow_radius;
        uint8_t opacity;
    };

    constexpr decorative_properties default_decorative_properties {
        .background_color = {0, 0, 0, 0},
        .border_bottom_left_radius = 0.0f,
        .border_bottom_right_radius = 0.0f,
        .border_color = {0, 0, 0, 0},
        .border_top_left_radius = 0.0f,
        .border_top_right_radius = 0.0f,
        .box_shadow_color = {0, 0, 0, 255},
        .box_shadow_offset_x = 0.0f,
        .box_shadow_offset_y = 0.0f,
        .box_shadow_radius = 0.0f,
        .opacity = 255
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

    // TODO: Probably need to normalize more properties we can also probably remove some conditions by having the last
    //  case situation default to zero by default and jut set it up in the struct initializer
    layout_properties normalize_layout_properties(const layout_properties& input);

    decorative_properties normalize_decorative_properties(const decorative_properties& input);
}

#endif