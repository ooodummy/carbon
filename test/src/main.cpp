#include <carbon/carbon.hpp>
#include <renderer/core.hpp>

#include <dwmapi.h>
#include <thread>

#include <glm/gtx/rotate_vector.hpp>
#include <fmt/core.h>

renderer::sync_manager updated_draw;
renderer::sync_manager updated_buf;

bool update_size = false;
bool close_requested = false;

int populate_count = 0;
int draw_count = 0;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
			close_requested = true;
            return 0;
        case WM_SIZE:
            carbon::application->set_size({LOWORD(lParam), HIWORD(lParam)});
            update_size = true;
            break;
        default:
            break;
    }

	if (carbon::impl_win32_winproc_handler(hWnd, msg, wParam, lParam))
		return 1;

    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

void draw_test_flex(renderer::buffer* buf) {
	static bool init = false;
	static auto flex_container = std::make_unique<carbon::flex_container>();

	if (!init) {
		flex_container->set_pos({50.0f, 50.0f});
		//flex_container->set_justify_content(carbon::justify_center);
		/*const auto item1 = flex_container->add_child<carbon::flex_item>();
		item1->set_basis(25.0f, carbon::unit_pixel);
		const auto item2 = flex_container->add_child<carbon::flex_item>();
		item2->set_basis(50.0f, carbon::unit_pixel);
		const auto item3 = flex_container->add_child<carbon::flex_item>();
		item3->set_basis(50.0f, carbon::unit_pixel);*/
		//const auto item4 = flex_container->add_child<carbon::flex_item>();
		//item4->set_basis(125.0f, carbon::unit_pixel);

		const auto container1 = flex_container->add_child<carbon::flex_container>();
		container1->set_flex(1.0f);
		container1->set_flow(carbon::column);
		container1->set_max_width(300.0f);
		const auto item3 = flex_container->add_child<carbon::flex_item>();
		item3->set_flex(1.0f);
		const auto container11 = container1->add_child<carbon::flex_container>();
		container11->set_flex(1.0);
		container11->set_min_width(50.0f);
		container11->set_max_width(100.0f);
		const auto item111 = container11->add_child<carbon::flex_item>();
		item111->set_flex(1.0f);
		const auto item112 = container11->add_child<carbon::flex_item>();
		item112->set_flex(1.0f);
		const auto item113 = container11->add_child<carbon::flex_item>();
		item113->set_flex(1.0f);
		const auto container12 = container1->add_child<carbon::flex_container>();
		container12->set_flex(1.0f);
		container12->set_flow(carbon::column);
		const auto item121 = container12->add_child<carbon::flex_item>();
		item121->set_flex(1.0f);
		const auto item122 = container12->add_child<carbon::flex_item>();
		item122->set_flex(1.0f);
		container12->set_max_width(100.0f);
		auto container2 = container1->add_child<carbon::flex_container>();
		container2->set_flex(1.0f);
		container2->set_min_width(150.0f);

		init = true;
	}

	flex_container->set_size(carbon::get_mouse_pos() - flex_container->get_pos());
	flex_container->compute();
	flex_container->draw();
}

#include <algorithm>

void draw_test_ui(renderer::buffer* buf) {
	static auto menu = std::make_unique<carbon::window>();
	static bool init = false;
	static std::shared_ptr<carbon::label<std::string>> label = nullptr;

	if (!init) {
		auto groupbox = menu->content->add_child<carbon::groupbox<std::string>>("Test");
		label = groupbox->body->add_child<carbon::label<std::string>>("Label", carbon::segoe_font);
		groupbox->body->add_child<carbon::label<std::string>>("Hi there", carbon::segoe_font);

		auto groupbox2 = menu->content->add_child<carbon::groupbox<std::string>>("Aimbot");
		groupbox2->body->add_child<carbon::label<std::string>>("Aimbot enabled", carbon::segoe_font);
		groupbox2->body->add_child<carbon::label<std::string>>("Visibility check", carbon::segoe_font);
		groupbox2->body->add_child<carbon::button<std::string>>("Test button");

		menu->set_pos({300.0f, 300.0f});
		menu->set_size({580.0f, 500.0f});
		init = true;
	}

	static int i = 0;
	i++;
	std::string test = std::to_string(i);
	label->set_label(test);

	menu->set_size(carbon::get_mouse_pos() - menu->get_pos());

	menu->input();
	menu->compute();
	menu->draw();
}

void draw_input_data(renderer::buffer* buf) {
	static auto overlay_container = std::make_unique<carbon::flex_container>();
	static bool init = false;

	static carbon::label<std::string>* label = nullptr;

	if (!init) {
		init = true;

		// Create labels
	}

	// Update labels

	static renderer::timer fps_timer;
	static int pds = 0;
	static int fps = 0;

	populate_count++;

	if (fps_timer.get_elapsed_duration() >= std::chrono::seconds(1)) {
		fps_timer.reset();

		pds = populate_count;
		populate_count = 0;

		fps = draw_count;
		draw_count = 0;
	}

	buf->push_font(carbon::segoe_font);

	const auto mouse = carbon::get_mouse_pos();
	buf->draw_text<std::string>({25.0f, 25.0f}, fmt::format("PDS: {}", pds));
	buf->draw_text<std::string>({25.0f, 45.0f}, fmt::format("FPS: {}", fps));
	buf->draw_text<std::string>({25.0f, 65.0f}, fmt::format("Mouse position: ({}, {})", mouse.x, mouse.y));
	buf->draw_text<std::string>({25.0f, 85.0f}, fmt::format("Mouse state: {} {}", carbon::is_key_pressed(VK_LBUTTON),
															 carbon::is_key_down(VK_LBUTTON)));
	buf->draw_text<std::string>({25.0f, 105.0f}, fmt::format("Batches: {}", carbon::benchmark.draw_calls,
															 carbon::is_key_down(VK_LBUTTON)));
	buf->draw_text<std::string>({25.0f, 125.0f}, fmt::format("Flex compute: {}", carbon::benchmark.flex_compute_calls,
															  carbon::is_key_down(VK_LBUTTON)));

	buf->pop_font();
}

void draw_thread() {
    const auto id = carbon::dx11->register_buffer();

    while (!close_requested) {
		updated_draw.wait();

		carbon::buf = carbon::dx11->get_working_buffer(id);

		carbon::begin();

		//draw_test_flex(carbon::buf);
		draw_test_ui(carbon::buf);
		draw_input_data(carbon::buf);

		carbon::end();

		carbon::dx11->swap_buffers(id);
        updated_buf.notify();
    }
}

// TODO: Mutex for textures
int main() {
    carbon::application = std::make_unique<renderer::win32_window>();
	carbon::application->set_title("Carbon");
	carbon::application->set_size({1920, 1080});

    // Center window position
    {
        RECT client;
        if (GetClientRect(GetDesktopWindow(), &client)) {
            const auto size = carbon::application->get_size();
			carbon::application->set_pos({client.right / 2 - size.x / 2, client.bottom / 2 - size.y / 2});
        }
    }

	carbon::application->set_proc(WndProc);

    if (!carbon::application->create()) {
        MessageBoxA(nullptr, "Failed to create window.", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    carbon::dx11 = std::make_unique<renderer::d3d11_renderer>(carbon::application.get());

    if (!carbon::dx11->init()) {
        MessageBoxA(nullptr, "Failed to initialize renderer.", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

	carbon::dx11->set_vsync(false);
	carbon::dx11->set_clear_color({88, 88, 88});//({88, 122, 202});

    carbon::segoe_font = carbon::dx11->register_font("Segoe UI Emoji", 10, FW_THIN, true);

	carbon::init();

    std::thread draw(draw_thread);

	carbon::application->set_visibility(true);

	MSG msg{};
    while (!close_requested && msg.message != WM_QUIT) {
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		if (msg.message == WM_NULL && !IsWindow(carbon::application->get_hwnd())) {
			close_requested = true;
			break;
		}

		// TODO: Fix issues with resize
        if (update_size) {
			carbon::dx11->resize();
			carbon::dx11->reset();

            update_size = false;
        }

		carbon::dx11->draw();
		carbon::benchmark.draw_calls = carbon::dx11->total_batches;
		draw_count++;

		updated_draw.notify();
        updated_buf.wait();
    }

    draw.join();

	carbon::dx11->release();
	carbon::application->destroy();

    return 0;
}