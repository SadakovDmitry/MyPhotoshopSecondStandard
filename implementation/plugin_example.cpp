#include "plugin_example.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_sfm.hpp"
#include "toolbar.hpp"
#include "canvas.hpp"
#include <dlfcn.h>

using namespace psapi;

class Instrument : public IWindow {
public:
    Instrument(/*const sfm::ASprite* sprite*/) : IWindow() {}
    // virtual bool update(const IRenderWindow* renderWindow, const Event& event) override { return false; }
    virtual void draw(IRenderWindow* renderWindow) override {}
    // ... add other methods

    //virtual void draw(RenderWindow& renderWindow) override {}

    //virtual bool update(const RenderWindow& renderWindow, const Event& event) {}

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

    vec2i pos(5, 5);
    vec2u size(100, 700);
    // ToolBar bar(pos, size);s
    auto toolbar = std::make_unique<psapi::ToolBar>(pos, size);
//     pos = {5, 5};
//     size = {50, 50};
//     auto pencil = std::make_unique<psapi::PencilTool>(pos, size, 1);
//     pos = {55, 5};
//     auto eraser = std::make_unique<psapi::EraserTool>(pos, size, 2);
//     pos = {5, 55};
//     auto sprayer = std::make_unique<psapi::SprayerTool>(pos, size, 3);
    pos = {200, 100};
    size = {900, 500};
    vec2u layers_size = {1800, 1000};
    auto canvas = std::make_unique<psapi::Canvas>(pos, size, layers_size);
//
    getRootWindow()->addWindow(std::move(toolbar));
//     static_cast<ToolBar*>(psapi::getRootWindowlib()->getWindowById(kToolBarWindowId))->addWindow(std::move(pencil));
//     static_cast<ToolBar*>(psapi::getRootWindowlib()->getWindowById(kToolBarWindowId))->addWindow(std::move(eraser));
//     static_cast<ToolBar*>(psapi::getRootWindowlib()->getWindowById(kToolBarWindowId))->addWindow(std::move(sprayer));
//     //psapi::getRootWindowlib()->addWindow(std::move(canvas));
    getRootWindow()->addWindow(std::move(canvas));

    return true;
}

void unloadBasePlugin() {
    IBar* toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
    toolbar->removeWindow(1);
    toolbar->removeWindow(2);
    toolbar->removeWindow(3);
}
