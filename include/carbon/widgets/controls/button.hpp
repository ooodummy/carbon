#ifndef CARBON_WIDGETS_CONTROLS_BUTTON_HPP
#define CARBON_WIDGETS_CONTROLS_BUTTON_HPP

#include "carbon/widgets/widget.hpp"

namespace carbon {
	class button : public widget {
	public:
		button(const std::wstring& name, const std::function<void()>& callback);

		bool is_hovered() const override;

		void handle_draw() override;
		void handle_input() override;

		void set_callback(const std::function<void()>& callback);

	private:
		std::wstring label_;

		std::function<void()> callback_;
	};
}// namespace carbon

#endif