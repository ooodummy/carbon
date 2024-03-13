#ifndef CARBON_WIDGETS_CONTROLS_SEPARATOR_HPP
#define CARBON_WIDGETS_CONTROLS_SEPARATOR_HPP

#include "carbon/widgets/widget.hpp"

namespace carbon {
	class separator : public widget {
	public:
		separator();

		void handle_draw() override;
	};
}// namespace carbon

#endif