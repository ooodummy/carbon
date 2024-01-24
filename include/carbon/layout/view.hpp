#ifndef CARBON_LAYOUT_VIEW_HPP
#define CARBON_LAYOUT_VIEW_HPP

#include "carbon/layout/base_view.hpp"

namespace carbon {
    class view : public base_view {
    public:
        view(const layout_properties& layout, const std::string& test_id = "") : base_view(layout, test_id) {}

    private:
        bool is_mouse_over_;

        struct {
            bool x_active;
            bool x_hovered;
            bool y_active;
            bool y_hovered;
        } scrolling_;
    };
}

#endif