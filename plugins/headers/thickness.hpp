#ifndef THICKNESS_HPP
#define THICKNESS_HPP

#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/headers/my_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_actions.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/headers/photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/headers/canvas.hpp"

#include <cstdint>
#include <memory>
#include <iostream>
#include <assert.h>

namespace psapi {

class ThicknessOption : public IThicknessOption {
private:
    float thickness;

    sfm::Texture texture;
    sfm::Sprite sprite;
    sfm::Texture cursor_texture;
    sfm::Sprite cursor_sprite;
    vec2i last_mouse_pos;
    friend class ThicknessOptionAction;
    wid_t id;
    bool is_active;
    bool cursor_is_dragging = false;
    vec2i pos;
    vec2i cursor_pos;
    vec2u size;
    vec2u cursor_size;
    vec2f scale;
    IBarButton::State state;
    const IWindow* parent;

    void updateState(const IRenderWindow *render_window_, const Event &event_);
public:
    ThicknessOption(vec2i pos_, vec2u size_, const std::string& bar_file, const std::string& cursor_file);

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow_, const Event &event_) override;

    virtual float getThickness() const override;
    virtual void setThickness(float thickness_) override;

    void draw(IRenderWindow* renderWindow) override;

    virtual wid_t getId() const override;
    virtual IWindow* getWindowById(wid_t id) override;
    virtual const IWindow* getWindowById(wid_t id) const override;
    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;

    virtual void setParent(const IWindow* parent) override;
    virtual void setSize(const vec2u &size) override;
    virtual void setPos(const vec2i &pos) override;

    virtual void forceActivate() override;
    virtual void forceDeactivate() override;

    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

    void setState(ABarButton::State state);
    IBarButton::State getState() const;

    virtual ~ThicknessOption() = default;
};

class ThicknessOptionAction : public AAction {
    ThicknessOption *thickness_bar;
public:
    ThicknessOptionAction(ThicknessOption *thickness_bar_, const IRenderWindow *render_window_, const Event &event_);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;
};

extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

}

#endif
