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
    carbon::buf = buf;
    carbon::main_font = seguiemj;

    static bool init = false;
    static auto flex_container = std::make_unique<carbon::flex_container>();

    // Compare to https://github.com/layoutBox/FlexLayout
    if (!init) {
        flex_container->set_pos({50.0f, 50.0f});
        flex_container->set_flow(carbon::column);

        flex_container->set_justify_content(carbon::justify_center);
        flex_container->set_padding(30.0f);

        // This does not produce the desired output
        //auto test1 = flex_container->add_child<carbon::flex_item>();
        //test1->set_basis(75.0f, carbon::unit_pixel);
        //auto test2 = flex_container->add_child<carbon::flex_item>();
        //test2->set_basis(75.0f, carbon::unit_pixel);
        //test2->set_margin({30.0f, 0.0f, 0.0f, 0.0f});
        //auto test3 = flex_container->add_child<carbon::flex_item>();
        //test3->set_basis(75.0f, carbon::unit_pixel);
        //test3->set_margin({30.0f, 0.0f, 0.0f, 0.0f});

        // Justify content
        const auto justify_start = flex_container->add_child<carbon::flex_container>();
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
		justify_space_evenly->add_child<carbon::flex_item>()->set_basis(75.0f, carbon::unit_pixel);

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

    application = std::make_shared<renderer::win32_window>("D3D11 Renderer", glm::i32vec2{ 960, 500 }, WndProc);

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

    renderer::text_font::font_config config{ .glyph_config{ .ranges{ renderer::text_font::glyph::ranges_default() } },
                                             .size_pixels = 32.f };
    tahoma = renderer::atlas.add_font_default(&config);

    seguiemj = renderer::atlas.add_font_from_file_ttf(std::string(csidl_fonts) + '\\' + "seguiemj.ttf", 32.f, &config);
    renderer::atlas.add_font_from_file_ttf(std::string(csidl_fonts) + '\\' + "seguiemj.ttf", 64.f, &config);
    dx11->create_atlases();

    application->set_visibility(true);

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