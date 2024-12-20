
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "../headers/pencil.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {

    std::unique_ptr<IAction> PencilTool::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<PencilAction>(this, renderWindow_, event_);
    }

    void PencilTool::updateState(const IRenderWindow *render_window_, const Event &event_) {
        getActionController()->execute(ABarButton::createAction(render_window_, event_));
    }

    sfm::Color PencilTool::getColor() {
        return color;
    }

    void PencilTool::setColor(sfm::Color col) {
        color = col;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      PencilAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    double PencilAction::CatmullRom(double p0, double p1, double p2, double p3, double t){
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

    vec2i PencilAction::operator() (double t){
        std::vector<vec2i>::iterator iter = pencil->points_arr.begin();
        int i = t / 1;
        iter += i;
        t -= i;
        vec2i temp_point {0, 0};

        temp_point.x = CatmullRom((*(iter - 1)).x, (*iter).x, (*(iter + 1)).x, (*(iter + 2)).x, t);
        temp_point.y = CatmullRom((*(iter - 1)).y, (*iter).y, (*(iter + 1)).y, (*(iter + 2)).y, t);
        return temp_point;
    }

    PencilAction::PencilAction(PencilTool *pencil_, const IRenderWindow *render_window_, const Event &event_)
    :   AUndoableAction(render_window_, event_), pencil(pencil_) {}

    bool PencilAction::undo(const Key &key) {
        std::cout << "Undo spray";
        return true;
    }

    bool PencilAction::redo(const Key &key) {
        std::cout << "Redo spray";
        return true;
    }

    bool PencilAction::isUndoable(const Key &key) {
        return true;
    }

    bool PencilAction::execute(const Key &key)
    {
        pencil->updateState(render_window, event);
        ICanvas* canvas = static_cast<ICanvas*>(getRootWindow()->getWindowById(kCanvasWindowId));

        IThicknessOption* thickness_bar = static_cast<IThicknessOption*>(getRootWindow()->getWindowById(kOptionsBarWindowId)->getWindowById(kThicknessBarId));
        if(thickness_bar) {
            pencil->thickness = thickness_bar->getThickness();
        }

        ILayer* temp_layer = canvas->getTempLayer();
        vec2i mouse_pos    = canvas->getMousePosition();
        vec2i canvas_pos   = canvas->getPos();
        vec2i cur_pos;

        if (pencil->getState() == IBarButton::State::Press) {
            IColorPalette* color_palette = static_cast<IColorPalette*>(getRootWindow()->getWindowById(kOptionsBarWindowId)->getWindowById(kColorPaletteId));
            IOpacityOption* capacity_bar = static_cast<IOpacityOption*>(getRootWindow()->getWindowById(kOptionsBarWindowId)->getWindowById(kOpacityBarId));
            if(color_palette) {
                pencil->color.r = static_cast<int>(color_palette->getColor().r);
                pencil->color.g = static_cast<int>(color_palette->getColor().g);
                pencil->color.b = static_cast<int>(color_palette->getColor().b);
                pencil->color.a = static_cast<int>(capacity_bar->getOpacity());
            }
            if (pencil->points_arr.size() < 4) {
                pencil->points_arr.push_back(mouse_pos);
            } else {
                int radius = static_cast<int>(pencil->thickness);
                vec2i last_pos = pencil->points_arr[3];
                pencil->points_arr.erase(pencil->points_arr.begin());
                pencil->points_arr.push_back(mouse_pos);

    	        for (double t = 0; t <= 1; t += 0.001) {
                    vec2i point = (*this)(t + 1);
                    if (canvas->isPressedLeftMouseButton()) {
                        for (int i = radius * -1; i < radius; i++) {
                            for (int j = radius * -1; j < radius; j++) {
                                if (i * i + j * j <= radius * radius / 4) {
                                    vec2i temp_point = {point.x + i, point.y + j};
                                    temp_layer->setPixel(temp_point, pencil->color);
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
            vec2i pos = {toolbar->getPos().x + 50, toolbar->getPos().y};
            vec2u size = {50, 50};
            auto tool = std::make_unique<PencilTool>(pos, size, 1);

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
