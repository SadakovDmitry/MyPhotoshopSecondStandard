
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "../headers/color_palette.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

#include <assert.h>

namespace psapi {

    ColorPalette::ColorPalette(vec2i pos_, vec2u size_, const std::string& file_palette, const std::string& file_cursor)
        : pos(pos_), size(size_), id(kColorPaletteId), selectedColor({0, 0, 0, 255}) {
        if (!paletteImage.loadFromFile(file_palette)) {
            std::cerr << "Error loading palette image\n";
        }
        if (!texture.loadFromFile(file_palette)) {
            std::cerr << "Error loading palette image\n";
        }
        //texture.loadFromImage(paletteImage);
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setPosition(pos.x, pos.y);

        if (!cursor_texture.loadFromFile(file_cursor)) {
            std::cerr << "Error loading file_cursor image\n";
        }

        cursor_sprite.setTexture(&cursor_texture);
        cursor_sprite.setTextureRect(sfm::IntRect({0, 0}, {30, 30}));
        cursor_sprite.setScale(0.5, 0.5);
    }

    sfm::Color ColorPalette::getColor() const {
        return selectedColor;
    }

    void ColorPalette::setColor(const sfm::Color &col) {
        selectedColor = col;
    }

    void ColorPalette::draw(IRenderWindow* renderWindow) {
        renderWindow->draw(&sprite);
        renderWindow->draw(&cursor_sprite);
    }

    std::unique_ptr<IAction> ColorPalette::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<ColorPaletteAction>(this, renderWindow_, event_);
    }

    void ColorPalette::updateState(const IRenderWindow *render_window_, const Event &event_) {
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

    ColorPaletteAction::ColorPaletteAction(ColorPalette *color_palette_, const IRenderWindow *render_window_, const Event &event_)
    :   AAction(render_window_, event_), color_palette(color_palette_) {}

    bool ColorPaletteAction::isUndoable(const Key &key) {
        return true;
    }

    bool ColorPaletteAction::execute(const Key &key) {
        color_palette->updateState(render_window, event);
        if (color_palette->cursor_is_dragging) {
            Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
            vec2i mouse_pos = canvas->getMousePosition();
            vec2u rel_pos = {static_cast<unsigned int>(mouse_pos.x + canvas->layer_pos.x - color_palette->pos.x),
                             static_cast<unsigned int>(mouse_pos.y + canvas->layer_pos.y - color_palette->pos.y)};
            color_palette->selectedColor = color_palette->paletteImage.getPixel(rel_pos);
            color_palette->cursor_sprite.setPosition(color_palette->last_mouse_pos.x - 8,
                                                     color_palette->last_mouse_pos.y - 8);
            color_palette->cursor_sprite.setColor(color_palette->selectedColor);
        }
        return true;
    }

    wid_t ColorPalette::getId() const {
        return id;
    }

    IWindow* ColorPalette::getWindowById(wid_t id) {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    const IWindow* ColorPalette::getWindowById(wid_t id) const {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    vec2i ColorPalette::getPos() const {
        return pos;
    }

    vec2u ColorPalette::getSize() const {
        return size;
    }

    void ColorPalette::setParent(const IWindow* parent) {
       this->parent = parent;
    }

    void ColorPalette::forceActivate() {is_active = true;}

    void ColorPalette::forceDeactivate() {is_active = false;}

    bool ColorPalette::isActive() const { return is_active == true; }

    bool ColorPalette::isWindowContainer() const {
        return false;
    }

    void ColorPalette::setSize(const vec2u& size) {
        this->size = size;
    }
    void ColorPalette::setPos(const vec2i& pos) {
        this->pos = pos;
    }

    void ColorPalette::setState(ABarButton::State state) {
        this->state = state;
    }

    IBarButton::State ColorPalette::getState() const {
        return state;
    }

    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {
//             ChildInfo info_palette;
//             info_palette.pos = {105, 5};
//             info_palette.size = {150, 150};
            auto optionsbar = static_cast<IOptionsBar*>(getRootWindow()->getWindowById(kOptionsBarWindowId));
            assert(optionsbar);
            vec2i pos = {optionsbar->getPos().x, optionsbar->getPos().y};
            vec2u size = {100, 100};
            auto palette = std::make_unique<ColorPalette>(pos, size,
                        "/Users/dima/MIPT/SecondSem/MyPaint2.0/images/palette.jpg",
                        "/Users/dima/MIPT/SecondSem/MyPaint2.0/images/color_cursor.png");
            assert(palette);
            optionsbar->addWindow(std::move(palette));
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
            IBar* toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            if (toolbar) {
                toolbar->removeWindow(1);
            }
        }
    }
}
