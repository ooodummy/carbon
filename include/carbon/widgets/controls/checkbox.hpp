#ifndef CARBON_WIDGETS_CONTROLS_CHECKBOX_HPP
#define CARBON_WIDGETS_CONTROLS_CHECKBOX_HPP

#include "carbon/widgets/widget.hpp"

namespace carbon {
	class checkbox : public widget {
	public:
		checkbox(const std::wstring& name, bool* value);

		void handle_draw() override;
		void handle_input() override;

	private:
		std::wstring label_;
		bool* value_;

		bool held_ = false;
		float animation_time_;
	};
}// namespace carbon

#endif