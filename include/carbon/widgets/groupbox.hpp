#ifndef CARBON_WIDGETS_GROUPBOX_HPP
#define CARBON_WIDGETS_GROUPBOX_HPP

#include "carbon/layout/base_view.hpp"

namespace carbon {
    class groupbox : public base_view {
    public:
        groupbox(const std::string& label, const layout_properties& layout) : base_view(layout, decorative_properties{
                                                                                                        .border_radius = 4.0f
                                                                                                    }, test_id) {
        }
    };
}

#endif