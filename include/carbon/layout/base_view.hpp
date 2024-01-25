#ifndef CARBON_LAYOUT_BASE_VIEW_HPP
#define CARBON_LAYOUT_BASE_VIEW_HPP

#include "carbon/layout/node.hpp"

namespace carbon {
    class base_view : public node {
    public:
        explicit base_view(const layout_properties& layout, const decorative_properties& style, const std::string& test_id = "");

        void paint() const override;

        std::shared_ptr<node> add(const std::shared_ptr<node>& child);

        template<typename T, typename... Args>
        std::shared_ptr<T> add(Args&&... args) {
            auto child = std::make_shared<T>(std::forward<Args>(args)...);
            return std::static_pointer_cast<T>(add(child));
        }

        void remove(const std::shared_ptr<node>& child);

    protected:
        decorative_properties style;
    };
}

#endif