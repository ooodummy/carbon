#include <carbon/carbon.hpp>

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

void draw_test_interface(renderer::buffer* buf) {
    carbon::begin();

    carbon::buf = buf;
    carbon::main_font = seguiemj;

    static auto root = std::make_shared<carbon::view>(carbon::layout_properties{
                                                          .align_items = carbon::align_items_center,
                                                          .height = carbon::value_type{300.0f, false},
                                                          .justify_content = carbon::justify_content_center,
                                                          //.margin = 10.0f,
                                                          .overflow = carbon::overflow_hidden,
                                                          .width = carbon::value_type{300.0f, false}
                                                      }, carbon::decorative_properties{
                                                          .background_color = {68, 68, 68, 255},
                                                          .border_color = {102, 102, 102, 255},
                                                          .border_radius = 6.0f
                                                      }, "formUI root");

    const auto test = root->layout.height;

    static bool init = false;
    if (!init) {
        // TODO: Generate UI layout
        carbon::layout_properties layout{
            .flex_grow = 1.0f
        };

        carbon::decorative_properties style{
          .background_color = {255, 0, 0, 255}
        };

        root->add<carbon::base_view>(layout, style, "test1");
        root->add<carbon::base_view>(layout, style, "test2");
        init = true;
    }

    carbon::layout(root, {1024.0f, 768.0f});
    carbon::compose(root);
    carbon::paint(root);

    carbon::debug_info();
    carbon::end();
}

void draw_thread() {
    const auto id = dx11->register_buffer(0);

    while (!close_requested) {
        updated_draw.wait();

        renderer::atlas.locked = true;
        set_default_font(renderer::get_default_font());

        auto buf = dx11->get_working_buffer(id);
        buf->set_projection(dx11->get_ortho_projection());

        draw_test_interface(buf);

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

    dx11->set_clear_color({ 88, 122, 202 });

    char csidl_fonts[MAX_PATH];
    memset(csidl_fonts, 0, MAX_PATH);
    SHGetFolderPathA(nullptr, CSIDL_FONTS, nullptr, 0, csidl_fonts);

    renderer::text_font::font_config config{.glyph_config{.ranges= renderer::text_font::glyph::ranges_default()}, .size_pixels = 32.f};
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