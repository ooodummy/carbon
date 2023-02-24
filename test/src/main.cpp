#include <carbon/carbon.hpp>

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
	static auto flex_container = std::make_unique<carbon::flex_container>();

	// Compare to https://github.com/layoutBox/FlexLayout
	if (!init) {
		flex_container->set_pos({50.0f, 50.0f});
		flex_container->set_flow(carbon::column);

		flex_container->set_justify_content(carbon::justify_center);
		flex_container->set_padding(30.0f);

		// This does not produce the desired output
		auto test1 = flex_container->add_child<carbon::flex_item>();
		test1->set_basis(75.0f, carbon::unit_pixel);
		auto test2 = flex_container->add_child<carbon::flex_item>();
		test2->set_basis(75.0f, carbon::unit_pixel);
		test2->set_margin({30.0f, 0.0f, 0.0f, 0.0f});
		auto test3 = flex_container->add_child<carbon::flex_item>();
		test3->set_basis(75.0f, carbon::unit_pixel);
		test3->set_margin({30.0f, 0.0f, 0.0f, 0.0f});

		// Justify content
		/*const auto justify_start = flex_container->add_child<carbon::flex_container>();
		justify_start->set_flex(1.0f);
		justify_start->set_justify_content(carbon::justify_start);
		justify_start->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_start->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_start->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_start->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_start->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);

		const auto justify_end = flex_container->add_child<carbon::flex_container>();
		justify_end->set_flex(1.0f);
		justify_end->set_justify_content(carbon::justify_end);
		justify_end->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_end->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_end->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_end->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_end->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);

		const auto justify_center = flex_container->add_child<carbon::flex_container>();
		justify_center->set_flex(1.0f);
		justify_center->set_justify_content(carbon::justify_center);
		justify_center->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_center->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_center->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_center->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_center->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);

		const auto justify_space_between = flex_container->add_child<carbon::flex_container>();
		justify_space_between->set_flex(1.0f);
		justify_space_between->set_justify_content(carbon::justify_space_between);
		justify_space_between->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_space_between->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_space_between->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_space_between->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_space_between->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);

		const auto justify_space_around = flex_container->add_child<carbon::flex_container>();
		justify_space_around->set_flex(1.0f);
		justify_space_around->set_justify_content(carbon::justify_space_around);
		justify_space_around->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_space_around->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_space_around->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_space_around->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_space_around->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);

		const auto justify_space_evenly = flex_container->add_child<carbon::flex_container>();
		justify_space_evenly->set_flex(1.0f);
		justify_space_evenly->set_justify_content(carbon::justify_space_evenly);
		justify_space_evenly->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_space_evenly->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_space_evenly->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_space_evenly->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);
		justify_space_evenly->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);*/

		// Test
		/*const auto container1 = flex_container->add_child<carbon::flex_container>();
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
		container2->set_min_width(150.0f);*/

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

		draw_test_flex(carbon::buf);
		//draw_test_ui(carbon::buf);
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