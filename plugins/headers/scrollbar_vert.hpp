#ifndef ERASER_HPP
#define ERASER_HPP

//#include "api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/headers/my_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/headers/photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/headers/canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_actions.hpp"


#include <cstdint>
#include <memory>
#include <iostream>
#include <assert.h>

namespace psapi {

class ScrollBarVert : public IBar {
private:
    friend class ScrollBar;
    friend class ScrollBarSlider;
    friend class ScrollBarVertAction;
    wid_t id;
    bool is_active;
    vec2i pos;
    vec2u size;
    vec2f scale;
    sfm::Sprite sprite;
    sfm::Texture texture;
    const IWindow* parent;
    std::vector<std::unique_ptr<ABarButton>> scrollbar;
    Scrollable* scroll_obj;
public:
    ScrollBarVert(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_);

    ~ScrollBarVert() = default;

    void draw(IRenderWindow* renderWindow) override;

    // virtual bool update(const IRenderWindow* renderWindow, const Event& event);

    wid_t getId() const override;

    IWindow* getWindowById(wid_t id) override;

    const IWindow* getWindowById(wid_t id) const override;

    vec2i getPos() const override;

    vec2u getSize() const override;

    void setParent(const IWindow* parent) override;

    void forceActivate() override;

    void forceDeactivate() override;

    bool isActive() const override;

    void addWindow(std::unique_ptr<IWindow> window) override;

    void removeWindow(wid_t id) override;

    bool isWindowContainer() const override;

    void finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const override;

    bool unPressAllButtons() override;

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow_, const Event& event_) override;

    void setSize(const vec2u& size_) override;
    void setPos(const vec2i& pos_) override;

    Scrollable* getScrollObject() const {
        return scroll_obj;
    }

};

class ScrollBarSlider : public ABarButton {
private:
    friend class ScrollBarArrUp;
    friend class ScrollBarArrDown;
    friend class ScrollBarSliderAction;
    bool is_dragging;
    vec2i drag_offset;
    Scrollable* scroll_obj;

    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    ScrollBarSlider(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    virtual ~ScrollBarSlider() = default;
};

class ScrollBarArrUp : public ABarButton {
    Scrollable* scroll_obj;
    friend class ScrollBarArrUpAction;

    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    ScrollBarArrUp(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_, const std::string& file);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;


    ~ScrollBarArrUp() = default;
};

class ScrollBarArrDown : public ABarButton {
private:
    Scrollable* scroll_obj;
    friend class ScrollBarArrDownAction;
    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    ScrollBarArrDown(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_, const std::string& file);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    ~ScrollBarArrDown() = default;
};



class ScrollBarVertAction : public AAction {
    ScrollBarVert* scrollbar;
public:
    ScrollBarVertAction(ScrollBarVert* scrollbar_, const IRenderWindow *renderWindow_, const Event &event_);
    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

class ScrollBarSliderAction : public AAction {
    ScrollBarSlider* slider;
public:
    ScrollBarSliderAction(ScrollBarSlider* slider_, const IRenderWindow *renderWindow_, const Event &event_);
    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

class ScrollBarArrDownAction : public AAction {
    ScrollBarArrDown* arr_down;
public:
    ScrollBarArrDownAction(ScrollBarArrDown* arr_down_, const IRenderWindow *renderWindow_, const Event &event_);
    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

class ScrollBarArrUpAction : public AAction {
   ScrollBarArrUp* arr_up;
public:
    ScrollBarArrUpAction(ScrollBarArrUp* arr_up_, const IRenderWindow *renderWindow_, const Event &event_);
    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

}
#endif
