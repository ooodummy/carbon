#ifndef CARBON_WIDGETS_BUTTON_HPP
#define CARBON_WIDGETS_BUTTON_HPP

#include "carbon/layout/base_view.hpp"
#include "carbon/layout/text.hpp"

#include <functional>
#include <utility>

namespace carbon {
    class button : public base_view {
    public:
        button(const std::string& label, std::function<void()> on_click_callback, layout_properties layout, decorative_properties style, text_style_properties text_style) : base_view(layout, style), label_(label), on_click_callback_(std::move(on_click_callback)) {
            this->add<text>(label, layout_properties{.align_self = align_self_center, .justify_content = justify_content_center}, text_style);
        }

    private:
        std::string label_;
        std::function<void()> on_click_callback_;
    };
}

#endif