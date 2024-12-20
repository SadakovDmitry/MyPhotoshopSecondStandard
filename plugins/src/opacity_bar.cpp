
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "../headers/opacity_bar.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

#include <assert.h>

namespace psapi {

    OpacityOption::OpacityOption(vec2i pos_, vec2u size_, const std::string& bar_file, const std::string& cursor_file)
        : pos(pos_), size(size_), id(kOpacityBarId), opacity(255) {
        if (!texture.loadFromFile(bar_file)) {
            std::cerr << "Error loading opacity image\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setPosition(pos.x, pos.y);

        if (!cursor_texture.loadFromFile(cursor_file)) {
            std::cerr << "Error loading cursor image\n";
        }

        cursor_sprite.setTexture(&cursor_texture);
        cursor_sprite.setTextureRect(sfm::IntRect({0, 0}, {10, 14}));
        cursor_sprite.setPosition({static_cast<float>(pos.x + size.x - 10), pos.y + static_cast<float>(size.y / 2 - 8)});
        cursor_sprite.setScale(1, 1);
    }

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
            uint32_t max_opacity = 100;
            opacity_bar->opacity = std::min(max_opacity, rel_pos.x);

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
            auto opacity = std::make_unique<OpacityOption>(pos, size,
                                            "/Users/dima/MIPT/SecondSem/MyPaint2.0/images/opacity_bar.png",
                                            "/Users/dima/MIPT/SecondSem/MyPaint2.0/images/thickness_cursor.png");
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
