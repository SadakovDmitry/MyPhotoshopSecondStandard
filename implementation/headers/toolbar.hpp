#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

// #include "api_photoshop.hpp"
// #include "api_bar.hpp"
// #include "sfm_prot.hpp"
// #include "canvas.hpp"
//#include "photoshop.hpp"

#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "my_sfm.hpp"
#include "buttons.hpp"

#include <cstdint>
#include <memory>
#include <iostream>
#include <queue>

namespace psapi {

class ToolBar : public IBar {
private:
    wid_t id;
    bool is_active;
    vec2i pos;
    vec2u size;
    vec2f scale;
    sfm::Sprite sprite;
    sfm::Texture texture;
    const IWindow* parent;
    std::vector<std::unique_ptr<IBarButton>> toolbar;
public:
    ToolBar(vec2i pos_, vec2u size_);
    ~ToolBar() = default;

    virtual void draw(IRenderWindow* renderWindow);

    virtual wid_t getId() const;

    virtual IWindow* getWindowById(wid_t id);

    virtual const IWindow* getWindowById(wid_t id) const;

    virtual vec2i getPos() const;

    virtual vec2u getSize() const;

    virtual void setParent(const IWindow* parent);

    virtual void forceActivate();

    virtual void forceDeactivate();

    virtual bool isActive() const;

    virtual void addWindow(std::unique_ptr<IWindow> window);

    virtual void removeWindow(wid_t id);

    virtual bool isWindowContainer() const;

    virtual void finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const;

    virtual bool unPressAllButtons();

    virtual std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event);

    virtual void setSize(const vec2u& size);
    virtual void setPos(const vec2i& pos);
};

class ToolBarAction : public AAction {
    std::vector<std::unique_ptr<IBarButton>>* toolbar;
public:
    ToolBarAction(std::vector<std::unique_ptr<IBarButton>>* toolbar_, const IRenderWindow *renderWindow_, const Event &event_);
    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

}

#endif
