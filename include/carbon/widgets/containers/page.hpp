#ifndef CARBON_WIDGETS_CONTAINERS_PAGE_HPP
#define CARBON_WIDGETS_CONTAINERS_PAGE_HPP

#include "carbon/widgets/widget.hpp"

namespace carbon {
	class page : public widget {
		friend class tab_sheet;

	public:
		page(const std::u32string& name);

		void handle_draw() override;

	protected:
		std::u32string label_;
		float animation_time_;
	};
}// namespace carbon

#endif