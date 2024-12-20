#ifndef CANVAS_HPP
#define CANVAS_HPP

// #include "api_photoshop.hpp"
// #include "api_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_canvas.hpp"
// #include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_memento.hpp"

#include "toolbar.hpp"
#include "memento.hpp"
#include "my_sfm.hpp"
#include "buttons.hpp"

#include <cstdint>
#include <memory>
#include <iostream>

namespace psapi {

class Scrollable {
protected:
    bool is_dragging;
    vec2i scroll_shift;
public:
    Scrollable() = default;
    Scrollable(vec2i scroll_shift_) : scroll_shift(scroll_shift_) {}

    virtual bool scroll(const vec2i& vec) = 0;
    virtual void setShift(vec2i shift_) = 0;
};

class Layer : public ILayer {
private:
    friend class Canvas;
    std::vector<sfm::Color> pix_arr;
    std::vector<std::unique_ptr<sfm::Drawable>> objects;
    vec2i pos;
    vec2u size;
    bool is_active;

public:
    Layer(vec2u size_);
    Layer(vec2i pos_, vec2u size_);
    Layer();
    sfm::Color getPixel(sfm::vec2i pos) const override;
    void       setPixel(sfm::vec2i pos, sfm::Color pixel) override;
    drawable_id_t addDrawable(std::unique_ptr<sfm::Drawable> object) override;
    void removeDrawable(drawable_id_t id) override;
    void removeAllDrawables() override;

    sfm::vec2u getSize() const override;
    std::unique_ptr<ILayerSnapshot> save() override;
    void restore(ILayerSnapshot* snapshot) override;
};

class Canvas : public ICanvas, public Scrollable{
private:
    friend class CanvasAction;
    std::vector<std::unique_ptr<Layer>> layers;
    std::unique_ptr<Layer> temp_layer;
    int id;
    vec2i pos;
    vec2u size;
    vec2f scale;
    sfm::Color base_color;
    psapi::sfm::Texture texture;
    psapi::sfm::Sprite sprite;
    vec2i mouse_pos;
    bool is_pressed;
    bool left_button_pressed = false;
    bool right_button_pressed = false;
    bool scroll_button_pressed = false;
    bool is_active  = true;
    const IWindow* parent;
    ABarButton* activeTool;
    std::vector<std::unique_ptr<sfm::Drawable>> objects;

    vec2i CalcMouseShift();
public:
    vec2u layer_size;
    vec2i layer_pos;

    Canvas(vec2i pos_, vec2u size_, vec2u layer_size_);
    ~Canvas() = default;
    /**
     * @brief Renders the window.
     * @param renderWindow The render target.
     */
    void draw(IRenderWindow* renderWindow) override;

    /**
     * @brief Gets the unique ID of the window.
     * @return The window ID, or kInvalidWindowId if not set.
     */
    wid_t getId() const override;

    /**
     * @brief Retrieves a window by its ID from this window or its children.
     * @param id The ID of the window to search for.
     * @return A pointer to the window if found, or nullptr if not found.
     */
    IWindow* getWindowById(wid_t id) override;

    /**
     * @brief Retrieves a window by its ID from this window or its children (const version).
     * @param id The ID of the window to search for.
     * @return A const pointer to the window if found, or nullptr if not found.
     */
    const IWindow* getWindowById(wid_t id) const override;

    /**
     * @brief Gets the position of the window.
     * @return The absolute window position as a vec2i.
     */
    vec2i getPos() const override;

    /**
     * @brief Gets the size of the window.
     * @return The window size as a vec2i.
     */
    vec2u getSize() const override;

    /**
     * @brief Sets the parent of this window.
     * @param parent A pointer to the parent window.
     */
    void setParent(const IWindow* parent) override;

    /**
     * @brief Forces the window to activate.
     */
    void forceActivate() override;

    /**
     * @brief Forces the window to deactivate.
     */
    void forceDeactivate() override;

    bool isActive() const override;

    /**
     * @brief Checks if the window is a window container.
     * @return True if the window is a window container, false otherwise.
     */
    bool isWindowContainer() const override;

    ILayer*       getLayer(size_t index) override;
    const ILayer* getLayer(size_t index) const override;

    ILayer*       getTempLayer() override;
    const ILayer* getTempLayer() const override;

    void cleanTempLayer() override;

    size_t getNumLayers() const override;

    size_t getActiveLayerIndex() const override;
    void   setActiveLayerIndex(size_t index) override;

    bool insertLayer     (size_t index, std::unique_ptr<ILayer> layer) override;
    bool removeLayer     (size_t index) override;
    bool insertEmptyLayer(size_t index) override;

    void setPos  (sfm::vec2i pos);
    void setSize (sfm::vec2u size);
    void setScale(sfm::vec2f scale);

    sfm::vec2i getMousePosition() const override;
    bool isPressed() const;

    void setActiveTool(ABarButton* tool) {
        activeTool = tool;
    }
    ABarButton* getActiveTool() {
        return activeTool;
    }

    bool scroll(const vec2i& vec) override;
    void setShift(vec2i shift_) override;

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    void setSize(const vec2u& size) override;
    void setPos(const vec2i& pos) override;

    void setZoom(sfm::vec2f zoom) override;

    std::unique_ptr<ICanvasSnapshot> save() override;
    void restore(ICanvasSnapshot* snapshot) override;

    sfm::Color getCanvasBaseColor() const override;

    bool isPressedLeftMouseButton() const override;
    bool isPressedRightMouseButton() const override;
    bool isPressedScrollButton() const override;
};

class CanvasAction : public AAction {
    Canvas* canvas;
public:
    CanvasAction(Canvas* canvas_, const IRenderWindow *renderWindow_, const Event &event_);
    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

} // namespace

#endif // API_CANVAS_HPP

