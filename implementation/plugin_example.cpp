#include "plugin_example.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_sfm.hpp"
#include "toolbar.hpp"
#include "canvas.hpp"
#include "menubar.hpp"
#include "optionsbar.hpp"
#include <dlfcn.h>

using namespace psapi;

class Instrument : public IWindow {
public:
    Instrument(/*const sfm::ASprite* sprite*/) : IWindow() {}
    // virtual bool update(const IRenderWindow* renderWindow, const Event& event) override { return false; }
    virtual void draw(IRenderWindow* renderWindow) override {}
    // ... add other methods

    //virtual void draw(RenderWindow& renderWindow) override {}

    virtual int64_t getId() const override { return kInvalidWindowId; }

    virtual IWindow* getWindowById(wid_t id) override { return nullptr; }

    virtual const IWindow* getWindowById(wid_t id) const override { return nullptr; }

    virtual vec2i getPos() const override { return vec2i(); }

    virtual vec2u getSize() const override { return vec2u(); }

    virtual void setParent(const IWindow* parent) override {}

    virtual void forceActivate() override {}

    virtual void forceDeactivate() override {}
};

bool loadBasePlugin() {
    std::cout << "in loadbaseplugin:" << getRootWindow() << "\n";

    vec2i pos(5, 55);
    vec2u size(100, 700);
    // ToolBar bar(pos, size);
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
