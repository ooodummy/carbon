#ifndef CARBON_INPUT_HPP
#define CARBON_INPUT_HPP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <glm/glm.hpp>

namespace carbon {
	enum side {
		top = 1 << 0,
		left = 1 << 1,
		right = 1 << 3,
		bottom = 1 << 4,
		vertical = top | bottom,
		horizontal = left | right,
		all = vertical | horizontal
	};

	void input_end();

	[[nodiscard]] glm::vec2 get_mouse_pos();

	[[nodiscard]] bool is_point_in_bounds(const glm::vec2& point, const glm::vec4& bounds);
	[[nodiscard]] bool is_bounds_in_bounds(const glm::vec4& inner, const glm::vec4& outer);
	[[nodiscard]] bool is_bounds_intersecting(const glm::vec4& a, const glm::vec4& b);
	[[nodiscard]] size_t get_nearest_side(const glm::vec4& bounds, const glm::vec2& point, side sides = all);

	[[nodiscard]] bool is_mouse_over(const glm::vec4& bounds);

	[[nodiscard]] bool is_key_down(uint32_t key);
	[[nodiscard]] bool is_key_pressed(uint32_t key);
	[[nodiscard]] bool is_key_released(uint32_t key);
	[[nodiscard]] uint32_t get_last_key_down();

	LRESULT impl_win32_winproc_handler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}// namespace carbon

#endif