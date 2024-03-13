#include <carbon/carbon.hpp>

#include "config.hpp"

#include <renderer/buffer.hpp>
#include <renderer/renderer.hpp>

#include <Dwmapi.h>
#include <ShlObj.h>

#include <thread>
#include <algorithm>

renderer::text_font* tahoma = nullptr;
renderer::text_font* seguiemj = nullptr;

extern "C" {
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

std::shared_ptr<renderer::win32_window> application;
std::unique_ptr<renderer::d3d11_renderer> dx11;

renderer::sync_manager updated_draw;
renderer::sync_manager updated_buf;

bool close_requested = false;

renderer::performance_counter swap_counter;
renderer::performance_counter present_counter;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static bool in_size_move = false;

    switch (msg) {
        case WM_PAINT:
            if (in_size_move)
                dx11->render();
            else {
                PAINTSTRUCT ps;
                std::ignore = BeginPaint(hWnd, &ps);
                EndPaint(hWnd, &ps);
            }
        case WM_DISPLAYCHANGE:
            dx11->on_display_change();
            break;
        case WM_CLOSE:
            close_requested = true;
            return 0;
        case WM_SIZE:
            if (!in_size_move)
                dx11->on_window_size_change({ LOWORD(lParam), HIWORD(lParam) });
            break;
        case WM_MOVE:
            dx11->on_window_moved();
            break;
        case WM_ENTERSIZEMOVE:
            in_size_move = true;
            break;
        case WM_EXITSIZEMOVE:
            in_size_move = false;
            dx11->on_window_size_change(application->get_size());
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
        window = std::make_shared<carbon::window>(L"Carbon UI Framework");

        auto tab_sheet = window->add_child<carbon::tab_sheet>();

        auto aim_page = tab_sheet->add_child<carbon::page>(U"\uf05b");
        {
            auto aimbot_group = aim_page->add_child<carbon::groupbox>(L"Aimbot");
            {
                aimbot_group->add_child<carbon::checkbox>(L"Enabled", &settings::get<bool>(cfg.aimbot));
                // TODO: Hotkey
                //aimbot_group->add_child<carbon::slider<int>>(L"Field of view", &settings::get<int>(cfg.aimbot_fov), 0,
                //                                             500, L"{}px");
                aimbot_group->add_child<carbon::checkbox>(L"Show FOV", &settings::get<bool>(cfg.aimbot_show_fov));
                //aimbot_group->add_child<carbon::slider<int>>(
                //    L"Max Distance", &settings::get<int>(cfg.aimbot_max_distance), 0, 1000, L"{}m");
                aimbot_group->add_child<carbon::checkbox>(L"Silent", &settings::get<bool>(cfg.aimbot_silent));
                //aimbot_group->add_child<carbon::checkbox>(L"Magic bullet", &settings::get<bool>(cfg.aimbot_modify_origin));
                //aimbot_group->add_child<carbon::checkbox>(L"Show aim point", &settings::get<bool>(cfg.aimbot_show_aim_point));
                // TODO: Color picker
                // TODO: Widget conditions
                aimbot_group->add_child<carbon::checkbox>(L"Only visible", &settings::get<bool>(cfg.aimbot_only_visible));
                aimbot_group->add_child<carbon::checkbox>(L"Ignore team", &settings::get<bool>(cfg.aimbot_ignore_team));
                //aimbot_group->add_child<carbon::checkbox>(L"Show target information", &settings::get<bool>(cfg.aimbot_show_target_info));
            }
            auto recoil_group = aim_page->add_child<carbon::groupbox>(L"Recoil control");
            {
                //recoil_group->add_child<carbon::checkbox>(L"Enabled", &settings::get<bool>(cfg.aim_no_recoil));
                //recoil_group->add_child<carbon::slider<int>>(L"Strength", &settings::get<int>(cfg.aim_recoil_control), 0,
                //                                             100, L"{}%");
                //recoil_group->add_child<carbon::checkbox>(L"No spread", &settings::get<bool>(cfg.aim_no_spread));
                //recoil_group->add_child<carbon::checkbox>(L"No sway", &settings::get<bool>(cfg.aim_no_sway));

                recoil_group->add_child<carbon::button>(
                    L"Click me", [] { MessageBoxA(nullptr, "Hello world!", "KiwiCheats.net", MB_OK); });
            }
        }

        auto visuals_page = tab_sheet->add_child<carbon::page>(U"\uf21d");
        {
            auto player_group = visuals_page->add_child<carbon::groupbox>(L"Player");
            {
                player_group->add_child<carbon::checkbox>(L"Enabled", &settings::get<bool>(cfg.player_visuals));
                //static std::vector<std::wstring> box_types = { L"None", L"2D", L"3D", L"Italic" };
                //player_group->add_child<carbon::dropdown>(L"Box", box_types, &settings::get<int>(cfg.player_visuals_box));
                player_group->add_child<carbon::checkbox>(L"Name", &settings::get<bool>(cfg.player_visuals_name));
                player_group->add_child<carbon::checkbox>(L"Health", &settings::get<bool>(cfg.player_visuals_health));
                player_group->add_child<carbon::checkbox>(L"Ammo", &settings::get<bool>(cfg.player_visuals_ammo));
            }
            auto item_group = visuals_page->add_child<carbon::groupbox>(L"Item");
            auto world_group = visuals_page->add_child<carbon::groupbox>(L"World");
        }

        auto misc_page = tab_sheet->add_child<carbon::page>(U"\uf022");
        {
            auto weapon_group = misc_page->add_child<carbon::groupbox>(L"Weapon");
            {
                //weapon_group->add_child<carbon::checkbox>(L"No dispersion", &settings::get<bool>(cfg.weapon_no_dispersion));
                //weapon_group->add_child<carbon::checkbox>(L"No malfunctions", &settings::get<bool>(cfg.weapon_no_malfunctions));
                //weapon_group->add_child<carbon::checkbox>(L"Fast load/unload", &settings::get<bool>(cfg.weapon_fast_mags));
                //weapon_group->add_child<carbon::checkbox>(L"Instant swap", &settings::get<bool>(cfg.weapon_instant_swap));
            }
            auto movement_group = misc_page->add_child<carbon::groupbox>(L"Movement");
            {
                //movement_group->add_child<carbon::checkbox>(L"No fracture", &settings::get<bool>(cfg.movement_no_fracture));
                //movement_group->add_child<carbon::checkbox>(L"Always sprint", &settings::get<bool>(cfg.movement_allow_sprint));
                //movement_group->add_child<carbon::checkbox>(L"Always jump", &settings::get<bool>(cfg.movement_allow_jump));
                //movement_group->add_child<carbon::checkbox>(L"Always prone", &settings::get<bool>(cfg.movement_allow_prone));
            }
            auto exploits_group = misc_page->add_child<carbon::groupbox>(L"Exploits");
            {
                //exploits_group->add_child<carbon::checkbox>(L"God mode", &settings::get<bool>(cfg.exploits_god));
                //exploits_group->add_child<carbon::checkbox>(L"Ignore power", &settings::get<bool>(cfg.exploits_ignore_power));
            }
        }
        auto config_page = tab_sheet->add_child<carbon::page>(U"\uf013");
        {
            auto config_group = config_page->add_child<carbon::groupbox>(L"Config");
            {
                static std::string config_name = "";
                config_group->add_child<carbon::textbox>(L"Name", L"default", 32, &config_name, false);
                config_group->add_child<carbon::button>(L"Save", [] {
                    settings::save("default");
                });
                config_group->add_child<carbon::button>(L"Load", [] {
                    settings::load("default");
                });
            }
        }

        init = true;
    }

    window->input();
    window->calculate_layout(application->get_size());
    window->draw();

    std::string test = "Hello World!";
    const auto size = tahoma->calc_text_size(test);
    buf->draw_text(test, {100.0f, 100.0f}, COLOR_WHITE, tahoma, renderer::align_center_top);
    buf->draw_rect({100.0f, 100.0f, size.x, size.y}, COLOR_RED);
}

void draw_test_interface(renderer::buffer* buf) {
    /*static auto root = std::make_shared<carbon::view>(carbon::layout_properties{
                                                          //.align_content = carbon::align_content_end,
                                                          .align_items = carbon::align_items_start,
                                                          .align_self = carbon::align_self_stretch,
                                                          .flex_direction = carbon::flex_direction_row,
                                                          .flex_grow = 1.0f,
                                                          .flex_wrap = carbon::flex_wrap_no_wrap,
                                                          //.height = carbon::value_type{300.0f, false},
                                                          .justify_content = carbon::justify_content_start,
                                                          //.margin = 10.0f,
                                                          .overflow = carbon::overflow_hidden,
                                                          .padding = 25.0f,
                                                          //.width = carbon::value_type{300.0f, false}
                                                      }, carbon::decorative_properties{
                                                          .background_color = {18, 18, 18, 255},
                                                          .border_color = {102, 102, 102, 255},
                                                          .border_radius = 6.0f
                                                      }, "formUI root");

    static bool init = false;
    if (!init) {
        carbon::layout_properties column{
            //.align_self = carbon::align_self_stretch,
            //.flex_grow = 1.0f,
            //.flex_basis = carbon::value_type{100.0f, false},
            //.min_height = carbon::value_type{50.0f, false},
            //.flex_wrap = carbon::flex_wrap_wrap,
            .height = carbon::value_type{50.0f, false},
            .width = carbon::value_type{100.0f, false},
            //.margin = 10.0f
        };

        carbon::decorative_properties style{
            .background_color = {29, 29, 29, 255},
            .border_color = {0, 0, 0, 255},
            .border_radius = 6.0f
        };

        root->add<carbon::base_view>(column, style, "test1");
        root->add<carbon::base_view>(column, style, "test2");

        auto test = []() -> void {
            MessageBoxA(nullptr, "Button clicked!", "Button", MB_ICONINFORMATION);
        };

        // std::string label, std::function<void()> on_click_callback, layout_properties layout, decorative_properties style, text_style_properties text_style
        root->add<carbon::button>("Button", test, column, style, carbon::text_style_properties{.color = {255, 255, 255}, .font = renderer::get_default_font()});

        //carbon::layout(root, {300.0f, 300.0f});
        //carbon::compose(root);
        init = true;
    }*/

    /*static auto window = std::make_shared<carbon::window>("formUI root");
    static bool init = false;

    if (!init) {
        carbon::layout_properties column{
            //.align_self = carbon::align_self_stretch,
            //.flex_grow = 1.0f,
            //.flex_basis = carbon::value_type{100.0f, false},
            //.min_height = carbon::value_type{50.0f, false},
            //.flex_wrap = carbon::flex_wrap_wrap,
            .height = carbon::value_type{50.0f, false},
            .width = carbon::value_type{100.0f, false},
            //.margin = 10.0f
        };

        carbon::decorative_properties style{
            .background_color = {29, 29, 29, 255},
            .border_color = {0, 0, 0, 255},
            .border_radius = 6.0f
        };

        auto test = []() -> void {
            MessageBoxA(nullptr, "Button clicked!", "Button", MB_ICONINFORMATION);
        };

        window->add<carbon::button>("Button", test, column, style, carbon::text_style_properties{.color = {255, 255, 255}, .font = renderer::get_default_font()});

        init = true;
    }

    carbon::layout(window, carbon::get_mouse_pos());
    carbon::compose(window);
    carbon::paint(window);*/

    /*static renderer::timer anim_timer;
    if (anim_timer.get_elapsed_duration() > std::chrono::milliseconds(640))
        anim_timer.reset();

    const auto anim = renderer::ease(0.0f, 6.28f, std::chrono::duration<float>(anim_timer.get_elapsed_duration()).count() / 0.64f, 1.0f, renderer::linear);

    const auto off_x = cosf(anim);
    const auto off_y = sinf(anim);

    glm::vec2 text_pos = {100.0f, 100.0f};
    buf->draw_text("FATALITY", text_pos + glm::vec2{off_x, off_y}, renderer::color_rgba{255, 0, 0, 255}, tahoma);
    buf->draw_text("FATALITY", text_pos + glm::vec2{off_x * -1.0f, off_y * -1.0f}, renderer::color_rgba{0, 0, 255, 255}, tahoma);
    buf->draw_text(std::format("FATALITY {}", anim), text_pos, renderer::color_rgba{245, 245, 245, 255}, tahoma);*/
}

void draw_thread() {
    const auto id = dx11->register_buffer(0);

    while (!close_requested) {
        updated_draw.wait();

        renderer::atlas.locked = true;
        set_default_font(renderer::get_default_font());

        auto buf = dx11->get_working_buffer(id);
        buf->set_projection(dx11->get_ortho_projection());

        carbon::begin();

        carbon::buf = buf;
        carbon::main_font = tahoma;

        draw_test_ui(buf);
        draw_test_interface(buf);

        carbon::debug_info();
        carbon::end();

        dx11->swap_buffers(id);

        swap_counter.tick();

        renderer::atlas.locked = false;

        updated_buf.notify();
    }
}

int main() {
#if _DEBUG
    // if (GetConsoleWindow() == nullptr) {
    // 	if (!AllocConsole()) {
    // 		MessageBoxA(nullptr, std::format("Unable to allocate console.\nError: {}", GetLastError()).c_str(), "Error",
    // 					MB_ICONERROR);
    // 		return 1;
    // 	}
    //
    // 	ShowWindow(GetConsoleWindow(), SW_SHOW);
    //
    // 	FILE* dummy;
    // 	freopen_s(&dummy, "CONIN$", "r", stdin);
    // 	freopen_s(&dummy, "CONOUT$", "w", stderr);
    // 	freopen_s(&dummy, "CONOUT$", "w", stdout);
    // }
#endif

    application = std::make_shared<renderer::win32_window>("D3D11 Renderer", glm::i32vec2{ 1920, 1080 }, WndProc);

    if (!application->create()) {
        MessageBoxA(nullptr, "Failed to create application window.", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    dx11 = std::make_unique<renderer::d3d11_renderer>(application);

    if (!dx11->initialize()) {
        MessageBoxA(nullptr, "Failed to initialize D3D11 renderer.", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    //dx11->set_clear_color({ 25, 27, 28 });
    dx11->set_clear_color({ 88, 122, 202 });

    char csidl_fonts[MAX_PATH];
    memset(csidl_fonts, 0, MAX_PATH);
    SHGetFolderPathA(nullptr, CSIDL_FONTS, nullptr, 0, csidl_fonts);

    renderer::text_font::font_config config{.glyph_config{.ranges= renderer::text_font::glyph::ranges_default()}, .size_pixels = 16.f};
    tahoma = renderer::atlas.add_font_default(&config);

    seguiemj = renderer::atlas.add_font_from_file_ttf(std::string(csidl_fonts) + '\\' + "seguiemj.ttf", 16.f, &config);
    dx11->create_atlases();

    application->set_visibility(true);
    carbon::initialize();

    std::thread draw(draw_thread);

    MSG msg{};
    while (!close_requested && msg.message != WM_QUIT) {
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_NULL && !IsWindow(application->get_hwnd())) {
            close_requested = true;
            break;
        }

        dx11->create_atlases();
        dx11->render();
        present_counter.tick();

        updated_draw.notify();
        updated_buf.wait();
    }

    draw.join();

    dx11->release();
    dx11.reset();

    application->destroy();

#if _DEBUG
    // ShowWindow(GetConsoleWindow(), SW_HIDE);
    //
    // if (!FreeConsole())
    // 	MessageBoxA(nullptr, "Unable to free console.", "Error", MB_ICONERROR);
#endif

    return 0;
}