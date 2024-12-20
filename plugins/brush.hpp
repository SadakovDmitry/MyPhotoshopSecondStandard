#ifndef PLUGIN_BRUSH
#define PLUGIN_BRUSH


#include "headers/api_impl/bar/button.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_canvas.hpp"
#include <deque>


using namespace psapi;


extern "C"
{
    bool onLoadPlugin();
    void onUnloadPlugin();
}


class Brush : public ABarButton
{
    std::deque<sfm::vec2i> points_;
    sfm::vec2i last_pos_ = sfm::vec2i( 0, 0);
    sfm::IntRect canvas_rect_ = {};

    IColorPalette *palette_ = nullptr;
    ICanvas *canvas_;
    IOptionsBar *options_bar_;
    sfm::Color color_ = sfm::Color( 255, 0, 0);

    bool options_added_ = false;
    std::vector<std::unique_ptr<IWindow>> options_;
    std::vector<wid_t> id_;

    friend class BrushAction;
public:
    Brush(wid_t init_id, std::unique_ptr<sfm::ITexture> &init_texture, std::unique_ptr<sfm::ISprite> &init_sprite);
    ~Brush();

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
    void draw(sfm::IRenderWindow *renderWindow) override;
    void setColor(const sfm::Color &new_color);
    const sfm::Color &getColor() const;

private:
    void drawInterpolatedPoints( ILayer *layer, sfm::vec2i new_point);
    void drawPoint( ILayer *layer, sfm::vec2i pos);
    sfm::vec2i interpolate( sfm::vec2i p0, sfm::vec2i p1, sfm::vec2i p2, sfm::vec2i p3, float t);
    void addOptions();
    void removeOptions();
    void createOptions();
    void updateState(const IRenderWindow *render_window, const Event &event);
};



class BrushAction : public AUndoableAction
{
    Brush *brush_;
    sfm::vec2i last_pos_ = sfm::vec2i( 0, 0);
public:
    BrushAction( Brush *init_brush, const IRenderWindow *render_window, const Event &event);

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;
};

#endif // PLUGIN_BRUSH
