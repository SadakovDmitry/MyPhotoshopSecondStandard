#ifndef FILES_HPP
#define FILES_HPP

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
#include <vector>
#include <dlfcn.h>

namespace psapi {

class FileButton : public AMenuButton {
private:
    friend class FileButtonAction;

    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    FileButton(vec2i pos_, vec2u size_, wid_t id_)
        : AMenuButton(pos_, size_, id_) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/Files.png")) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
        vec2i menu_pos = {pos.x + static_cast<int>(getSize().x), pos.y + static_cast<int>(getSize().y)};
        vec2u menu_size = {size.x, 50};
        menu = new AMenu(menu_pos, menu_size);
    }

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
    LoadTool(vec2i pos_, vec2u size_, wid_t id_)
        : ABarButton(pos_, size_, id_) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/load.png")) {
            std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }


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
    SaveTool(vec2i pos_, vec2u size_, wid_t id_)
        : ABarButton(pos_, size_, id_) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/save.png")) {
            std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }

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
