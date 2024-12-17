// #include "api_photoshop.hpp"
// #include "api_bar.hpp"
#include <dlfcn.h>
#include "canvas.hpp"
#include "toolbar.hpp"
#include <assert.h>
// #include "sfm_prot.hpp"
#include "plugin_example.hpp"
#include "photoshop.hpp"
#include "ctime"

typedef bool (*LoadPluginFunc)();
typedef void (*UnloadPluginFunc)();

using namespace psapi;
using namespace sfm;

void loadPluginfromLib(const std::string& plugin) {
    void* pluginLib = dlopen(("/Users/dima/MIPT/SecondSem/MyPaint2.0/plugins/" + plugin).c_str(), RTLD_LAZY);
    if (!pluginLib) {
        std::cerr << "Failed to load plugin dylib in main: " << dlerror() << "\n";
        assert("loadPluginfromLib" && 0);
    }

    LoadPluginFunc loadPlugin = reinterpret_cast<LoadPluginFunc>(dlsym(pluginLib, "loadPlugin"));
    UnloadPluginFunc unloadPlugin = reinterpret_cast<UnloadPluginFunc>(dlsym(pluginLib, "unloadPlugin"));

    if (!loadPlugin || !unloadPlugin) {
        std::cerr << "Failed to locate functions in plugin dylib in main: " << dlerror() << "\n";
        dlclose(pluginLib);
        assert("loadPluginfromLib" && 0);
    }

    if (!loadPlugin()) {
        std::cerr << "Failed to initialize plugin in main.\n";
        dlclose(pluginLib);
        assert("loadPluginfromLib" && 0);
    }

    std::cout << "Plugin loaded successfully!\n";
}

int main() {
    loadBasePlugin();
    loadPluginfromLib("pencil.dylib");
    loadPluginfromLib("eraser.dylib");
    loadPluginfromLib("sprayer.dylib");
    loadPluginfromLib("scrollbar_vert.dylib");
    loadPluginfromLib("scrollbar_hor.dylib");
    // loadPluginfromLib("blur.dylib");
    // loadPluginfromLib("brightness.dylib");

    std::cout << "Hello!!!\n";
    const float file_height = 1564.0;
    const float file_width = 2332.0;
    RenderWindow render_window;
    IRootWindow* root_window = getRootWindow();
    while (render_window.isOpen()) {
        unsigned long long start = clock();
        sfm::Event event;
        // std::cerr << "\033[35m              isOpen\033[0m\n";
        while (render_window.pollEvent(event)) {
            if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) { render_window.close(); }
            if (event.type == sfm::Event::Closed) { render_window.close(); }
            //getRootWindow()->update(&render_window, event);
            // unsigned long long start = clock();
            // std::cerr << "\033[34m              event\033[0m\n";
            getActionController()->execute(root_window->createAction(&render_window, event));
            // getRootWindow()->draw(&render_window);
            // unsigned long long end = clock();
            // unsigned long long time = ((end - start) * 100000) / CLOCKS_PER_SEC;
            // std::cout << "\033[32mtime: " << time << "\033[0m\n";
            getRootWindow()->draw(&render_window);
            render_window.display();
        }
        // unsigned long long end = clock();
        // unsigned long long time = ((end - start) * 100000) / CLOCKS_PER_SEC;
        // std::cout << "\033[32mtime: " << time << "\033[0m\n";
        // if (time >= 11) {
        //     getchar();
        // }
        getRootWindow()->draw(&render_window);
        render_window.display();
        unsigned long long end = clock();
        unsigned long long time = ((end - start) * 100000) / CLOCKS_PER_SEC;
        //std::cout << "\033[32mtime: " << time << "\033[0m\n";
    }
    return 0;
}
