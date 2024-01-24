#include "carbon/layout/styling.hpp"

namespace carbon {
    void layout_properties::normalize() {
        padding_top = coalesce(padding_top, padding_vertical, padding, 0.0f);
        padding_bottom = coalesce(padding_bottom, padding_vertical, padding, 0.0f);
        padding_left = coalesce(padding_left, padding_horizontal, padding, 0.0f);
        padding_right = coalesce(padding_right, padding_horizontal, padding, 0.0f);

        margin_top = coalesce(margin_top, margin_vertical, margin);
        margin_bottom = coalesce(margin_bottom, margin_vertical, margin);
        margin_left = coalesce(margin_left, margin_horizontal, margin);
        margin_right = coalesce(margin_right, margin_horizontal, margin);

        border_bottom_width = coalesce(border_bottom_width, border_width, 0.0f);
        border_top_width = coalesce(border_top_width, border_width, 0.0f);
        border_left_width = coalesce(border_left_width, border_width, 0.0f);
        border_right_width = coalesce(border_right_width, border_width, 0.0f);

        column_gap = coalesce(column_gap, gap);
        row_gap = coalesce(row_gap, gap);

        overflow_x = coalesce(overflow_x, overflow, overflow_visible);
        overflow_y = coalesce(overflow_y, overflow, overflow_visible);
    }

    void decorative_properties::normalize() {
        border_top_left_radius = coalesce(border_top_left_radius, border_radius, 0.0f);
        border_top_right_radius = coalesce(border_top_right_radius, border_radius, 0.0f);
        border_bottom_left_radius = coalesce(border_bottom_left_radius, border_radius, 0.0f);
        border_bottom_right_radius = coalesce(border_bottom_right_radius, border_radius, 0.0f);
    }
}