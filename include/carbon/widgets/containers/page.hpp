#ifndef CARBON_WIDGETS_CONTAINERS_PAGE_HPP
#define CARBON_WIDGETS_CONTAINERS_PAGE_HPP

#include "carbon/widgets/widget.hpp"

namespace carbon {
	class page : public widget {
	public:
		page(const std::wstring& name);

		void handle_draw() override;

		std::wstring get_label();

	private:
		std::wstring label_;
	};
}// namespace carbon

#endif