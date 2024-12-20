#ifndef PHOTOSHOP_CANVAS_IMPLEMENTATION
#define PHOTOSHOP_CANVAS_IMPLEMENTATION


#include <api/api_canvas.hpp>
#include <api_impl/windows.hpp>
#include <api_impl/scroll.hpp>


using namespace psapi;

using sfm::Color;

const sfm::vec2u CANVAS_SECTOR_SIZE = psapi::getCanvasIntRect().size;
const sfm::vec2i CANVAS_SECTOR_POS = psapi::getCanvasIntRect().pos;

class Canvas;


class Layer : public ILayer
{
    Canvas *canvas_;
    sfm::IntRect rect_;

    std::vector<Color> pixels_;
    std::list<std::unique_ptr<sfm::Drawable>> drawables_;

    friend class Canvas;
public:
    Layer(Canvas *canvas);
    ~Layer() = default;

    Color getPixel(sfm::vec2i pos) const override;
    void setPixel(sfm::vec2i pos, sfm::Color pixel) override;
    Color getPixelGlobal(sfm::vec2i pos) const;
    void setPixelGlobal(vec2i pos, Color pixel);

    drawable_id_t addDrawable(std::unique_ptr<sfm::Drawable> drawable) override;
    void removeDrawable(drawable_id_t id) override;
    void removeAllDrawables() override;

    vec2u getSize() const override;

    std::unique_ptr<ILayerSnapshot> save() override;
    void restore(ILayerSnapshot *snapshot) override;
};


class Canvas : public ICanvas
{
    std::list<std::unique_ptr<ILayer>> layers_;
    std::unique_ptr<ILayer> temp_layer_;

    bool is_active_;
    const IWindow *parent_;
    size_t active_layer_index_;

    vec2u size_;
    vec2i pos_;
    sfm::IntRect actual_rect_;
    vec2f zoom_ = vec2f( 1, 1);
    vec2i mouse_pos_ = vec2i( 0, 0);

    std::unique_ptr<sfm::ITexture> texture_;
    std::unique_ptr<sfm::ISprite> sprite_;

    HorizontalScroll h_scroll_;
    VerticalScroll v_scroll_;

    friend class Layer;
    friend class CanvasAction;
public:
    Canvas( vec2i init_pos, vec2u init_size);
    ~Canvas() = default;

    ILayer*       getLayer(size_t index)       override;
    const ILayer* getLayer(size_t index) const override;

    ILayer*       getTempLayer()       override;
    const ILayer* getTempLayer() const override;

    void cleanTempLayer() override;

    size_t getNumLayers() const override;

    size_t getActiveLayerIndex()       const override;
    void   setActiveLayerIndex(size_t index) override;

    bool insertLayer     (size_t index, std::unique_ptr<ILayer> layer) override;
    bool removeLayer     (size_t index) override;
    bool insertEmptyLayer(size_t index) override;

    sfm::vec2i getMousePosition() const override;

    virtual bool isPressedRightMouseButton() const override;
    virtual bool isPressedLeftMouseButton()  const override;
    virtual bool isPressedScrollButton()     const override;

    wid_t getId() const override;
    vec2i getPos() const override;
    vec2u getSize() const override;
    sfm::IntRect getActualRect() const;
    Color getCanvasBaseColor() const override;

    void setParent(const IWindow* parent) override;
    void setZoom(sfm::vec2f zoom) override;
    void setSize(const vec2u &size) override;
    void setPos(const vec2i &pos) override;

    bool isWindowContainer() const override;
    bool isActive() const override;

    void forceActivate() override;
    void forceDeactivate() override;

    IWindow *getWindowById( wid_t id) override;
    const IWindow *getWindowById( wid_t id) const override;

    void draw(IRenderWindow *removeWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    std::unique_ptr<ICanvasSnapshot> save() override;
    void restore(ICanvasSnapshot *snapshot) override;
};


class CanvasAction : public AAction
{
    Canvas *canvas_;
public:
    CanvasAction(Canvas *canvas, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


#endif // PHOTOSHOP_CANVAS_IMPLEMENTATION
