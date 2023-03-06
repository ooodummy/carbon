#ifndef CARBON_WIDGETS_DROPDOWN_HPP
#define CARBON_WIDGETS_DROPDOWN_HPP

#include "carbon/widgets/widget.hpp"

namespace carbon {
	class dropdown : public widget {
	public:
		dropdown(const std::wstring& name, std::vector<std::wstring> items, int* selected);

		bool is_hovered() const override;

		void handle_draw() override;
		void handle_input() override;

	private:
		std::wstring name_;

		std::vector<std::wstring> items_;
		int* selected_;

		float animation_time_ = 0.0f;
	};
}

#endif