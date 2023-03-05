#ifndef CARBON_WIDGETS_CONTROLS_TEXTBOX_HPP
#define CARBON_WIDGETS_CONTROLS_TEXTBOX_HPP

#include "carbon/widgets/widget.hpp"

#include <codecvt>
#include <locale>
#include <string>

namespace carbon {
	class textbox : public widget {
	public:
		textbox(const std::wstring& name,
				const std::wstring& placeholder,
				size_t max_length,
				std::string* value,
				bool hidden = false);

		void handle_draw() override;

	private:
		std::wstring name_;
		std::wstring placeholder_;
		size_t max_length_;
		bool hidden_;

		std::string* value_;
	};
}// namespace carbon

#endif