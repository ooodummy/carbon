#ifndef CARBON_WIDGETS_CONTAINERS_GROUPBOX_HPP
#define CARBON_WIDGETS_CONTAINERS_GROUPBOX_HPP

#include "carbon/widgets/controls/separator.hpp"

namespace carbon {
	// TODO: Scrolling support
	class groupbox : public widget {
	public:
		groupbox(const std::wstring& name);

		void handle_draw() override;

	private:
		std::wstring label_;
	};
}// namespace carbon

#endif