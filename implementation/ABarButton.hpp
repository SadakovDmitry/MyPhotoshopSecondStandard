#ifndef BAR_BUTTON_HPP
#define BAR_BUTTON_HPP

#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "actions.hpp"
#include "my_sfm.hpp"

#include <cstdint>
#include <memory>
#include <iostream>
#include <queue>

namespace psapi {

class ABarButton : public IBarButton {
protected:
    friend class ToolBar;
    wid_t id;
    bool is_active = true;
    sfm::Sprite sprite;
    sfm::Texture texture;
    vec2u size;
    vec2i pos;
    vec2f scale;
    State state;
    const IWindow* parent;
    friend class ABarButtonAction;
public:
    //ABarButton();
    // ABarButton(vec2i pos_, vec2u size_, wid_t id_) : id(id_), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr) {
    //     // if(!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/Pencil.png")) {
    //     //     //throw std::runtime_error("ошибка открытия файла > " + file + "!");
    //     // }
    //     // sprite.setTexture(&texture);
    //     // sprite.setTextureRect(sfm::IntRect(0, 0, 50, 50));
    //     // sprite.setScale(1, 1);
    //     // sprite.setColor(sfm::Color(255, 255, 255, 255));
    //     // sprite.setPosition(5, 5);
    // }
    ABarButton(vec2i pos_, vec2u size_, wid_t id_) : id(id_), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr) {}
    ABarButton() = default;

    virtual ~ABarButton() = default;

    virtual void draw(IRenderWindow* renderWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

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

    virtual void setState(State state) override;
    virtual State getState() const override;

    // virtual void action() = 0;
};

class ABarButtonAction : public AAction
{
    ABarButton *button;
public:
    ABarButtonAction(ABarButton *button_, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


}
#endif
