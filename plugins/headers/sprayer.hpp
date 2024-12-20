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

class SprayerTool : public ABarButton {
    friend class SprayerAction;
    sfm::Color color;

    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    SprayerTool(vec2i pos_, vec2u size_, wid_t id_, const std::string& file);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    ~SprayerTool() = default;

};

class SprayerAction : public AUndoableAction {
    SprayerTool *spray;
    sfm::vec2i last_pos_ = sfm::vec2i(0, 0);
public:
    SprayerAction(SprayerTool *init_brush, const IRenderWindow *render_window_, const Event &event_);

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;
};

extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

}

#endif
