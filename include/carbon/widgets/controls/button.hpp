#ifndef CARBON_WIDGETS_CONTROLS_BUTTON_HPP
#define CARBON_WIDGETS_CONTROLS_BUTTON_HPP

#include "carbon/widgets/widget.hpp"

namespace carbon {
	class button : public widget {
	public:
		button(const std::wstring& name, const std::function<void()>& callback);

		void handle_draw() override;
		void handle_input() override;

	private:
		std::wstring label_;
		std::function<void()> callback_;

		bool held_ = false;
		float animation_time_;
	};
}// namespace carbon

#endif