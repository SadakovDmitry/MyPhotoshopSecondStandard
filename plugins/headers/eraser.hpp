#ifndef ERASER_HPP
#define ERASER_HPP

//#include "api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/my_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_actions.hpp"

#include <cstdint>
#include <memory>
#include <iostream>
#include <assert.h>

namespace psapi {

// const wid_t kToolBarWindowId    = 101;
// const wid_t kOptionsBarWindowId = 102;

class EraserTool : public ABarButton {
private:
    friend class EraserAction;
    sfm::Color color;
    float thickness;
    std::vector<vec2i> points_arr;

    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    EraserTool(vec2i pos_, vec2u size_, wid_t id_)
        : ABarButton(pos_, size_, id_) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/Eraser.png")) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
        color = {255, 255, 255, 0};
    }

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    sfm::Color getColor();
    void setColor(sfm::Color);

    ~EraserTool() = default;

};

class EraserAction : public AUndoableAction {
    EraserTool *eraser;
    sfm::vec2i last_pos_ = sfm::vec2i(0, 0);
public:
    EraserAction(EraserTool *eraser_, const IRenderWindow *render_window_, const Event &event_);

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;

private:
    double CatmullRom(double p0, double p1, double p2, double p3, double t);
    vec2i operator() (double t);
};

extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

}

#endif