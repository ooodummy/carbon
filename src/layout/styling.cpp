#include "carbon/layout/styling.hpp"

namespace carbon {
    layout_properties normalize_layout_properties(const carbon::layout_properties& input) {
        layout_properties result = default_layout_properties;

        result.padding_top = coalesce(input.padding_top, input.padding_vertical, input.padding, 0.0f);
        result.padding_bottom = coalesce(input.padding_bottom, input.padding_vertical, input.padding, 0.0f);
        result.padding_left = coalesce(input.padding_left, input.padding_horizontal, input.padding, 0.0f);
        result.padding_right = coalesce(input.padding_right, input.padding_horizontal, input.padding, 0.0f);

        result.margin_top = coalesce(input.margin_top, input.margin_vertical, input.margin, 0.0f);
        result.margin_bottom = coalesce(input.margin_bottom, input.margin_vertical, input.margin, 0.0f);
        result.margin_left = coalesce(input.margin_left, input.margin_horizontal, input.margin, 0.0f);
        result.margin_right = coalesce(input.margin_right, input.margin_horizontal, input.margin, 0.0f);

        result.border_bottom_width = coalesce(input.border_bottom_width, input.border_width, 0.0f);
        result.border_top_width = coalesce(input.border_top_width, input.border_width, 0.0f);
        result.border_left_width = coalesce(input.border_left_width, input.border_width, 0.0f);
        result.border_right_width = coalesce(input.border_right_width, input.border_width, 0.0f);

        result.column_gap = coalesce(input.column_gap, input.gap, 0.0f);
        result.row_gap = coalesce(input.row_gap, input.gap, 0.0f);

        result.overflow_x = coalesce(input.overflow_x, input.overflow, overflow_visible);
        result.overflow_y = coalesce(input.overflow_y, input.overflow, overflow_visible);

        return result;
    }

    decorative_properties normalize_decorative_properties(const decorative_properties& input) {
        decorative_properties result = default_decorative_properties;

        result.border_top_left_radius = coalesce(input.border_top_left_radius, input.border_radius, 0.0f);
        result.border_top_right_radius = coalesce(input.border_top_right_radius, input.border_radius, 0.0f);
        result.border_bottom_left_radius = coalesce(input.border_bottom_left_radius, input.border_radius, 0.0f);
        result.border_bottom_right_radius = coalesce(input.border_bottom_right_radius, input.border_radius, 0.0f);

        return result;
    }
}