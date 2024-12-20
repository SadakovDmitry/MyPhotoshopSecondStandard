#include "../headers/plugin_example.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_sfm.hpp"
#include "../headers/toolbar.hpp"
#include "../headers/canvas.hpp"
#include "../headers/menubar.hpp"
#include "../headers/optionsbar.hpp"
#include <dlfcn.h>

using namespace psapi;

bool loadBasePlugin() {
    std::cout << "in loadbaseplugin:" << getRootWindow() << "\n";

    vec2i pos(5, 55);
    vec2u size(100, 700);
    auto toolbar = std::make_unique<psapi::ToolBar>(pos, size);
    pos = {150, 150};
    size = {900, 500};
    vec2u layers_size = {1800, 1000};
    auto canvas = std::make_unique<psapi::Canvas>(pos, size, layers_size);
    pos = {0, 0};
    size = {1200, 50};
    auto menubar = std::make_unique<psapi::MenuBar>(pos, size);

    pos = {1100, 55};
    size = {100, 700};
    auto optionsbar = std::make_unique<psapi::OptionsBar>(pos, size);

    getRootWindow()->addWindow(std::move(toolbar));
    getRootWindow()->addWindow(std::move(canvas));
    getRootWindow()->addWindow(std::move(menubar));
    getRootWindow()->addWindow(std::move(optionsbar));

    return true;
}

void unloadBasePlugin() {
    IBar* toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
    toolbar->removeWindow(1);
    toolbar->removeWindow(2);
    toolbar->removeWindow(3);
}
