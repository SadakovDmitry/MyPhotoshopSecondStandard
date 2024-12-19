#ifndef COLOR_PALETTE_HPP
#define COLOR_PALETTE_HPP

//#include "api_photoshop.hpp"
// #include "/Users/dima/MIPT/SecondSem/MyPaint/implementation/toolbar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_bar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/my_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_actions.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/implementation/canvas.hpp"

#include <cstdint>
#include <memory>
#include <iostream>
#include <assert.h>

namespace psapi {
class ColorPalette;

// class ColorPalette : public IColorPalette {
// private:
//     friend class ColorPaletteAction;
//     friend class ColorButton;
//     //friend class Cursor;
//     sfm::Color selectedColor;
//     sfm::Image image;
//     ABarButton* col_button;
//     ABarButton* cursor;
//
// public:
//
//     ColorPalette(vec2i pos_, vec2u size_)
//         : selectedColor({0, 0, 0, 255}) {
//         if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/palette.jpg")) {
//              std::cerr << "Error opening file\n";
//         }
//         if (!image.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/palette.jpg")) {
//              std::cerr << "Error opening file\n";
//         }
//         sprite.setTexture(&texture);
//         sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
//         sprite.setScale(1, 1);
//         sprite.setColor(sfm::Color(255, 255, 255, 255));
//         sprite.setPosition(pos.x, pos.y);
//         col_button->setPos(pos_);
//         col_button->setSize(size_);
//     }
//
//     std::unique_ptr<IAction> ColorPalette::createAction(const IRenderWindow *renderWindow_, const Event &event_) override;
//
//
//     sfm::Color getColor();
//     void setColor(sfm::Color);
//     static std::unique_ptr<IColorPalette> create();
//
//     virtual ~ColorPalette() = default;
// };
//
// class ColorPaletteAction : public AAction {
//     ColorPalette *color_palette;
// public:
//     ColorPaletteAction(ColorPalette *color_palette_, const IRenderWindow *render_window_, const Event &event_);
//
//     bool isUndoable(const Key &key) override;
//     bool execute(const Key &key) override;
// };
//
// class ColorButton : public ABarButton {
// public:
//     ColorButton(vec2i pos_, vec2u size_)
//         : ABurButton(pos_, size_, id_), selectedColor({0, 0, 0, 255}) {
//         if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/palette.jpg")) {
//              std::cerr << "Error opening file\n";
//         }
//         if (!image.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/palette.jpg")) {
//              std::cerr << "Error opening file\n";
//         }
//         sprite.setTexture(&texture);
//         sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
//         sprite.setScale(1, 1);
//         sprite.setColor(sfm::Color(255, 255, 255, 255));
//         sprite.setPosition(pos.x, pos.y);
//     }
//
//     std::unique_ptr<IAction> ColorPalette::createAction(const IRenderWindow *renderWindow_, const Event &event_) override;
//
//
//     sfm::Color getColor();
//     void setColor(sfm::Color);
//
//     virtual ~ColorButton() = default;
// };
//
// class ColorButtonAction : public AAction {
//     ColorButton *color_button;
// public:
//     ColorButtonAction(ColorButton *color_button_, const IRenderWindow *render_window_, const Event &event_);
//
//     bool isUndoable(const Key &key) override;
//     bool execute(const Key &key) override;
// };
//
// class Cursor : public ABarButton {
// public:
//     Cursor(vec2i pos_, vec2u size_)
//         : ABurButton(pos_, size_, id_), selectedColor({0, 0, 0, 255}) {
//         if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/palette.jpg")) {
//              std::cerr << "Error opening file\n";
//         }
//         if (!image.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/palette.jpg")) {
//              std::cerr << "Error opening file\n";
//         }
//         sprite.setTexture(&texture);
//         sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
//         sprite.setScale(1, 1);
//         sprite.setColor(sfm::Color(255, 255, 255, 255));
//         sprite.setPosition(pos.x, pos.y);
//     }
//
//     std::unique_ptr<IAction> ColorPalette::createAction(const IRenderWindow *renderWindow_, const Event &event_) override;
//
//
//     sfm::Color getColor();
//     void setColor(sfm::Color);
//
//     virtual ~Cursor() = default;
// };
//
// class CursorAction : public AAction {
//     Cursor *cursor;
// public:
//     CursorAction(Cursor *cursor_, const IRenderWindow *render_window_, const Event &event_);
//
//     bool isUndoable(const Key &key) override;
//     bool execute(const Key &key) override;
// };

class ColorPalette : public IColorPalette {
private:
    sfm::Color selectedColor;
    sfm::Image paletteImage;

    sfm::Texture texture;
    sfm::Sprite sprite;
    sfm::Texture cursor_texture;
    sfm::Sprite cursor_sprite;
    vec2i last_mouse_pos;
    friend class ColorPaletteAction;
    wid_t id;
    bool is_active;
    bool cursor_is_dragging = false;
    vec2i pos;
    vec2u size;
    vec2f scale;
    IBarButton::State state;
    const IWindow* parent;

    void updateState(const IRenderWindow *render_window_, const Event &event_);
public:
    ColorPalette(vec2i pos_, vec2u size_)
        : pos(pos_), size(size_), id(kColorPaletteId), selectedColor({0, 0, 0, 255}) {
        if (!paletteImage.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/palette.jpg")) {
            std::cerr << "Error loading palette image\n";
        }
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/palette.jpg")) {
            std::cerr << "Error loading palette image\n";
        }
        //texture.loadFromImage(paletteImage);
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setPosition(pos.x, pos.y);

        if (!cursor_texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint2.0/source/color_cursor.png")) {
            std::cerr << "Error loading palette image\n";
        }

        cursor_sprite.setTexture(&cursor_texture);
        cursor_sprite.setTextureRect(sfm::IntRect({0, 0}, {30, 30}));
        cursor_sprite.setScale(0.5, 0.5);
    }

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow_, const Event &event_) override;

    virtual sfm::Color getColor() const override;
    virtual void setColor(const sfm::Color& color) override;

    void draw(IRenderWindow* renderWindow) override;

    virtual wid_t getId() const override;
    virtual IWindow* getWindowById(wid_t id) override;
    virtual const IWindow* getWindowById(wid_t id) const override;
    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;

    virtual void setParent(const IWindow* parent) override;
    virtual void setSize(const vec2u &size) override;
    virtual void setPos(const vec2i &pos) override;

    virtual void forceActivate() override;
    virtual void forceDeactivate() override;

    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

    void setState(ABarButton::State state);
    IBarButton::State getState() const;

    virtual ~ColorPalette() = default;
};

class ColorPaletteAction : public AAction {
    ColorPalette *color_palette;
public:
    ColorPaletteAction(ColorPalette *color_palette_, const IRenderWindow *render_window_, const Event &event_);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;
};

extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

}

#endif
