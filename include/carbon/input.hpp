#ifndef CARBON_INPUT_HPP
#define CARBON_INPUT_HPP

#include <glm/glm.hpp>
#include <functional>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace carbon {
    // Execute on the end of frames where input functions are only used in one thread, so we can properly reset the input state and check if keys have been released or pressed
    void input_end();

    // Input processing code for basic input handling
    [[nodiscard]] glm::vec2 get_mouse_pos();
    [[nodiscard]] bool is_mouse_over(const glm::vec4& bounds);

    [[nodiscard]] bool is_key_down(uint32_t key);
    [[nodiscard]] bool is_key_pressed(uint32_t key);
    [[nodiscard]] bool is_key_released(uint32_t key);

    [[nodiscard]] uint32_t get_last_key();

    LRESULT impl_win32_winproc_handler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    struct hotkey {
        uint32_t key;
        bool* value;
    };

    // Used for multi key press shortcut
    struct chorded_hotkey {
        std::vector<uint32_t> mods;
        std::vector<uint32_t> keys;

        std::function<void()> callback;
    };

    class hotkey_manager {
    public:
        void add_chord(const chorded_hotkey& c);
        void process_chords();

    private:
        std::vector<hotkey> hotkeys_;
        std::vector<chorded_hotkey> chords_;
    };
}// namespace carbon

#endif