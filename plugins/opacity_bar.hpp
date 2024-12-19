#ifndef THICKNESS_HPP
#define THICKNESS_HPP

#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/my_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_actions.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/canvas.hpp"

#include <cstdint>
#include <memory>
#include <iostream>
#include <assert.h>

namespace psapi {

class OpacityOption : public IOpacityOption {
private:
    float opacity;

    sfm::Texture texture;
    sfm::Sprite sprite;
    sfm::Texture cursor_texture;
    sfm::Sprite cursor_sprite;
    vec2i last_mouse_pos;
    friend class OpacityOptionAction;
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
    OpacityOption(vec2i pos_, vec2u size_)
        : pos(pos_), size(size_), id(kOpacityBarId), opacity(255) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/opacity_bar.png")) {
            std::cerr << "Error loading palette image\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setPosition(pos.x, pos.y);

        if (!cursor_texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/thickness_cursor.png")) {
            std::cerr << "Error loading palette image\n";
        }

        cursor_sprite.setTexture(&cursor_texture);
        cursor_sprite.setTextureRect(sfm::IntRect({0, 0}, {10, 14}));
        cursor_sprite.setPosition({static_cast<float>(pos.x + size.x - 10), pos.y + static_cast<float>(size.y / 2 - 8)});
        cursor_sprite.setScale(1, 1);
    }

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow_, const Event &event_) override;

    virtual float getOpacity() const override;
    virtual void setOpacity(float opacity) override;

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

    virtual ~OpacityOption() = default;
};

class OpacityOptionAction : public AAction {
    OpacityOption *opacity_bar;
public:
    OpacityOptionAction(OpacityOption *opacity_bar_, const IRenderWindow *render_window_, const Event &event_);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;
};

extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

}

#endif
