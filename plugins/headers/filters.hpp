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
#include <vector>
#include <dlfcn.h>

namespace psapi {

// const wid_t kToolBarWindowId    = 101;
// const wid_t kOptionsBarWindowId = 102;

class FiltersButton : public AMenuButton {
private:
    friend class FiltersButtonAction;

    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    FiltersButton(vec2i pos_, vec2u size_, wid_t id_)
        : AMenuButton(pos_, size_, id_) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/Filters.png")) {
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

    ~FiltersButton() = default;
};

class FiltersButtonAction : public AAction {
    FiltersButton *button;
public:
    FiltersButtonAction(FiltersButton *button_, const IRenderWindow *render_window_, const Event &event_);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;
};

class BlurTool : public ABarButton {
private:
    friend class BlurToolAction;

    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    BlurTool(vec2i pos_, vec2u size_, wid_t id_)
        : ABarButton(pos_, size_, id_) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/blur.png")) {
            std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    // void draw(IRenderWindow* renderWindow) override;

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    ~BlurTool() = default;
};

class BlurToolAction : public AAction {
    BlurTool *blur;
public:
    BlurToolAction(BlurTool *blur_, const IRenderWindow *render_window_, const Event &event_);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;

    std::vector<std::vector<double>> generateGaussKernel(int radius, double sigma);
    sfm::Color applyGaussianBlur(int x, int y, Canvas* canvas, const std::vector<std::vector<double>>& kernel);
};

class BrightnessTool : public ABarButton {
private:
    friend class BrightnessToolAction;

    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    BrightnessTool(vec2i pos_, vec2u size_, wid_t id_)
        : ABarButton(pos_, size_, id_) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/brightness.png")) {
            std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    ~BrightnessTool() = default;
};

class BrightnessToolAction : public AAction {
    BrightnessTool *brightness;
public:
    BrightnessToolAction(BrightnessTool *brightness_, const IRenderWindow *render_window_, const Event &event_);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;

    std::vector<std::vector<double>> generateGaussKernel(int radius, double sigma);
    sfm::Color applySharpening(int x, int y, Canvas* canvas, const std::vector<std::vector<double>>& kernel, double alpha);
};

class NegativeTool : public ABarButton {
private:
    friend class NegativeToolAction;

    void updateState(const IRenderWindow *render_window, const Event &event);
public:
    NegativeTool(vec2i pos_, vec2u size_, wid_t id_)
        : ABarButton(pos_, size_, id_) {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/negative.png")) {
            std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    ~NegativeTool() = default;
};

class NegativeToolAction : public AAction {
    NegativeTool *negative;
public:
    NegativeToolAction(NegativeTool *negative_, const IRenderWindow *render_window_, const Event &event_);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;
};


extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

}

#endif