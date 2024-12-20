
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "../headers/opacity_bar.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

#include <assert.h>

namespace psapi {

    float OpacityOption::getOpacity() const {
        return opacity;
    }

    void OpacityOption::setOpacity(float opacity_) {
        opacity = opacity_;
    }

    void OpacityOption::draw(IRenderWindow* renderWindow) {
        renderWindow->draw(&sprite);
        renderWindow->draw(&cursor_sprite);
    }

    std::unique_ptr<IAction> OpacityOption::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<OpacityOptionAction>(this, renderWindow_, event_);
    }

    void OpacityOption::updateState(const IRenderWindow *render_window_, const Event &event_) {
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

    OpacityOptionAction::OpacityOptionAction(OpacityOption *opacity_bar_, const IRenderWindow *render_window_, const Event &event_)
    :   AAction(render_window_, event_), opacity_bar(opacity_bar_) {}

    bool OpacityOptionAction::isUndoable(const Key &key) {
        return true;
    }

    bool OpacityOptionAction::execute(const Key &key) {
        opacity_bar->updateState(render_window, event);
        if (opacity_bar->cursor_is_dragging) {
            Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
            vec2i mouse_pos = canvas->getMousePosition();
            vec2u rel_pos = {static_cast<unsigned int>(mouse_pos.x + canvas->layer_pos.x - opacity_bar->pos.x),
                             static_cast<unsigned int>(mouse_pos.y + canvas->layer_pos.y - opacity_bar->pos.y)};
            opacity_bar->opacity = rel_pos.x * 10 / 9;

            opacity_bar->cursor_sprite.setPosition(opacity_bar->last_mouse_pos.x - 5, opacity_bar->pos.y + 22);
        }
        return true;
    }

    wid_t OpacityOption::getId() const {
        return id;
    }

    IWindow* OpacityOption::getWindowById(wid_t id) {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    const IWindow* OpacityOption::getWindowById(wid_t id) const {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    vec2i OpacityOption::getPos() const {
        return pos;
    }

    vec2u OpacityOption::getSize() const {
        return size;
    }

    void OpacityOption::setParent(const IWindow* parent) {
       this->parent = parent;
    }

    void OpacityOption::forceActivate() {is_active = true;}

    void OpacityOption::forceDeactivate() {is_active = false;}

    bool OpacityOption::isActive() const { return is_active == true; }

    bool OpacityOption::isWindowContainer() const {
        return false;
    }

    void OpacityOption::setSize(const vec2u& size) {
        this->size = size;
    }
    void OpacityOption::setPos(const vec2i& pos) {
        this->pos = pos;
    }

    void OpacityOption::setState(ABarButton::State state) {
        this->state = state;
    }

    IBarButton::State OpacityOption::getState() const {
        return state;
    }

    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {
            auto optionsbar = static_cast<IOptionsBar*>(getRootWindow()->getWindowById(kOptionsBarWindowId));
            assert(optionsbar);
            vec2i pos = {optionsbar->getPos().x, optionsbar->getPos().y + 160};
            vec2u size = {100, 60};
            auto opacity = std::make_unique<OpacityOption>(pos, size);
            assert(opacity);
            optionsbar->addWindow(std::move(opacity));
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
