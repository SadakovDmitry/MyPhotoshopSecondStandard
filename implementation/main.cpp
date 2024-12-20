
#include <dlfcn.h>
#include "headers/canvas.hpp"
#include "headers/toolbar.hpp"
#include <assert.h>
#include "headers/plugin_example.hpp"
#include "headers/photoshop.hpp"
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QApplication>

typedef bool (*LoadPluginFunc)();
typedef void (*UnloadPluginFunc)();

using namespace psapi;
using namespace sfm;

void loadPluginfromLib(const std::string& plugin) {
    void* pluginLib = dlopen(("/Users/dima/MIPT/SecondSem/MyPaint2.0/plugins/dylibs/" + plugin).c_str(), RTLD_LAZY);
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

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    loadBasePlugin();
    // loadPluginfromLib("pencil.dylib");
    // loadPluginfromLib("eraser.dylib");
    // loadPluginfromLib("sprayer.dylib");
    // loadPluginfromLib("scrollbar_vert.dylib");
    // loadPluginfromLib("scrollbar_hor.dylib");
    // loadPluginfromLib("filters.dylib");
    // loadPluginfromLib("color_palette.dylib");
    // loadPluginfromLib("thickness.dylib");
    // loadPluginfromLib("opacity_bar.dylib");
    loadPluginfromLib("libpencil.dylib");
    loadPluginfromLib("liberaser.dylib");
    loadPluginfromLib("libsprayer.dylib");
    loadPluginfromLib("libscrollbar_vert.dylib");
    loadPluginfromLib("libscrollbar_hor.dylib");
    loadPluginfromLib("libfilters.dylib");
    loadPluginfromLib("libcolor_palette.dylib");
    loadPluginfromLib("libthickness.dylib");
    loadPluginfromLib("libopacity_bar.dylib");
    loadPluginfromLib("libfiles.dylib");

    std::cout << "Hello!!!\n";
    const float file_height = 1564.0;
    const float file_width = 2332.0;
    RenderWindow renderWindow;
    IRootWindow* root_window = getRootWindow();
    while (renderWindow.isOpen()) {
        sfm::Event event;
        while (renderWindow.pollEvent(event)) {
            if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) { renderWindow.close(); }
            if (event.type == sfm::Event::Closed) { renderWindow.close(); }
            //getRootWindow()->update(&renderWindow, event);
            // unsigned long long start = clock();
            // std::cerr << "\033[34m              event\033[0m\n";
            getActionController()->execute(root_window->createAction(&renderWindow, event));
            // getRootWindow()->draw(&renderWindow);
            // unsigned long long end = clock();
            // unsigned long long time = ((end - start) * 100000) / CLOCKS_PER_SEC;
            // std::cout << "\033[32mtime: " << time << "\033[0m\n";
            getRootWindow()->draw(&renderWindow);
            renderWindow.display();
        }
        getRootWindow()->draw(&renderWindow);
        renderWindow.display();
    }
    return 0;
}
