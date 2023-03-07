#include "config.hpp"

#include <carbon/carbon.hpp>
#include <dwmapi.h>
#include <fmt/core.h>
#include <glm/gtx/rotate_vector.hpp>
#include <renderer/buffer.hpp>
#include <renderer/renderer.hpp>
#include <thread>
#include <yoga/Yoga.h>

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
				carbon::dx11->on_window_size_change({ LOWORD(lParam), HIWORD(lParam) });
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

void draw_test_ui(renderer::buffer* buf) {
	static bool init = false;
	static std::shared_ptr<carbon::window> window;

	/*static renderer::timer rainbow_timer;
	auto elapsed_ms = rainbow_timer.get_elapsed_duration().count();
	if (elapsed_ms > 5000)
		rainbow_timer.reset();
	renderer::color_rgba rainbow = renderer::color_hsva(0.0f).ease(renderer::color_hsva(359.99f),
	static_cast<float>(elapsed_ms) / 5000.0f); carbon::theme.primary = rainbow;*/

	if (!init) {
		window = std::make_shared<carbon::window>(L"KiwiCheats.net - Escape from Tarkov");

		auto aim_page = window->add_page(U"\uf05b");
		{
			auto aimbot_group = aim_page->add_child<carbon::groupbox>(L"Aimbot");
			{
				aimbot_group->add_child<carbon::checkbox>(L"Enabled", &config::get<bool>(cfg.aimbot));
				// TODO: Hotkey
				aimbot_group->add_child<carbon::slider<int>>(L"Field of view", &config::get<int>(cfg.aimbot_fov), 0,
															 500, L"{}px");
				aimbot_group->add_child<carbon::checkbox>(L"Show FOV", &config::get<bool>(cfg.aimbot_show_fov));
				aimbot_group->add_child<carbon::slider<int>>(
				L"Max Distance", &config::get<int>(cfg.aimbot_max_distance), 0, 1000, L"{}m");
				aimbot_group->add_child<carbon::checkbox>(L"Silent", &config::get<bool>(cfg.aimbot_silent));
				aimbot_group->add_child<carbon::checkbox>(L"Magic bullet", &config::get<bool>(cfg.aimbot_modify_origin));
				aimbot_group->add_child<carbon::checkbox>(L"Show aim point", &config::get<bool>(cfg.aimbot_show_aim_point));
				// TODO: Color picker
				// TODO: Widget conditions
				aimbot_group->add_child<carbon::checkbox>(L"Only visible", &config::get<bool>(cfg.aimbot_only_visible));
				aimbot_group->add_child<carbon::checkbox>(L"Ignore team", &config::get<bool>(cfg.aimbot_ignore_team));
				aimbot_group->add_child<carbon::checkbox>(L"Show target information", &config::get<bool>(cfg.aimbot_show_target_info));
			}
			auto recoil_group = aim_page->add_child<carbon::groupbox>(L"Recoil control");
			{
				recoil_group->add_child<carbon::checkbox>(L"Enabled", &config::get<bool>(cfg.aim_no_recoil));
				recoil_group->add_child<carbon::slider<int>>(L"Strength", &config::get<int>(cfg.aim_recoil_control), 0,
															 100, L"{}%");
				recoil_group->add_child<carbon::checkbox>(L"No spread", &config::get<bool>(cfg.aim_no_spread));
				recoil_group->add_child<carbon::checkbox>(L"No sway", &config::get<bool>(cfg.aim_no_sway));

				recoil_group->add_child<carbon::button>(
				L"Click me", [] { MessageBoxA(nullptr, "Hello world!", "KiwiCheats.net", MB_OK); });
			}
		}

		auto visuals_page = window->add_page(U"\uf21d");
		{
			auto player_group = visuals_page->add_child<carbon::groupbox>(L"Player");
			{
				player_group->add_child<carbon::checkbox>(L"Enabled", &config::get<bool>(cfg.player_visuals));
				static std::vector<std::wstring> box_types = { L"None", L"2D", L"3D", L"Italic" };
				player_group->add_child<carbon::dropdown>(L"Box", box_types, &config::get<int>(cfg.player_visuals_box));
				player_group->add_child<carbon::checkbox>(L"Name", &config::get<bool>(cfg.player_visuals_name));
				player_group->add_child<carbon::checkbox>(L"Health", &config::get<bool>(cfg.player_visuals_health));
				player_group->add_child<carbon::checkbox>(L"Ammo", &config::get<bool>(cfg.player_visuals_ammo));
			}
			auto item_group = visuals_page->add_child<carbon::groupbox>(L"Item");
			auto world_group = visuals_page->add_child<carbon::groupbox>(L"World");
		}

		auto misc_page = window->add_page(U"\uf022");
		{
			auto weapon_group = misc_page->add_child<carbon::groupbox>(L"Weapon");
			{
				weapon_group->add_child<carbon::checkbox>(L"No dispersion", &config::get<bool>(cfg.weapon_no_dispersion));
				weapon_group->add_child<carbon::checkbox>(L"No malfunctions", &config::get<bool>(cfg.weapon_no_malfunctions));
				weapon_group->add_child<carbon::checkbox>(L"Fast load/unload", &config::get<bool>(cfg.weapon_fast_mags));
				weapon_group->add_child<carbon::checkbox>(L"Instant swap", &config::get<bool>(cfg.weapon_instant_swap));
			}
			auto movement_group = misc_page->add_child<carbon::groupbox>(L"Movement");
			{
				movement_group->add_child<carbon::checkbox>(L"No fracture", &config::get<bool>(cfg.movement_no_fracture));
				movement_group->add_child<carbon::checkbox>(L"Always sprint", &config::get<bool>(cfg.movement_allow_sprint));
				movement_group->add_child<carbon::checkbox>(L"Always jump", &config::get<bool>(cfg.movement_allow_jump));
				movement_group->add_child<carbon::checkbox>(L"Always prone", &config::get<bool>(cfg.movement_allow_prone));
			}
			auto exploits_group = misc_page->add_child<carbon::groupbox>(L"Exploits");
			{
				exploits_group->add_child<carbon::checkbox>(L"God mode", &config::get<bool>(cfg.exploits_god));
				exploits_group->add_child<carbon::checkbox>(L"Ignore power", &config::get<bool>(cfg.exploits_ignore_power));
			}
		}
		auto config_page = window->add_page(U"\uf013");
		{
			auto config_group = config_page->add_child<carbon::groupbox>(L"Config");
			{
				config_group->add_child<carbon::button>(L"Save", [] {
					config::save(L"default");
				});
				config_group->add_child<carbon::button>(L"Load", [] {
					config::load(L"default");
				});
			}
		}

		init = true;
	}

	window->input();
	window->calculate_layout(carbon::application->get_size());
	window->draw();
}

void draw_input_data(renderer::buffer* buf) {
	buf->push_font(carbon::segoe_ui);
	const auto mouse = carbon::get_mouse_pos();
	buf->draw_text<std::string>({ 25.0f, 25.0f }, fmt::format("FPS: {}", carbon::timer.get_fps()));
	buf->draw_text<std::string>({ 25.0f, 40.0f }, fmt::format("Mouse position: ({}, {})", mouse.x, mouse.y));
	buf->draw_text<std::string>({ 25.0f, 55.0f }, fmt::format("Mouse state: {} {}", carbon::is_key_pressed(VK_LBUTTON),
															  carbon::is_key_down(VK_LBUTTON)));

	buf->draw_text<std::string>({ 25.0f, 70.0f }, fmt::format("Vertices: {}", buf->get_vertices().size()));
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

		draw_test_ui(carbon::buf);
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
			MessageBoxA(nullptr, fmt::format("Unable to allocate console.\nError: {}", GetLastError()).c_str(), "Error",
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

	if (!config::load(L"default"))
		config::save(L"default");

	carbon::application =
	std::make_shared<renderer::win32_window>("D3D11 Renderer", glm::i32vec2{ 1920, 1080 }, WndProc);

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
	carbon::dx11->set_clear_color({ 88, 88, 88 });

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
		carbon::timer.tick();

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