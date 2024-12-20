#include <Standard/api_bar.hpp>
#include "../sfm.hpp"
#include "../actions.hpp"


using psapi::IColorPalette;
using psapi::IThicknessOption;
using psapi::IOpacityOption;

using psapi::sfm::Color;
using psapi::sfm::RectangleShape;
using psapi::IRenderWindow;
using psapi::sfm::Event;

using psapi::sfm::vec2i;
using psapi::sfm::vec2u;

using psapi::IAction;
using psapi::wid_t;


class ColorPalette : public IColorPalette
{
    Color color_ = Color(0, 0, 0);

    vec2i pos_;
    vec2u size_;

    std::unique_ptr<RectangleShape> indicator_ = nullptr;
    std::vector<std::unique_ptr<RectangleShape>> colors_;

    const psapi::IOptionsBar *parent_ = nullptr;

    friend class ColorPaletteAction;
public:
    ColorPalette(vec2i init_pos, vec2u init_size);

    Color getColor() const override;
    void setColor(const Color &color) override;

    void draw(IRenderWindow* renderWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    wid_t getId() const override;

    IWindow* getWindowById(wid_t id) override;
    const IWindow* getWindowById(wid_t id) const override;


    vec2i getPos() const override;
    vec2u getSize() const override;

    void setSize(const vec2u& size) override;
    void setPos(const vec2i& pos) override;
    void setParent(const IWindow* parent) override;

    void forceActivate() override;
    void forceDeactivate() override;

    bool isActive() const override;
    bool isWindowContainer() const override;
};


class ThicknessOption : public IThicknessOption
{
    float thickness_ = 0;
public:
    ThicknessOption(vec2i init_pos, vec2u init_size);

    float getThickness() const override;
    void setThickness(float thickness) override;
};


class OpacityOption : public IOpacityOption
{
    float opacity_ = 0;
public:
    OpacityOption(vec2i init_pos, vec2u init_size);

    float getOpacity() const override;
    void setOpacity(float opacity) override;
};


class ColorPaletteAction : public AAction
{
    ColorPalette *palette_;
public:
    ColorPaletteAction(ColorPalette *palette, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool isOnShape(const vec2i &mouse_pos, const std::unique_ptr<RectangleShape> &shape);
};
