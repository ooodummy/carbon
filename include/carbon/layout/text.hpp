#ifndef CARBON_LAYOUT_TEXT_HPP
#define CARBON_LAYOUT_TEXT_HPP

#include "carbon/layout/node.hpp"

namespace carbon {
    class text : public node {
    public:
        text(const std::string& label, const layout_properties& layout, const text_style_properties& style) : label_(label), style_(style) {
            this->layout = layout;
            this->layout.normalize();
        }

        void paint() const override {
            const glm::vec2 position = {state.x - state.total_scroll_x, state.y - state.total_scroll_y};

            buf->draw_text(label_.c_str(), position, style_.color, style_.font, (renderer::text_flags)(renderer::text_flags::dropshadow_text | renderer::text_flags::align_horizontal));
        }

    private:
        std::string label_;
        text_style_properties style_;
    };
}

#endif