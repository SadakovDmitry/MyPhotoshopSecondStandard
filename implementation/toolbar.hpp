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
#include "ABarButton.hpp"

#include <cstdint>
#include <memory>
#include <iostream>
#include <queue>

namespace psapi {

// const wid_t kToolBarWindowId    = 101;
// const wid_t kOptionsBarWindowId = 102;

// class ABarButton : public IBarButton {
// protected:
//     friend class ToolBar;
//     wid_t id;
//     bool is_active = true;
//     sfm::Sprite sprite;
//     sfm::Texture texture;
//     vec2u size;
//     vec2i pos;
//     vec2f scale;
//     State state;
//     const IWindow* parent;
// public:
//     //ABarButton();
//     // ABarButton(vec2i pos_, vec2u size_, wid_t id_) : id(id_), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr) {
//     //     // if(!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/Pencil.png")) {
//     //     //     //throw std::runtime_error("ошибка открытия файла > " + file + "!");
//     //     // }
//     //     // sprite.setTexture(&texture);
//     //     // sprite.setTextureRect(sfm::IntRect(0, 0, 50, 50));
//     //     // sprite.setScale(1, 1);
//     //     // sprite.setColor(sfm::Color(255, 255, 255, 255));
//     //     // sprite.setPosition(5, 5);
//     // }
//     ABarButton(vec2i pos_, vec2u size_, wid_t id_) : id(id_), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr) {}
//     ABarButton() = default;
//
//     virtual ~ABarButton() = default;
//
//     virtual void draw(IRenderWindow* renderWindow) override;
//
//     virtual bool update(const IRenderWindow* renderWindow, const Event& event) override;
//
//     virtual wid_t getId() const override;
//
//     virtual IWindow* getWindowById(wid_t id) override;
//
//     virtual const IWindow* getWindowById(wid_t id) const override;
//
//     virtual vec2i getPos() const override;
//
//     virtual vec2u getSize() const override;
//
//     virtual void setParent(const IWindow* parent) override;
//
//     virtual void forceActivate() override;
//
//     virtual void forceDeactivate() override;
//
//     virtual bool isActive() const override;
//
//     virtual bool isWindowContainer() const override;
//
//     virtual void setState(State state) override;
//     virtual State getState() const override;
//
//     virtual void action() = 0;
// };

// class PencilTool : public ABarButton {
// private:
//     // std::vector<Point> points_arr;
//     std::vector<vec2i> points_arr;
// public:
//     PencilTool(vec2i pos_, vec2u size_, wid_t id_)
//         : ABarButton(pos_, size_, id_),  points_arr() {
//         if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/Pencil.png")) {
//              std::cerr << "Error opening file\n";
//         }
//         sprite.setTexture(&texture);
//         sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
//         sprite.setScale(1, 1);
//         sprite.setColor(sfm::Color(255, 255, 255, 255));
//         sprite.setPosition(pos.x, pos.y);
//     }
//
//     virtual void action() override ;
//
//     // virtual void draw(IRenderWindow* renderWindow) override;
//
//     double CatmullRom(double p0, double p1, double p2, double p3, double t);
//     vec2i operator() (double t);
//
//     virtual ~PencilTool() = default;
// };
//
// class SprayerTool : public ABarButton {
// public:
//     SprayerTool(vec2i pos_, vec2u size_, wid_t id_)
//         : ABarButton(pos_, size_, id_) {
//         if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/Sprayer.jpg")) {
//              std::cerr << "Error opening file\n";
//         }
//         sprite.setTexture(&texture);
//         sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
//         sprite.setScale(1, 1);
//         sprite.setColor(sfm::Color(255, 255, 255, 255));
//         sprite.setPosition(pos.x, pos.y);
//     }
//
//     virtual void action() override ;
//
//     virtual ~SprayerTool() = default;
// };
//
// class EraserTool : public ABarButton {
// public:
//     //ABarButton();
//     EraserTool(vec2i pos_, vec2u size_, wid_t id_)
//         : ABarButton(pos_, size_, id_)
//     {
//         if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/Eraser.png")) {
//             std::cerr << "Error opening file\n";
//         }
//         sprite.setTexture(&texture);
//         sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
//         sprite.setScale(1, 1);
//         sprite.setColor(sfm::Color(255, 255, 255, 255));
//         sprite.setPosition(pos.x, pos.y);
//     }
//
//     virtual void action() override;
//
//     virtual ~EraserTool() = default;
// };

// class ActiveTool {
// public:
//     ActiveTool() : activeTool(nullptr) {}
//
//     void setActiveTool(std::unique_ptr<ABarButton> tool) {
//         activeTool = std::move(tool);
//     }
//
//     void onMouseClick(int x, int y){
//         if (activeTool) {
//             activeTool->action();
//         }
//     }
//
//     void draw(IRenderWindow* renderWindow) {
//         if (activeTool) {
//             activeTool->draw(renderWindow);
//         }
//     }
//
// private:
//     std::unique_ptr<ABarButton> activeTool;
// };

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
    ToolBar(vec2i pos_, vec2u size_) : id(kToolBarWindowId), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr), toolbar() {
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
    ~ToolBar() = default;

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

class ToolBarAction : public AAction {
    std::vector<std::unique_ptr<IBarButton>>* toolbar;
public:
    ToolBarAction(std::vector<std::unique_ptr<IBarButton>>* toolbar_, const IRenderWindow *renderWindow_, const Event &event_);
    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

}

#endif
