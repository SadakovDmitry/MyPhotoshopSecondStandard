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
        // if(!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/Pencil.png")) {
        //     //throw std::runtime_error("ошибка открытия файла > " + file + "!");
        // }
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
    virtual void removeAllOptions();
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
