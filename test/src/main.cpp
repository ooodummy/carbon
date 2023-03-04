#include <carbon/carbon.hpp>

#include <dwmapi.h>
#include <thread>

#include <glm/gtx/rotate_vector.hpp>
#include <fmt/core.h>
#include <yoga/Yoga.h>
#include <renderer/renderer.hpp>
#include <renderer/buffer.hpp>

renderer::sync_manager updated_draw;
renderer::sync_manager updated_buf;

bool update_size = false;
bool close_requested = false;

int populate_count = 0;
int draw_count = 0;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static bool in_size_move = false;

	switch (msg) {
		case WM_PAINT:
			if (in_size_move)
				carbon::dx11->render();
			else {
				PAINTSTRUCT ps;
				std::ignore = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
			}
		case WM_DISPLAYCHANGE:
			carbon::dx11->on_display_change();
			break;
		case WM_CLOSE:
			close_requested = true;
			return 0;
		case WM_SIZE:
			if (!in_size_move)
				carbon::dx11->on_window_size_change({LOWORD(lParam), HIWORD(lParam)});
			break;
		case WM_MOVE:
			carbon::dx11->on_window_moved();
			break;
		case WM_ENTERSIZEMOVE:
			in_size_move = true;
			break;
		case WM_EXITSIZEMOVE:
			in_size_move = false;
			carbon::dx11->on_window_size_change(carbon::application->get_size());
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
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
	static std::shared_ptr<carbon::window> window;

	if (!init) {
		window = std::make_shared<carbon::window>();

		auto global_group = window->page_->add_child<carbon::groupbox>();
		global_group->add_child<carbon::button>();
		global_group->add_child<carbon::checkbox>();

		auto esp_group = window->page_->add_child<carbon::groupbox>();

		init = true;
	}

	window->input();
	window->calculate_layout(carbon::application->get_size());
	window->draw();
}

void draw_input_data(renderer::buffer* buf) {
	buf->push_font(carbon::segoe_font);

	const auto mouse = carbon::get_mouse_pos();
	buf->draw_text<std::string>({25.0f, 25.0f}, fmt::format("FPS: {}", carbon::performance.get_fps()));
	buf->draw_text<std::string>({25.0f, 40.0f}, fmt::format("Mouse position: ({}, {})", mouse.x, mouse.y));
	buf->draw_text<std::string>({25.0f, 55.0f}, fmt::format("Mouse state: {} {}", carbon::is_key_pressed(VK_LBUTTON),
															 carbon::is_key_down(VK_LBUTTON)));
	buf->pop_font();
}

void draw_thread() {
    const auto id = carbon::dx11->register_buffer();

    while (!close_requested) {
		updated_draw.wait();

		carbon::buf = carbon::dx11->get_working_buffer(id);

		carbon::begin();

		/*carbon::buf->draw_rect({200.0f, 200.0f, 100.0f, 50.0f}, COLOR_RED, 1.0f);
		carbon::buf->draw_rect_filled({200.0f, 260.0f, 100.0f, 50.0f}, COLOR_GREEN);
		carbon::buf->draw_rect_rounded({200.0f, 320.0f, 100.0f, 50.0f}, 15.0f, COLOR_GREEN, 1.0f);
		carbon::buf->draw_rect_rounded_filled({200.0f, 380.0f, 100.0f, 50.0f}, 15.0f, COLOR_BLUE);*/

		draw_test_flex(carbon::buf);
		draw_input_data(carbon::buf);

		carbon::end();

		carbon::dx11->swap_buffers(id);
        updated_buf.notify();
    }
}

// TODO: Mutex for textures
int main() {
#if _DEBUG
	if (GetConsoleWindow() == nullptr) {
		if (!AllocConsole()) {
			MessageBoxA(nullptr, fmt::format("Unable to allocate console.\nError: {}", GetLastError()).c_str(),
						"Error",
						MB_ICONERROR);
			return 1;
		}

		ShowWindow(GetConsoleWindow(), SW_SHOW);

		FILE* dummy;
		freopen_s(&dummy, "CONIN$", "r", stdin);
		freopen_s(&dummy, "CONOUT$", "w", stderr);
		freopen_s(&dummy, "CONOUT$", "w", stdout);
	}
#endif

	carbon::application = std::make_shared<renderer::win32_window>("D3D11 Renderer", glm::i32vec2{1920, 1080}, WndProc);

	if (!carbon::application->create()) {
		MessageBoxA(nullptr, "Failed to create application window.", "Error", MB_ICONERROR | MB_OK);
		return 1;
	}

	// Testing Win32 window attributes
	/*{
		auto attribute = DWMWCP_DONOTROUND;
		DwmSetWindowAttribute(application->get_hwnd(), DWMWA_WINDOW_CORNER_PREFERENCE, &attribute, sizeof(attribute));
	}*/

	carbon::dx11 = std::make_unique<renderer::d3d11_renderer>(carbon::application);
	carbon::dx11->set_clear_color({88, 88, 88});

	if (!carbon::dx11->initialize()) {
		MessageBoxA(nullptr, "Failed to initialize D3D11 renderer.", "Error", MB_ICONERROR | MB_OK);
		return 1;
	}

	carbon::initialize();

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

		carbon::dx11->render();
		carbon::performance.tick();

		updated_draw.notify();
		updated_buf.wait();
	}

	draw.join();

	carbon::dx11->release();
	carbon::dx11.reset();

	carbon::application->destroy();

#if _DEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	if (!FreeConsole())
		MessageBoxA(nullptr, "Unable to free console.", "Error", MB_ICONERROR);
#endif

	return 0;
}