#ifndef OPTIONSBAR_HPP
#define OPTIONSBAR_HPP

#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "my_sfm.hpp"
#include "buttons.hpp"

#include <cstdint>
#include <memory>
#include <iostream>
#include <queue>

namespace psapi {

class OptionsBar : public IOptionsBar {
private:
    friend class OptionsBarAction;
    wid_t id;
    bool is_active;
    vec2i pos;
    vec2u size;
    vec2f scale;
    sfm::Sprite sprite;
    sfm::Texture texture;
    const IWindow* parent;
    std::vector<std::unique_ptr<IWindow>> optionsbar;
public:
    OptionsBar(vec2i pos_, vec2u size_) : id(kOptionsBarWindowId), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr), optionsbar() {
        sprite.setTexture(&texture);
        sfm::IntRect rect;
        rect.pos = {0, 0};
        rect.size = size;
        sprite.setTextureRect(rect);
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }
    ~OptionsBar() = default;

    virtual void draw(IRenderWindow* renderWindow) override;
    virtual wid_t getId() const override;
    virtual IWindow* getWindowById(wid_t id) override;
    virtual const IWindow* getWindowById(wid_t id) const override;
    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;
    virtual void setParent(const IWindow* parent) override;
    virtual void forceActivate() override;
    virtual void forceDeactivate() override;
    virtual bool isActive() const override;
    virtual void addWindow(std::unique_ptr<IWindow> window) override;
    virtual void removeWindow(wid_t id) override;
    virtual bool isWindowContainer() const override;
    virtual void finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const;
    virtual bool unPressAllButtons();
    virtual std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    virtual void setSize(const vec2u& size) override;
    virtual void setPos(const vec2i& pos) override;
    virtual void removeAllOptions() override;
};

class OptionsBarAction : public AAction {
    std::vector<std::unique_ptr<IWindow>>* optionsbar;
public:
    OptionsBarAction(std::vector<std::unique_ptr<IWindow>>* optionsbar_, const IRenderWindow *renderWindow_, const Event &event_);
    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

}

#endif
