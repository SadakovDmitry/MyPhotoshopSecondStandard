
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "../headers/eraser.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {

    EraserTool::EraserTool(vec2i pos_, vec2u size_, wid_t id_, const std::string& file)
        : ABarButton(pos_, size_, id_) {
        if (!texture.loadFromFile(file)) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);

        if (!hovered_texture.loadFromFile(file)) {
            std::cerr << "Error opening file: " << file << "\n";
        }
        hovered_sprite.setTexture(&hovered_texture);
        hovered_sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        hovered_sprite.setScale(1, 1);
        hovered_sprite.setColor(sfm::Color(200, 200, 200, 200));
        hovered_sprite.setPosition(pos.x, pos.y);

        if (!pressed_texture.loadFromFile(file)) {
            std::cerr << "Error opening file: " << file << "\n";
        }
        pressed_sprite.setTexture(&pressed_texture);
        pressed_sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        pressed_sprite.setScale(1, 1);
        pressed_sprite.setColor(sfm::Color(155, 155, 155, 255));
        pressed_sprite.setPosition(pos.x, pos.y);

        if (!released_texture.loadFromFile(file)) {
            std::cerr << "Error opening file: " << file << "\n";
        }
        released_sprite.setTexture(&released_texture);
        released_sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        released_sprite.setScale(1, 1);
        released_sprite.setColor(sfm::Color(255, 255, 255, 255));
        released_sprite.setPosition(pos.x, pos.y);

        color = {255, 255, 255, 0};
    }

    std::unique_ptr<IAction> EraserTool::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<EraserAction>(this, renderWindow_, event_);
    }

    void EraserTool::updateState(const IRenderWindow *render_window_, const Event &event_) {
        vec2i mouse_pos = sfm::Mouse::getPosition(render_window_);
        vec2i pos = getPos();
        vec2u size = getSize();

        if (pos.x <= mouse_pos.x && mouse_pos.x <= pos.x + size.x &&
            pos.y <= mouse_pos.y && mouse_pos.y <= pos.y + size.y)
        {
            if (event_.type == Event::MouseButtonPressed) {
                if (state != IBarButton::State::Press) {
                    state = IBarButton::State::Press;
                    //static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId))->unPressAllButtons();
                } else {
                    state = IBarButton::State::Released;
                }
            } else if (state != IBarButton::State::Press) {
                state = psapi::IBarButton::State::Hover;
            }
        } else if (state == psapi::IBarButton::State::Hover || state == psapi::IBarButton::State::Released) {
            state = psapi::IBarButton::State::Normal;
        }
    }

    sfm::Color EraserTool::getColor() {
        return color;
    }

    void EraserTool::setColor(sfm::Color col) {
        color = col;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      EraserAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    double EraserAction::CatmullRom(double p0, double p1, double p2, double p3, double t){
        double t2   = t * t;
        double t3   = t2 * t;
        double t3_2 = t3 * 2;
        double t2_3 = t2 * 3;

        double hermite00 =  t3_2 - t2_3 + 1;
        double hermite10 =  t3   - t2 * 2 + t;
        double hermite01 = -t3_2 + t2_3;
        double hermite11 =  t3   - t2;

        double m0 = (p2 - p0) / 2;
        double m1 = (p3 - p1) / 2;

        return std::max(hermite00 * p1 + hermite10 * m0 * 1.0 +
               hermite01 * p2 + hermite11 * m1 * 1.0, 0.0);
    }

    vec2i EraserAction::operator() (double t){
        std::vector<vec2i>::iterator iter = eraser->points_arr.begin();
        int i = t / 1;
        iter += i;
        t -= i;
        vec2i temp_point {0, 0};

        temp_point.x = CatmullRom((*(iter - 1)).x, (*iter).x, (*(iter + 1)).x, (*(iter + 2)).x, t);
        temp_point.y = CatmullRom((*(iter - 1)).y, (*iter).y, (*(iter + 1)).y, (*(iter + 2)).y, t);
        return temp_point;
    }

    EraserAction::EraserAction(EraserTool *eraser_, const IRenderWindow *render_window_, const Event &event_)
    :   AUndoableAction(render_window_, event_), eraser(eraser_) {}

    bool EraserAction::undo(const Key &key) {
        std::cout << "Undo spray";
        return true;
    }

    bool EraserAction::redo(const Key &key) {
        std::cout << "Redo spray";
        return true;
    }

    bool EraserAction::isUndoable(const Key &key) {
        return true;
    }

    bool EraserAction::execute(const Key &key)
    {
        eraser->updateState(render_window, event);

        if (eraser->getState() == IBarButton::State::Press) {
            ICanvas* canvas = static_cast<ICanvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
            ILayer* temp_layer = canvas->getTempLayer();
            vec2i mouse_pos    = canvas->getMousePosition();

            IThicknessOption* thickness_bar = static_cast<IThicknessOption*>(getRootWindow()->getWindowById(kOptionsBarWindowId)->getWindowById(kThicknessBarId));
            if(thickness_bar) {
                eraser->thickness = thickness_bar->getThickness();
            }

            if (eraser->points_arr.size() < 4) {
                eraser->points_arr.push_back(mouse_pos);
            } else {
                int radius = static_cast<int>(eraser->thickness);
                vec2i last_pos = eraser->points_arr[3];
                eraser->points_arr.erase(eraser->points_arr.begin());
                eraser->points_arr.push_back(mouse_pos);

    	        for (double t = 0; t <= 1; t += 0.001) {
                    vec2i point = (*this)(t + 1);
                    if (canvas->isPressedLeftMouseButton()) {
                        for (int i = radius * -1; i < radius; i++) {
                            for (int j = radius * -1; j < radius; j++) {
                                if (i * i + j * j <= radius * radius / 4) {
                                    vec2i temp_point = {point.x + i, point.y + j};
                                    temp_layer->setPixel(temp_point, eraser->color);
                                }
                            }
                        }
                    }
                }
            }
        }
        return true;
    }

    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {
            auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            // ChildInfo info = toolbar->getNextChildInfo();
            vec2i pos = {toolbar->getPos().x, toolbar->getPos().y + 50};
            vec2u size = {50, 50};
            auto tool = std::make_unique<EraserTool>(pos, size, 2, "/Users/dima/MIPT/SecondSem/MyPaint2.0/images/eraser.png");

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
