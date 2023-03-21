#ifndef CARBON_WIDGETS_CONTAINERS_TAB_SHEET_HPP
#define CARBON_WIDGETS_CONTAINERS_TAB_SHEET_HPP

#include "carbon/widgets/containers/page.hpp"
#include "carbon/widgets/widget.hpp"

namespace carbon {
	// TODO: Inherit from this to style sub tab sheet
	class tab_sheet : public widget {
	public:
		tab_sheet();

		void handle_draw() override;
		void handle_input() override;

		std::shared_ptr<page> add_page(const std::u32string& name);

	private:
		std::shared_ptr<widget> bar_;
		std::shared_ptr<widget> content_;

		size_t selected_ = 0;
	};
}// namespace carbon

#endif