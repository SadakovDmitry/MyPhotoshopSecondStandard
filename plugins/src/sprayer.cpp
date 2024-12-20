
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "../headers/sprayer.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {

    SprayerTool::SprayerTool(vec2i pos_, vec2u size_, wid_t id_, const std::string& file)
        : ABarButton(pos_, size_, id_) {
        if (!texture.loadFromFile(file)) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
        color = {0, 255, 0, 255};
    }

    std::unique_ptr<IAction> SprayerTool::createAction(const IRenderWindow *renderWindow, const Event &event) {
        return std::make_unique<SprayerAction>(this, renderWindow, event);
    }

    void SprayerTool::updateState(const IRenderWindow *render_window, const Event &event) {
        getActionController()->execute(ABarButton::createAction(render_window, event));
    }

    SprayerAction::SprayerAction(SprayerTool *init_spray, const IRenderWindow *render_window_, const Event &event_)
    :   AUndoableAction(render_window_, event_), spray(init_spray) {}

    bool SprayerAction::undo(const Key &key) {
        std::cout << "Undo spray";
        return true;
    }

    bool SprayerAction::redo(const Key &key) {
        std::cout << "Redo spray";
        return true;
    }

    bool SprayerAction::isUndoable(const Key &key) {
        return true;
    }

    bool SprayerAction::execute(const Key &key)
    {
        spray->updateState(render_window, event);
        srand(time(0));
        ICanvas* canvas = static_cast<ICanvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        vec2i mouse_pos    = canvas->getMousePosition();
        vec2i canvas_pos   = canvas->getPos();
        vec2i cur_pos;

        if (canvas->isPressedLeftMouseButton() && (spray->getState() == IBarButton::State::Press)) {
            IColorPalette* color_palette = static_cast<IColorPalette*>(getRootWindow()->getWindowById(kOptionsBarWindowId)->getWindowById(kColorPaletteId));
            IOpacityOption* capacity_bar = static_cast<IOpacityOption*>(getRootWindow()->getWindowById(kOptionsBarWindowId)->getWindowById(kOpacityBarId));
            if(color_palette) {
                spray->color.r = static_cast<int>(color_palette->getColor().r);
                spray->color.g = static_cast<int>(color_palette->getColor().g);
                spray->color.b = static_cast<int>(color_palette->getColor().b);
                spray->color.a = static_cast<int>(capacity_bar->getOpacity());
            }

            for (int i = 0; i < 30; i++) {
                cur_pos.x = mouse_pos.x + (rand() % 20);
                cur_pos.y = mouse_pos.y + (rand() % 20);
                temp_layer->setPixel(cur_pos, spray->color);
            }
        }
        return true;
    }

    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {

            auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            // ChildInfo info = toolbar->getNextChildInfo();
            vec2i pos = {toolbar->getPos().x, toolbar->getPos().y};
            vec2u size = {50, 50};
            auto tool = std::make_unique<SprayerTool>(pos, size, 3, "/Users/dima/MIPT/SecondSem/MyPaint2.0/source/Sprayer.jpg");

            if (toolbar) {
                toolbar->addWindow(std::move(tool));
                std::cout << "add Window Succsesfull\n";
                return true;
            }
            std::cout << "not add Window Succsesfull\n";
            assert(toolbar);
            return false;
        }

        __attribute__((visibility("default"))) void unloadPlugin() {
            IBar* toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            if (toolbar) {
                toolbar->removeWindow(1);
            }
        }
    }
}
