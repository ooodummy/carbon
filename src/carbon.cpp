#include "carbon/carbon.hpp"

void carbon::initialize() {
    init_default_theme();
}

void carbon::begin() {
    benchmark.flex_compute_calls = 0;
}

void carbon::end() {
    benchmark.performance_counter.tick();

    input_end();
}

void carbon::debug_info() {
    float offset = 0.0f;
    const auto print = [&offset](const std::string& text) -> void {
        buf->draw_text(text, {10.0f, 10.0f + offset}, COLOR_WHITE, main_font, renderer::dropshadow_text);
        offset += main_font->size + 4.0f;
    };

    print(std::format("FPS: {}", benchmark.performance_counter.get_fps()));
    print(std::format("FT: {}", benchmark.performance_counter.get_dt()));
    print(std::format("Flex compute: {}", benchmark.flex_compute_calls));
}
