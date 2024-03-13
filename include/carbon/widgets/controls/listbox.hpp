#ifndef CARBON_WIDGETS_CONTROLS_LISTBOX_HPP
#define CARBON_WIDGETS_CONTROLS_LISTBOX_HPP

#include "carbon/widgets/widget.hpp"

namespace carbon {
	class listbox : public widget {
	public:
		listbox(std::wstring name, const std::vector<std::wstring>& items, int* selected) : widget(), name_(name),
		items_(items), selected_(selected) {
			// TODO: Layout
		}

	private:
		std::wstring name_;
		std::vector<std::wstring> items_;
		int* selected_;

		std::vector<float> animation_times_;
	};
}

#endif