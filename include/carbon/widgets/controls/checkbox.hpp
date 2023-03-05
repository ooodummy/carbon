#ifndef CARBON_WIDGETS_CONTROLS_CHECKBOX_HPP
#define CARBON_WIDGETS_CONTROLS_CHECKBOX_HPP

#include "carbon/widgets/widget.hpp"

namespace carbon {
	class checkbox : public widget {
	public:
		checkbox(const std::wstring& name, bool* value);

		bool is_hovered() const override;

		void handle_draw() override;
		void handle_input() override;

	private:
		bool* value_;

		std::wstring label_;
	};
}// namespace carbon

#endif