#ifndef CARBON_LAYOUT_VIEW_HPP
#define CARBON_LAYOUT_VIEW_HPP

#include "carbon/constants.hpp"
#include "carbon/layout/base_view.hpp"

namespace carbon {
    class view : public base_view {
    public:
        view(const layout_properties& layout, const decorative_properties& style, const std::string& test_id = "") : base_view(layout, style, test_id) {}

        void paint() const override;

    private:
        bool is_mouse_over_{};

        struct {
            bool x_active;
            bool x_hovered;
            bool y_active;
            bool y_hovered;
        } scrolling_{};
    };
}

#endif