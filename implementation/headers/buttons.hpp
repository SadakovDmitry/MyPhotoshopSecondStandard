#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "actions.hpp"
#include "my_sfm.hpp"

#include <cstdint>
#include <memory>
#include <iostream>
#include <queue>

namespace psapi {

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         ABarButton
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

class ABarButton : public IBarButton {
protected:
    friend class AMenu;
    wid_t id;
    bool is_active;
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
    virtual std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

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
};

class ABarButtonAction : public AAction
{
    ABarButton *button;
public:
    ABarButtonAction(ABarButton *button_, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         AMenu
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

class AMenu : public IBar {
private:
    friend class AMenuButton;
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
    AMenu(vec2i pos_, vec2u size_) : id(1), is_active(false), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr), toolbar() {
        // if(!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/Pencil.png")) {
        //     //throw std::runtime_error("ошибка открытия файла > " + file + "!");
        // }
        texture.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y));
        std::vector<sfm::Color> pix_arr(size.x * size.y, sfm::Color(255, 255, 255, 255));
        texture.loadFromMemory(pix_arr.data(), size.x * size.y, sfm::IntRect({0, 0}, size));
        sprite.setTexture(&texture);
        sfm::IntRect rect;
        rect.pos = {0, 0};
        rect.size = size;
        sprite.setTextureRect(rect);
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    ~AMenu() = default;

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

class AMenuAction : public AAction {
    std::vector<std::unique_ptr<IBarButton>>* toolbar;
public:
    AMenuAction(std::vector<std::unique_ptr<IBarButton>>* toolbar_, const IRenderWindow *renderWindow_, const Event &event_);
    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         AMenuButton
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

class AMenuButton : public IMenuButton {
protected:
    friend class MenuBar;
    friend class AMenuButtonAction;
    wid_t id;
    bool is_active = true;
    sfm::Sprite sprite;
    sfm::Texture texture;
    vec2u size;
    vec2i pos;
    vec2f scale;
    State state;
    const IWindow* parent;
    AMenu* menu;
public:
    AMenuButton(vec2i pos_, vec2u size_, wid_t id_) : id(id_), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr), menu(nullptr) {}
    AMenuButton() = default;

    virtual ~AMenuButton() = default;

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

    virtual void addMenuItem(std::unique_ptr<IWindow> item) override;

    virtual void activateMenu() override;
    virtual void deactivateMenu() override;

    virtual IBar *getMenu() override;
    virtual const IBar *getMenu() const override;
};

class AMenuButtonAction : public AAction
{
    AMenuButton *button;
public:
    AMenuButtonAction(AMenuButton *button_, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

}
#endif
