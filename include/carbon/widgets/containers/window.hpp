#ifndef CARBON_WIDGETS_CONTAINERS_WINDOW_HPP
#define CARBON_WIDGETS_CONTAINERS_WINDOW_HPP

#include "carbon/widgets/containers/groupbox.hpp"
#include "carbon/widgets/containers/page.hpp"
#include "carbon/widgets/containers/tab_sheet.hpp"
#include "carbon/widgets/controls/button.hpp"
#include "carbon/widgets/widget.hpp"

namespace carbon {
	class window : public widget {
	public:
		window(const std::wstring& name);

		void handle_draw() override;
		void handle_input() override;

		std::shared_ptr<page> add_page(const std::wstring& name);

	private:
		std::shared_ptr<widget> title_bar_;
		std::shared_ptr<widget> exit_button_;

		std::wstring title_;

		std::shared_ptr<tab_sheet> tab_sheet_;
		std::shared_ptr<widget> area_;

		bool dragging_title_bar_ = false;
		glm::vec2 drag_offset_;

		bool resizing_ = false;
	};
}// namespace carbon

#endif