#ifndef MENUBAR_HPP
#define MENUBAR_HPP

#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "my_sfm.hpp"
#include "buttons.hpp"

#include <cstdint>
#include <memory>
#include <iostream>
#include <queue>

namespace psapi {

class MenuBar : public IBar {
private:
    wid_t id;
    bool is_active;
    vec2i pos;
    vec2u size;
    vec2f scale;
    sfm::Sprite sprite;
    sfm::Texture texture;
    const IWindow* parent;
    std::vector<std::unique_ptr<IMenuButton>> menubar;
public:
    MenuBar(vec2i pos_, vec2u size_) : id(kMenuBarWindowId), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr), menubar() {
        // if(!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/Pencil.png")) {
        //     //throw std::runtime_error("ошибка открытия файла > " + file + "!");
        // }
        sprite.setTexture(&texture);
        sfm::IntRect rect;
        rect.pos = {0, 0};
        rect.size = size;
        sprite.setTextureRect(rect);
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(155, 155, 155, 255));
        sprite.setPosition(pos.x, pos.y);
    }
    ~MenuBar() = default;

    virtual void draw(IRenderWindow* renderWindow);

    // virtual bool update(const IRenderWindow* renderWindow, const Event& event);

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

class MenuBarAction : public AAction {
    std::vector<std::unique_ptr<IMenuButton>>* menubar;
public:
    MenuBarAction(std::vector<std::unique_ptr<IMenuButton>>* toolbar_, const IRenderWindow *renderWindow_, const Event &event_);
    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

}

#endif
