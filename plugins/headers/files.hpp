#ifndef FILES_HPP
#define FILES_HPP

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
#include <vector>
#include <dlfcn.h>

namespace psapi {

class FileButton : public AMenuButton {
private:
    friend class FileButtonAction;

    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    FileButton(vec2i pos_, vec2u size_, wid_t id_, const std::string& file);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    ~FileButton() = default;
};

class FileButtonAction : public AAction {
    FileButton *button;
public:
    FileButtonAction(FileButton *button_, const IRenderWindow *render_window_, const Event &event_);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;
};

class LoadTool : public ABarButton {
private:
    friend class LoadToolAction;
    sfm::Image loaded_image;

    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    LoadTool(vec2i pos_, vec2u size_, wid_t id_, const std::string& file);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    ~LoadTool() = default;
};

class LoadToolAction : public AAction {
    LoadTool *load;

    void drawImage(ILayer* temp_layer);
public:
    LoadToolAction(LoadTool *load_, const IRenderWindow *render_window_, const Event &event_);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;
};

class SaveTool : public ABarButton {
private:
    friend class SaveToolAction;

    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    SaveTool(vec2i pos_, vec2u size_, wid_t id_, const std::string& file);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    ~SaveTool() = default;
};

class SaveToolAction : public AAction {
    SaveTool *save_tool;
public:
    SaveToolAction(SaveTool *save_tool_, const IRenderWindow *render_window_, const Event &event_);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;
};

extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

}

#endif
