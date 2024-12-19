
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "thickness.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

#include <assert.h>

namespace psapi {

    float ThicknessOption::getThickness() const {
        return thickness;
    }

    void ThicknessOption::setThickness(float thickness_) {
        thickness = thickness_;
    }

    void ThicknessOption::draw(IRenderWindow* renderWindow) {
        renderWindow->draw(&sprite);
        renderWindow->draw(&cursor_sprite);
    }

    std::unique_ptr<IAction> ThicknessOption::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<ThicknessOptionAction>(this, renderWindow_, event_);
    }

    void ThicknessOption::updateState(const IRenderWindow *render_window_, const Event &event_) {
        vec2i mouse_pos = sfm::Mouse::getPosition(render_window_);
        vec2i pos = this->getPos();
        vec2u size = this->getSize();

        if (pos.x <= mouse_pos.x && mouse_pos.x <= pos.x + size.x &&
            pos.y <= mouse_pos.y && mouse_pos.y <= pos.y + size.y) {
            if (event_.type == Event::MouseButtonPressed) {
                this->state = IBarButton::State::Press;
                cursor_is_dragging = true;
            } else if (event_.type == Event::MouseButtonReleased) {
                this->state = IBarButton::State::Released;
                cursor_is_dragging = false;
            } else {
                this->state = IBarButton::State::Hover;
            }
        } else if (event_.type == Event::MouseButtonReleased) {
            this->state = IBarButton::State::Released;
            cursor_is_dragging = false;
        } else {
            this->state = IBarButton::State::Normal;
            cursor_is_dragging = false;
        }

        if (cursor_is_dragging) {
            last_mouse_pos = mouse_pos;
        }
    }

    ThicknessOptionAction::ThicknessOptionAction(ThicknessOption *thickness_bar_, const IRenderWindow *render_window_, const Event &event_)
    :   AAction(render_window_, event_), thickness_bar(thickness_bar_) {}

    bool ThicknessOptionAction::isUndoable(const Key &key) {
        return true;
    }

    bool ThicknessOptionAction::execute(const Key &key) {
        thickness_bar->updateState(render_window, event);
        if (thickness_bar->cursor_is_dragging) {
            Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
            vec2i mouse_pos = canvas->getMousePosition();
            vec2u rel_pos = {static_cast<unsigned int>(mouse_pos.x + canvas->layer_pos.x - thickness_bar->pos.x),
                             static_cast<unsigned int>(mouse_pos.y + canvas->layer_pos.y - thickness_bar->pos.y)};
            thickness_bar->thickness = rel_pos.x;

            thickness_bar->cursor_sprite.setPosition(thickness_bar->last_mouse_pos.x - 5, thickness_bar->pos.y + 22);
        }
        return true;
    }

    wid_t ThicknessOption::getId() const {
        return id;
    }

    IWindow* ThicknessOption::getWindowById(wid_t id) {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    const IWindow* ThicknessOption::getWindowById(wid_t id) const {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    vec2i ThicknessOption::getPos() const {
        return pos;
    }

    vec2u ThicknessOption::getSize() const {
        return size;
    }

    void ThicknessOption::setParent(const IWindow* parent) {
       this->parent = parent;
    }

    void ThicknessOption::forceActivate() {is_active = true;}

    void ThicknessOption::forceDeactivate() {is_active = false;}

    bool ThicknessOption::isActive() const { return is_active == true; }

    bool ThicknessOption::isWindowContainer() const {
        return false;
    }

    void ThicknessOption::setSize(const vec2u& size) {
        this->size = size;
    }
    void ThicknessOption::setPos(const vec2i& pos) {
        this->pos = pos;
    }

    void ThicknessOption::setState(ABarButton::State state) {
        this->state = state;
    }

    IBarButton::State ThicknessOption::getState() const {
        return state;
    }

    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {
//             ChildInfo info_palette;
//             info_palette.pos = {105, 5};
//             info_palette.size = {150, 150};
            auto optionsbar = static_cast<IOptionsBar*>(getRootWindow()->getWindowById(kOptionsBarWindowId));
            assert(optionsbar);
            vec2i pos = {optionsbar->getPos().x, optionsbar->getPos().y + 100};
            vec2u size = {100, 60};
            auto thickness = std::make_unique<ThicknessOption>(pos, size);
            assert(thickness);
            optionsbar->addWindow(std::move(thickness));
//             ChildInfo info_thicknessWindow;
//             info_thicknessWindow.pos = {255, 5};
//             info_thicknessWindow.size = {200, 120};
//             auto thicknessWindow = std::make_unique<ChoiceThicknessWindow>(info_thicknessWindow.pos, info_thicknessWindow.size, 201);
//
//             auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
//             ChildInfo info = toolbar->getNextChildInfo();
//             auto pencil = std::make_unique<PencilTool>(info.pos, info.size, 1, palette.get(), thicknessWindow.get());
//
//             if (toolbar) {
//                 toolbar->addWindow(std::move(pencil));
//                 getRootWindow()->addWindow(std::move(palette));
//                 getRootWindow()->addWindow(std::move(thicknessWindow));
//                 // toolbar->addWindow(std::move(palette));
//                 //return true;
//             }
//             assert(toolbar && "pnacil plugin");
//
//             auto thicknessW = static_cast<ChoiceThicknessWindow*>(getRootWindow()->getWindowById(201));
//             vec2u size = {thicknessW->getSize().x, 10};
//             vec2i pos = {static_cast<int>(thicknessW->getPos().x),
//                          static_cast<int>(thicknessW->getPos().y + thicknessW->getSize().y)};
//             auto scroll_obj = static_cast<Scrollable*>(thicknessW);
//             scroll_obj->setShift({thicknessW->getPos().x, thicknessW->getPos().y});
//             auto scrollbar = std::make_unique<ScrollBarHor>(pos, size, 300, scroll_obj);
//             getRootWindow()->addWindow(std::move(scrollbar));
//             toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(300));
//             assert(toolbar);
//
//             size = {10, 10};
//             pos = {static_cast<int>(thicknessWindow->getPos().x),
//                    static_cast<int>(thicknessWindow->getPos().y + thicknessWindow->getSize().y)};
//             auto slider = std::make_unique<ScrollBarSlider>(pos, size, 1, scroll_obj);
//
//             size = {toolbar->getSize().y, toolbar->getSize().y};
//             pos = {static_cast<int>(thicknessWindow->getPos().x + thicknessWindow->getSize().x - size.x),
//                    static_cast<int>(thicknessWindow->getPos().y + thicknessWindow->getSize().y)};
//             auto arr_up = std::make_unique<ScrollBarArrRight>(pos, size, 2, scroll_obj, "arrow_right.jpg");
//
//             pos = {static_cast<int>(thicknessWindow->getPos().x),
//                    static_cast<int>(thicknessWindow->getPos().y + thicknessWindow->getSize().y)};
//             auto arr_down = std::make_unique<ScrollBarArrLeft>(pos, size, 3, scroll_obj, "arrow_left.jpg");
//
//             toolbar->addWindow(std::move(slider));
//             toolbar->addWindow(std::move(arr_up));
//             toolbar->addWindow(std::move(arr_down));

            return true;
        }

        __attribute__((visibility("default"))) void unloadPlugin() {
            IBar* toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kThicknessBarId));
            if (toolbar) {
                toolbar->removeWindow(1);
            }
        }
    }
}
