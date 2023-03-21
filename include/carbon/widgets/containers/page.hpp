#ifndef CARBON_WIDGETS_CONTAINERS_PAGE_HPP
#define CARBON_WIDGETS_CONTAINERS_PAGE_HPP

#include "carbon/widgets/widget.hpp"

#include "carbon/widgets/containers/groupbox.hpp"

namespace carbon {
	// https://github.com/facebook/yoga/issues/623
	class page : public widget {
		friend class tab_sheet;

	public:
		page(const std::u32string& name);

		void draw() override;

		std::shared_ptr<groupbox> add_groupbox(const std::wstring& name);

	protected:
		std::shared_ptr<widget> inner_container_node_;

		std::u32string label_;
		float animation_time_;
	};
}// namespace carbon

#endif