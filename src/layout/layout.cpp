#include "carbon/layout/layout.hpp"

namespace carbon {
    void apply_min_max_and_aspect_ratio(const std::shared_ptr<node>& node);

    void layout(const std::shared_ptr<node>& tree, glm::vec2 root_size) {
        std::deque<std::shared_ptr<node>> traversal_queue;

        const auto root = std::make_shared<base_view>(layout_properties {.height = value_type {root_size.y, false}, .width = value_type {root_size.x, false}}, decorative_properties{}, "layout##root");
        root->add(tree);

        std::vector<std::shared_ptr<node>> nodes_in_level_order = {root};

        // Note: This pass is correct
        // Traverse the tree in level order and generate the reverse queue
        traversal_queue.push_back(root);
        while (!traversal_queue.empty()) {
            const auto e = traversal_queue.front();
            traversal_queue.pop_front();

            nodes_in_level_order.push_back(e);

            const auto is_horizontal = e->parent != nullptr && (e->parent->layout.flex_direction == flex_direction_row ||
                                                                e->parent->layout.flex_direction == flex_direction_row_reverse);

            auto c = e->first_child;
            while (c != nullptr) {
                traversal_queue.push_back(c);
                c = c->next;
            }

            // Reset output layout state
            e->state.x = 0.0f;
            e->state.y = 0.0f;
            e->state.client_width = 0.0f;
            e->state.client_height = 0.0f;
            e->state.scroll_width = 0.0f;
            e->state.scroll_height = 0.0f;
            e->state.scroll_x = 0.0f;
            e->state.scroll_y = 0.0f;
            e->state.has_horizontal_scrollbar = e->layout.overflow_x == overflow_scroll;
            e->state.has_vertical_scrollbar = e->layout.overflow_y == overflow_scroll;

            if (e->layout.width.has_value() && !e->layout.width->percentage)
                e->state.client_width = e->layout.width.value().value;
            if (e->layout.height.has_value() && !e->layout.height->percentage)
                e->state.client_height = e->layout.height.value().value;

            if (e->layout.flex_basis.has_value() && !e->layout.flex_basis->percentage) {
                if (is_horizontal) {
                    e->state.client_width = e->layout.flex_basis.value().value;
                } else {
                    e->state.client_height = e->layout.flex_basis.value().value;
                }
            }

            // Resolve percentage scales
            if (e->layout.width.has_value() && e->layout.width->percentage) {
                std::optional<float> defined_width = std::nullopt;
                float accumulated_multiplier = 1.0f;

                auto p = e->parent;
                while (!defined_width.has_value() && p != nullptr) {
                    if (p->layout.width.has_value()) {
                        if (p->layout.width.value().percentage) {
                            accumulated_multiplier *= p->layout.width.value().value;
                        } else {
                            defined_width = p->layout.width.value().value;
                        }
                    }

                    p = p->parent;
                }

                e->state.client_width = e->layout.width.value().value * accumulated_multiplier * coalesce(defined_width, 0.0f);
            }

            if (e->layout.height.has_value() && e->layout.height->percentage) {
                std::optional<float> defined_height = std::nullopt;
                float accumulated_multiplier = 1.0f;

                auto p = e->parent;
                while (!defined_height.has_value() && p != nullptr) {
                    if (p->layout.height.has_value()) {
                        if (p->layout.height.value().percentage) {
                            accumulated_multiplier *= p->layout.height.value().value;
                        } else {
                            defined_height = p->layout.height.value().value;
                        }
                    }

                    p = p->parent;
                }

                e->state.client_height = e->layout.height.value().value * accumulated_multiplier * coalesce(defined_height, 0.0f);
            }

            // Resolve flex-basis
            if (e->layout.flex_basis.has_value() && e->layout.flex_basis.value().percentage) {
                if (is_horizontal) {
                    e->state.client_width = e->layout.flex_basis.value().value * e->parent->state.client_width;
                } else {
                    e->state.client_height = e->layout.flex_basis.value().value * e->parent->state.client_height;
                }
            }

            // TODO: Resolve text size
        }

        // Second tree pass, resolve wrapping children.
        // Going bottom-up level order.
        for (int i = nodes_in_level_order.size() - 1; i >= 0; i--) {
            const auto e = nodes_in_level_order[i];

            const auto is_wrap = e->layout.flex_wrap == flex_wrap_wrap || e->layout.flex_wrap == flex_wrap_wrap_reverse;
            const auto is_horizontal = e->layout.flex_direction == flex_direction_row ||
                                       e->layout.flex_direction == flex_direction_row_reverse;
            const auto is_vertical = e->layout.flex_direction == flex_direction_column ||
                                     e->layout.flex_direction == flex_direction_column_reverse;
            const auto is_justify_space = e->layout.justify_content == justify_content_space_between ||
                                          e->layout.justify_content == justify_content_space_around ||
                                          e->layout.justify_content == justify_content_space_evenly;

            // Width is at least the sum of the children with defined widths
            if (!e->layout.width.has_value()) {
                size_t children_count;
                auto c = e->first_child;
                while (c != nullptr) {
                    if (c->state.client_width > 0.0f) {
                        if (is_horizontal && c->layout.position == position_relative) {
                            // Padding is inside the width
                            e->state.client_width += c->state.client_width + c->layout.margin_left.value() + c->layout.margin_right.value();
                        }
                        if (is_vertical && c->layout.position == position_relative) {
                            // Column layout only warps the widest child
                            e->state.client_width = std::max(e->state.client_width, c->state.client_width + c->layout.margin_left.value() + c->layout.margin_right.value());
                        }
                    }

                    if (c->layout.position == position_relative) {
                        children_count++;
                    }

                    c = c->next;
                }

                e->state.client_width += e->layout.padding_left.value() +
                                         e->layout.padding_right.value() +
                                         e->layout.border_left_width.value() +
                                         e->layout.border_right_width.value();

                if (is_horizontal) {
                    e->state.client_width += ((float)children_count - 1.0f) * e->layout.row_gap.value();
                }
            }

            // Height is at least the sum of the children with defined heights
            if (!e->layout.height.has_value()) {
                size_t children_count;
                auto c = e->first_child;
                while (c != nullptr) {
                    if (c->state.client_height > 0.0f) {
                        if (is_horizontal && c->layout.position == position_relative) {
                            e->state.client_height += c->state.client_height + c->layout.margin_top.value() + c->layout.margin_bottom.value();
                        }
                        if (is_vertical && c->layout.position == position_relative) {
                            e->state.client_height = std::max(e->state.client_height, c->state.client_height + c->layout.margin_top.value() + c->layout.margin_bottom.value());
                        }
                    }

                    if (c->layout.position == position_relative) {
                        children_count++;
                    }

                    c = c->next;
                }

                e->state.client_height += e->layout.padding_top.value() +
                                         e->layout.padding_bottom.value() +
                                         e->layout.border_top_width.value() +
                                         e->layout.border_bottom_width.value();

                if (is_vertical) {
                    e->state.client_height += ((float)children_count - 1.0f) * e->layout.column_gap.value();
                }
            }

            // The size that was first calculated is the size of the tallest child plus padding
            if (is_wrap) {
                if (is_horizontal && !e->layout.height.has_value()) {
                    e->state.client_height = e->layout.padding_top.value() +
                                              e->layout.padding_bottom.value() +
                                              e->layout.border_top_width.value() +
                                              e->layout.border_bottom_width.value();
                }
                if (is_vertical && !e->layout.width.has_value()) {
                    e->state.client_width = e->layout.padding_left.value() +
                                             e->layout.padding_right.value() +
                                             e->layout.border_left_width.value() +
                                             e->layout.border_right_width.value();
                }
            }

            // Prepare rows
            std::vector<std::vector<std::shared_ptr<node>>> rows = {{}};
            float main = 0.0f;
            float cross = 0.0f;
            float longest_child_size = 0.0f;

            auto c = e->first_child;
            while (c != nullptr) {
                if (c->layout.position != position_relative || c->layout.display == display_none) {
                    c = c->next;
                    continue;
                }

                const float delta_main = is_horizontal ? (c->state.client_width + c->layout.margin_left.value() + c->layout.margin_right.value()) + (is_justify_space ? e->layout.row_gap.value() : 0.0f) :
                             (c->state.client_height + c->layout.margin_top.value() + c->layout.margin_bottom.value()) + (is_justify_space ? e->layout.column_gap.value() : 0.0f);
                const float parent_main = is_horizontal ? (e->state.client_width - e->layout.padding_left.value() - e->layout.padding_right.value() - e->layout.border_left_width.value() - e->layout.border_right_width.value()) :
                               (e->state.client_height - e->layout.padding_top.value() - e->layout.padding_bottom.value() - e->layout.border_top_width.value() - e->layout.border_bottom_width.value());

                if (is_wrap && main + delta_main > parent_main) {
                    float length = longest_child_size;
                    length += is_horizontal ? e->layout.column_gap.value() : e->layout.row_gap.value();
                    longest_child_size = 0.0f;

                    rows.emplace_back();

                    if (is_wrap) {
                        if (is_horizontal && !e->layout.height.has_value()) {
                            e->state.client_height += length;
                        }
                        if (is_vertical && !e->layout.width.has_value()) {
                            e->state.client_width += length;
                        }
                    }

                    main = 0.0f;
                    cross += length;
                }

                main += delta_main;

                // Keep track of the longest child in the flex container for the purpose of wrapping
                longest_child_size = std::max(longest_child_size, is_horizontal ? c->state.client_height : c->state.client_width);

                rows.back().push_back(c);
                c = c->next;
            }

            e->state.children = rows;

            // The last row
            if (is_wrap) {
                if (is_horizontal && !e->layout.height.has_value()) {
                    e->state.client_height += longest_child_size;
                }
                if (is_vertical && !e->layout.width.has_value()) {
                    e->state.client_width += longest_child_size;
                }
            }
        }

        // Note: This one probably has the bug
        // Third tree pass resolve flex, going top-down level order
        for (const auto& e : nodes_in_level_order) {
            const auto p = e->parent;

            if (e->layout.flex < 0.0f) {
                e->layout.flex = 0.0f;
            }

            const float parent_width = p != nullptr ? p->state.client_width : 0.0f;
            const float parent_height = p != nullptr ? p->state.client_height  : 0.0f;

            const auto direction = e->layout.flex_direction;
            const auto is_horizontal = direction == flex_direction_row || direction == flex_direction_row_reverse;
            const auto is_vertical = direction == flex_direction_column || direction == flex_direction_column_reverse;
            const auto is_reversed = direction == flex_direction_row_reverse || direction == flex_direction_column_reverse;
            const auto is_justify_space = e->layout.justify_content == justify_content_space_between ||
                                          e->layout.justify_content == justify_content_space_around ||
                                          e->layout.justify_content == justify_content_space_evenly;
            const auto is_content_space = e->layout.align_content == align_content_space_between ||
                                          e->layout.align_content == align_content_space_around ||
                                          e->layout.align_content == align_content_space_evenly;

            // If a parent had undefined width or height, and it's size was only calculated once children sizes were
            // added, then percentage sizing should happen now
            if (((p == nullptr) || (p != nullptr && !p->layout.width.has_value())) && e->layout.width.has_value() && e->layout.width.value().percentage) {
                e->state.client_width = e->layout.width.value().value * parent_width;
            }
            if (((p == nullptr) || (p != nullptr && !p->layout.height.has_value())) && e->layout.height.has_value() && e->layout.height.value().percentage) {
                e->state.client_height = e->layout.height.value().value * parent_height;
            }

            // If elements has both left, right offsets and no width, calculate width (analogues for height)
            if (e->layout.top.has_value() && e->layout.bottom.has_value() && !e->layout.height.has_value()) {
                e->state.y = (p != nullptr ? p->state.y : 0.0f) + e->layout.top.value();
                e->state.client_height = parent_height - e->layout.top.value() - e->layout.bottom.value();
            }
            if (e->layout.left.has_value() && e->layout.right.has_value() && !e->layout.width.has_value()) {
                e->state.x = (p != nullptr ? p->state.x : 0.0f) + e->layout.left.value();
                e->state.client_width = parent_width - e->layout.left.value() - e->layout.right.value();
            }

            // Handle absolute positioning
            if (e->layout.position == position_absolute) {
                e->state.x = (p != nullptr ? p->state.x : 0.0f);
                e->state.y = (p != nullptr ? p->state.y : 0.0f);

                if (e->layout.left.has_value()) {
                    e->state.x = e->state.x + e->layout.left.value();
                }
                else if (e->layout.right.has_value()) {
                    e->state.x = (p != nullptr ? (p->state.x + p->state.client_width) : 0.0f) - e->state.client_width - e->layout.right.value();
                }

                if (e->layout.top.has_value()) {
                    e->state.y = e->state.y + e->layout.top.value();
                }
                else if (e->layout.bottom.has_value()) {
                    e->state.y = (p != nullptr ? (p->state.y + p->state.client_height) : 0.0f) - e->state.client_height - e->layout.bottom.value();
                }
            }

            // Keep in mind scrollbars
            if (e->state.has_horizontal_scrollbar) {
                e->state.client_height -= CROSS_AXIS_SIZE;
            }
            if (e->state.has_vertical_scrollbar) {
                e->state.client_width -= CROSS_AXIS_SIZE;
            }

            if (e->layout.flex_wrap == flex_wrap_wrap_reverse) {
                std::reverse(e->state.children.begin(), e->state.children.end());
            }

            if (is_reversed) {
                for (auto& row : e->state.children) {
                    std::reverse(row.begin(), row.end());
                }
            }

            const float reset_main = is_horizontal ? e->state.x + e->layout.padding_left.value() + e->layout.border_left_width.value() :
                                     e->state.y + e->layout.padding_top.value() + e->layout.border_top_width.value();
            const float reset_cross = is_horizontal ? e->state.y + e->layout.padding_top.value() + e->layout.border_top_width.value() :
                                      e->state.x + e->layout.padding_left.value() + e->layout.border_left_width.value();

            //--------------------------------------------------------------------------------------------------------\\

            float main = reset_main;
            float cross = reset_cross;

            const float main_gap = is_horizontal ? e->layout.row_gap.value() : e->layout.column_gap.value();
            const float cross_gap = is_horizontal ? e->layout.column_gap.value() : e->layout.row_gap.value();

            std::vector<float> max_cross_children;
            std::vector<size_t> children_in_line;

            for (const auto& line : e->state.children) {
                float max_cross_child = 0.0f;
                size_t children_count = 0;

                for (const auto& c : line) {
                    if (c->layout.position != position_relative || c->layout.display == display_none) {
                        continue;
                    }

                    children_count++;
                    max_cross_child = std::max(max_cross_child, is_horizontal ? c->state.client_height : c->state.client_width);
                }

                max_cross_children.push_back(max_cross_child);
                children_in_line.push_back(children_count);
            }

            //--------------------------------------------------------------------------------------------------------\\

            // Iterate over lines
            for (size_t i = 0; i < e->state.children.size(); i++) {
                const auto line = e->state.children[i];
                const auto max_cross_child = max_cross_children[i];
                const auto children_count = children_in_line[i];

                float total_flex_grow = 0.0f;
                float total_flex_shrink = 0.0f;

                // Calculate available space for justify content along the main-axis
                float available_main = is_horizontal ? e->state.client_width - e->layout.padding_left.value() - e->layout.padding_right.value() - e->layout.border_left_width.value() - e->layout.border_right_width.value() :
                                       e->state.client_height - e->layout.padding_top.value() - e->layout.padding_bottom.value() - e->layout.border_top_width.value() - e->layout.border_bottom_width.value();

                if (!is_justify_space) {
                    available_main -= main_gap * ((float)line.size() - 1.0f);
                }

                float available_cross = is_horizontal ? e->state.client_height - e->layout.padding_top.value() - e->layout.padding_bottom.value() - e->layout.border_top_width.value() - e->layout.border_bottom_width.value() :
                                        e->state.client_width - e->layout.padding_left.value() - e->layout.padding_right.value() - e->layout.border_left_width.value() - e->layout.border_right_width.value();

                for (size_t k = 0; k < max_cross_children.size(); k++) {
                    available_cross -= max_cross_children[k];
                    if (k != max_cross_children.size() - 1 && !is_content_space) {
                        available_cross -= cross_gap;
                    }
                }

                if (e->state.has_horizontal_scrollbar && e->state.scroll_width > e->state.client_width) {
                    if (is_horizontal) {
                        available_main -= CROSS_AXIS_SIZE;
                    }
                    else {
                        available_cross -= CROSS_AXIS_SIZE;
                    }
                }

                if (e->state.has_vertical_scrollbar && e->state.scroll_height > e->state.client_height) {
                    if (is_vertical) {
                        available_main -= CROSS_AXIS_SIZE;
                    }
                    else {
                        available_cross -= CROSS_AXIS_SIZE;
                    }
                }

                for (const auto& c : line) {
                    if (c->layout.position != position_relative || c->layout.display == display_none) {
                        continue;
                    }

                    available_main -= is_horizontal ? c->state.client_width + (!is_justify_space ? c->layout.margin_left.value() + c->layout.margin_right.value() : 0.0f) :
                                      c->state.client_height + (!is_justify_space ? c->layout.margin_top.value() + c->layout.margin_bottom.value() : 0.0f);

                    if (c->layout.flex > 0.0f || c->layout.flex_grow > 0.0f) {
                        if (c->layout.flex > 0.0f) {
                            total_flex_grow += c->layout.flex;
                        }
                        else if (c->layout.flex_grow > 0.0f) {
                            total_flex_grow += c->layout.flex_grow;
                        }
                    }
                    if (c->layout.flex_shrink > 0.0f) {
                        total_flex_shrink += c->layout.flex_shrink;
                    }
                }

                // Adjust positions for justify content
                if (e->layout.justify_content == justify_content_center) {
                    // TODO: Available main/cross is useful here for skipping own size but we should ignore the border
                    //  or padding and we don't
                    main += available_main / 2.0f;
                }
                if ((is_reversed && e->layout.justify_content == justify_content_start) || (!is_reversed && e->layout.justify_content == justify_content_end)) {
                    main += available_main;
                }
                if (e->layout.justify_content == justify_content_space_around) {
                    main += available_main / (float)children_count / 2.0f;
                }
                if (e->layout.justify_content == justify_content_space_evenly) {
                    main += available_main / ((float)children_count + 1.0f);
                }

                //----------------------------------------------------------------------------------------------------\\

                // Align content
                if (e->layout.align_content == align_content_center) {
                    if (i == 0) {
                        cross += available_cross / 2.0f;
                    }
                }
                if (e->layout.align_content == align_content_end) {
                    if (i == 0) {
                        cross += available_cross;
                    }
                }
                if (e->layout.align_content == align_content_space_between) {
                    if (i > 0) {
                        cross += available_cross / ((float)max_cross_children.size() - 1.0f);
                    }
                }
                if (e->layout.align_content == align_content_space_around) {
                    const auto gap = available_cross / (float)max_cross_children.size();
                    cross += (i == 0 ? gap / 2.0f : gap);
                }
                if (e->layout.align_content == align_content_space_evenly) {
                    const auto gap = available_cross / ((float)max_cross_children.size() + 1.0f);
                    cross += gap;
                }
                if (e->layout.align_content == align_content_stretch) {
                    if (i > 0) {
                        cross += available_cross / (float)max_cross_children.size();
                    }
                }

                //----------------------------------------------------------------------------------------------------\\

                // Iterate over children and apply positions and flex sizes
                float used_main = 0.0f;
                for (size_t j = 0; j < line.size(); j++) {
                    const auto& c = line[j];

                    if (c->layout.position != position_relative || c->layout.display == display_none) {
                        continue;
                    }

                    if (!is_justify_space) {
                        if (available_main > 0.0f && (c->layout.flex > 0.0f || c->layout.flex_grow > 0.0f)) {
                            const auto flex_value = c->layout.flex != 0.0f ? c->layout.flex : c->layout.flex_grow;

                            // When splitting the available space, the last child gets the remainder
                            float size = std::round((flex_value / total_flex_grow) * available_main);
                            used_main += size;
                            if (j == line.size() - 1 && used_main < available_main) {
                                size += available_main - used_main;
                            }

                            if (is_horizontal) {
                                c->state.client_width += size;
                            }
                            else {
                                c->state.client_height += size;
                            }
                        }

                        if (available_main < 0.0f && c->layout.flex_shrink > 0.0f) {
                            // TODO: Split remainder
                            if (is_horizontal) {
                                c->state.client_width += (c->layout.flex_shrink / total_flex_shrink) * available_main;
                            }
                            else {
                                c->state.client_height += (c->layout.flex_shrink / total_flex_shrink) * available_main;
                            }
                        }
                    }

                    apply_min_max_and_aspect_ratio(c);

                    if (is_justify_space) {
                        c->state.x += is_horizontal ? main : cross;
                        c->state.y += is_horizontal ? cross : main;
                        main += is_horizontal ? c->state.client_width : c->state.client_height;

                        if (e->layout.justify_content == justify_content_space_between) {
                            main += available_main / ((float)children_count - 1.0f);
                        }
                        if (e->layout.justify_content == justify_content_space_around) {
                            main += available_main / (float)children_count;
                        }
                        if (e->layout.justify_content == justify_content_space_evenly) {
                            main += available_main / ((float)children_count + 1.0f);
                        }
                    }
                    else {
                        c->state.x += is_horizontal ? main + c->layout.margin_left.value() : cross + c->layout.margin_left.value();
                        c->state.y += is_horizontal ? cross + c->layout.margin_top.value() : main + c->layout.margin_top.value();

                        main += is_horizontal ? c->state.client_width + c->layout.margin_left.value() + c->layout.margin_right.value() :
                                              c->state.client_height + c->layout.margin_top.value() + c->layout.margin_bottom.value();
                        main += main_gap;
                    }

                    //------------------------------------------------------------------------------------------------\\

                    float line_cross_size = max_cross_child;

                    // If there's only one line, the flex container has defined height, use it as the cross-size
                    if (e->state.children.size() == 1) {
                        line_cross_size = is_horizontal ? e->state.client_height - e->layout.padding_top.value() - e->layout.padding_bottom.value() - e->layout.border_top_width.value() - e->layout.border_bottom_width.value() :
                                          e->state.client_width - e->layout.padding_left.value() - e->layout.padding_right.value() - e->layout.border_left_width.value() - e->layout.border_right_width.value();
                    }

                    // Apply align items
                    if (c->layout.align_self == align_self_auto) {
                        if (e->layout.align_items == align_items_center) {
                            if (is_horizontal) {
                                c->state.y += (line_cross_size - c->state.client_height) / 2.0f;
                            }
                            else {
                                c->state.x += (line_cross_size - c->state.client_width) / 2.0f;
                            }
                        }
                        if (e->layout.align_items == align_items_end) {
                            if (is_horizontal) {
                                c->state.y += line_cross_size - c->state.client_height;
                            }
                            else {
                                c->state.x += line_cross_size - c->state.client_width;
                            }
                        }
                        if (e->layout.align_items == align_items_stretch && ((is_horizontal && !c->layout.height.has_value()) || (is_vertical && !c->layout.width.has_value()))) {
                            if (is_horizontal) {
                                c->state.client_height = line_cross_size;
                            }
                            else {
                                c->state.client_width = line_cross_size;
                            }
                        }
                    }

                    // Apply align self
                    if (c->layout.align_self == align_self_start) {
                        if (is_horizontal) {
                            c->state.y = reset_cross;
                        }
                        else {
                            c->state.x = reset_cross;
                        }
                    }
                    if (c->layout.align_self == align_self_center) {
                        if (is_horizontal) {
                            c->state.y += (line_cross_size - c->state.client_height) / 2.0f;
                        }
                        else {
                            c->state.x += (line_cross_size - c->state.client_width) / 2.0f;
                        }
                    }
                    if (c->layout.align_self == align_self_end) {
                        if (is_horizontal) {
                            c->state.y += line_cross_size - c->state.client_height;
                        }
                        else {
                            c->state.x += line_cross_size - c->state.client_width;
                        }
                    }
                    if (c->layout.align_self == align_self_stretch && ((is_horizontal && !c->layout.height.has_value()) || (is_vertical && !c->layout.width.has_value()))) {
                        if (is_horizontal) {
                            c->state.y = reset_cross;
                            c->state.client_height = line_cross_size;
                        }
                        else {
                            c->state.x = reset_cross;
                            c->state.client_width = line_cross_size;
                        }
                    }

                    // Add left, top, right, bottom offsets
                    if (c->layout.left.has_value()) {
                        c->state.x += c->layout.left.value();
                    }
                    else if (c->layout.right.has_value()) {
                        c->state.x -= c->layout.right.value();
                    }
                    if (c->layout.top.has_value()) {
                        c->state.y += c->layout.top.value();
                    }
                    else if (c->layout.bottom.has_value()) {
                        c->state.y -= c->layout.bottom.value();
                    }
                }

                main = reset_main;
                cross += max_cross_child + cross_gap;
            }

            e->state.children = {};

            e->state.x = std::round(e->state.x);
            e->state.y = std::round(e->state.y);
            e->state.client_width = std::round(e->state.client_width);
            e->state.client_height = std::round(e->state.client_height);
        }

        // Fourth tree pass, calculate scroll sizes. It's only possible after sizes and positions are set.
        //  Going top-down, level-order.
        for (const auto& e : nodes_in_level_order) {
            if (e->layout.overflow_x == overflow_auto ||
                e->layout.overflow_x == overflow_scroll ||
                e->layout.overflow_y == overflow_auto ||
                e->layout.overflow_y == overflow_scroll) {
                float farthest_x = 0.0f;
                float farthest_y = 0.0f;

                auto c = e->first_child;
                while (c != nullptr) {
                    const auto potential_horizontal_scroll = c->state.has_horizontal_scrollbar ? CROSS_AXIS_SIZE : 0.0f;
                    const auto potential_vertical_scroll = c->state.has_vertical_scrollbar ? CROSS_AXIS_SIZE : 0.0f;
                    farthest_x = std::max(farthest_x, c->state.x + c->layout.margin_left.value() + c->state.client_width + potential_horizontal_scroll + c->layout.margin_right.value() - e->state.x);
                    farthest_y = std::max(farthest_y, c->state.y + c->layout.margin_top.value() + c->state.client_height + potential_vertical_scroll + c->layout.margin_bottom.value() - e->state.y);
                    c = c->next;
                }

                farthest_x += e->layout.padding_right.value() + e->layout.border_right_width.value();
                farthest_y += e->layout.padding_bottom.value() + e->layout.border_bottom_width.value();

                e->state.scroll_width = std::max(farthest_x, e->state.client_width);
                e->state.scroll_height = std::max(farthest_y, e->state.client_height);

                if (e->layout.overflow_x == overflow_auto && e->state.scroll_width > e->state.client_width) {
                    e->state.client_height -= CROSS_AXIS_SIZE;
                    e->state.has_horizontal_scrollbar = true;
                }
                if (e->layout.overflow_y == overflow_auto && e->state.scroll_height > e->state.client_height) {
                    e->state.client_width -= CROSS_AXIS_SIZE;
                    e->state.has_vertical_scrollbar = true;
                }
            }
            else {
                e->state.scroll_width = e->state.client_width;
                e->state.scroll_height = e->state.client_height;
            }
        }
    }

    void apply_min_max_and_aspect_ratio(const std::shared_ptr<node>& e) {
        float min_height = 0.0f;
        float min_width = 0.0f;
        float max_height = std::numeric_limits<float>::max();
        float max_width = std::numeric_limits<float>::max();

        if (e->layout.min_height.has_value()) {
            min_height = e->layout.min_height.value().percentage ? e->layout.min_height.value().value * (e->parent != nullptr ? e->parent->state.client_height : 0.0f) : e->layout.min_height.value().value;
        }
        if (e->layout.min_width.has_value()) {
            min_width = e->layout.min_width.value().percentage ? e->layout.min_width.value().value * (e->parent != nullptr ? e->parent->state.client_width : 0.0f) : e->layout.min_width.value().value;
        }
        if (e->layout.max_height.has_value()) {
            max_height = e->layout.max_height.value().percentage ? e->layout.max_height.value().value * (e->parent != nullptr ? e->parent->state.client_height : 0.0f) : e->layout.max_height.value().value;
        }
        if (e->layout.max_width.has_value()) {
            max_width = e->layout.max_width.value().percentage ? e->layout.max_width.value().value * (e->parent != nullptr ? e->parent->state.client_width : 0.0f) : e->layout.max_width.value().value;
        }

        float effective_width = std::min(std::max(e->state.client_width, min_width), max_width);
        float effective_height = std::min(std::max(e->state.client_height, min_height), max_height);

        const bool is_horizontal = e->parent != nullptr && (e->parent->layout.flex_direction == flex_direction_row || e->parent->layout.flex_direction == flex_direction_row_reverse);

        if (e->layout.aspect_ratio.has_value()) {
            const auto aspect_ratio = e->layout.aspect_ratio.value();
            if ((!e->layout.width.has_value() || min_width > 0.0f) && !e->layout.height.has_value()) {
                const auto calculated_height = effective_width / aspect_ratio;
                effective_height = std::min(std::max(calculated_height, min_height), max_height);
            }
            else if ((!e->layout.height.has_value() || min_height > 0.0f) && !e->layout.width.has_value()) {
                const auto calculated_width = effective_height * aspect_ratio;
                effective_width = std::min(std::max(calculated_width, min_width), max_width);
            }
            else if (!e->layout.width.has_value() && !e->layout.height.has_value()) {
                // // If both width and height are undefined
                if (is_horizontal) {
                    effective_height = std::min(std::max(effective_width / aspect_ratio, min_height), max_height);
                }
                else {
                    effective_width = std::min(std::max(effective_height * aspect_ratio, min_width), max_width);
                }
            }
            else {
                // Both width and height are defined
                if (is_horizontal) {
                    effective_height = effective_width / aspect_ratio;
                }
                else {
                    effective_width = effective_height * aspect_ratio;
                }

                effective_width = std::min(std::max(effective_width, min_width), max_width);
                effective_height = std::min(std::max(effective_height, min_height), max_height);
            }
        }

        e->state.client_width = effective_width;
        e->state.client_height = effective_height;
    }
}