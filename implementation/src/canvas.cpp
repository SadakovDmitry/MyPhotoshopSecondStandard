#include "../headers/canvas.hpp"
#include <assert.h>

namespace psapi {

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         Layer
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    Layer::Layer(vec2u size_) : size(size_) {
        pix_arr = std::vector<sfm::Color>(size_.x * size_.y, sfm::Color(255, 255, 255, 255));
    }
    Layer::Layer(vec2i pos_, vec2u size_) : size(size_), pos(pos_){
        pix_arr = std::vector<sfm::Color>(size_.x * size_.y, sfm::Color(255, 255, 255, 255));
    }
    Layer::Layer() = default;

    sfm::Color Layer::getPixel(sfm::vec2i pix_pos) const {
        if (pix_pos.x < size.x && pix_pos.y < size.y &&
            pix_pos.x > 0      && pix_pos.y > 0        ) {
            return pix_arr[pix_pos.x + pix_pos.y * size.x];
        }
        return sfm::Color(0, 0, 0, 0);
    }
    void       Layer::setPixel(sfm::vec2i pos, sfm::Color pixel) {
        if (pos.x < size.x && pos.y < size.y &&
            pos.x > 0      && pos.y > 0        ) {
            pix_arr[pos.x + pos.y * size.x] = pixel;
        }
    }

    drawable_id_t Layer::addDrawable(std::unique_ptr<sfm::Drawable> object) {
        objects.push_back(std::move(object));
        return objects.size() - 1;
    }

    void Layer::removeDrawable(drawable_id_t id) {
        if (id < objects.size()) {
            objects.erase(objects.begin() + id);
        }
    }

    void Layer::removeAllDrawables() {
        objects.clear();
    }

    sfm::vec2u Layer::getSize() const {
        return size;
    }

    std::unique_ptr<ILayerSnapshot> Layer::save() {
        // auto snapshot = std::make_unique<SnapshotType>();
        // snapshot->pixels = pix_arr;
        // snapshot->drawables = objects;
        // snapshot->size = size;
        // return snapshot;
        return std::make_unique<LayerSnapshot>(pix_arr, std::move(objects), size);
    }

    void Layer::restore(ILayerSnapshot* snapshot) {
        LayerSnapshot* casted_snapshot = dynamic_cast<LayerSnapshot*>(snapshot);
        if (casted_snapshot) {
            pix_arr = casted_snapshot->pixels;
            objects = std::move(casted_snapshot->drawables);
            size = casted_snapshot->size;
        }
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         Canvas
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    Canvas::Canvas(vec2i pos_, vec2u size_, vec2u layer_size_)
            : temp_layer(std::make_unique<Layer>(layer_size_)), layers(), pos(pos_), layer_size(layer_size_), layer_pos(pos_), size(size_), texture(), sprite(), mouse_pos(vec2i(0, 0)), is_pressed(false), id(kCanvasWindowId), activeTool(nullptr) {
        texture.create(static_cast<unsigned int>(layer_size.x), static_cast<unsigned int>(layer_size.y));
        std::vector<sfm::Color> pix_arr(layer_size.x * layer_size.y, sfm::Color(255, 255, 255, 255));
        texture.loadFromMemory(pix_arr.data(), layer_size.x * layer_size.y, sfm::IntRect(sfm::vec2i(0, 0), sfm::vec2u(size.x, size.y)));
        // if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/Eraser.png")) {
        //     std::cerr << "Error opening file\n";
        // }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect(sfm::vec2i(0, 0), sfm::vec2u(size.x, size.y)));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
        layers.push_back(std::make_unique<Layer>(layer_size));
    }

    /**
     * @brief Renders the window.
     * @param renderWindow The render target.
     */
    void Canvas::draw(IRenderWindow* renderWindow) {
        // if(activeTool) {
        //     activeTool->action();
        // }
        // std::cout << "\033[34m              draw canvas\033[0m\n";

        for (auto& layer : layers) {
            assert(layer);
            Layer *now_layer = static_cast<Layer*>(layer.get());
            texture.update(now_layer->pix_arr.data());
            sprite.setTexture(&texture);
            //renderWindow->draw(&sprite);
            sprite.draw(renderWindow);
        }

        Layer *now_layer = static_cast<Layer*>(temp_layer.get());
        texture.update(now_layer->pix_arr.data());
        // texture.update(temp_layer->pix_arr.data());
        sprite.setTexture(&texture);
        // renderWindow->draw(&sprite);
        sprite.draw(renderWindow);
    }

    vec2i Canvas::CalcMouseShift() {
        int slider_pos_y = getRootWindow()->getWindowById(kCanvasWindowId - 1)->getWindowById(1)->getPos().y;
        int slider_pos_x = getRootWindow()->getWindowById(kToolBarWindowId + 11)->getWindowById(1)->getPos().x;
        float shift_coef_y  = static_cast<float>(slider_pos_y - pos.y) / size.y;
        float shift_coef_x  = static_cast<float>(slider_pos_x - pos.x) / size.x;
        int shift_y = shift_coef_y * (layer_size.y - size.y);
        int shift_x = (shift_coef_x * (layer_size.x - size.x)) * (layer_size.x / size.x);
        sprite.setTextureRect(sfm::IntRect({shift_x, shift_y}, {size.x, size.y}));
        return vec2i(shift_x, shift_y);
    }

//     bool Canvas::update(const IRenderWindow* renderWindow, const Event& event) {
//
//         if (sfm::Keyboard::isKeyPressed(sfm::Keyboard::Key::U)) {
//             for (int i = 0; i < layer_size.x; i++) {
//                 for (int j = 0; j < layer_size.y; j++) {
//                     ILayer* temp_layer = this->getTempLayer();
//                     sfm::Color col = temp_layer->getPixel({i, j});
//                     int k = -1;
//                     temp_layer->setPixel({i, j}, {static_cast<uint8_t>((col.r - 255) * k),
//                                                   static_cast<uint8_t>((col.g - 255) * k),
//                                                   static_cast<uint8_t>((col.b - 255) * k),
//                                                   255});
//                 }
//             }
//         }
//         if (event.type == Event::MouseMoved) {
//             mouse_pos = sfm::vec2i(event.mouseMove.x - layer_pos.x, event.mouseMove.y - layer_pos.y);
//
//             if (mouse_pos.x + layer_pos.x >= pos.x && mouse_pos.x + layer_pos.x <= pos.x + size.x &&
//                 mouse_pos.y + layer_pos.y >= pos.y && mouse_pos.y + layer_pos.y <= pos.y + size.y) {
//                 return true;
//             }
//             return true;
//         }
//         if (event.type == Event::MouseButtonPressed) {
//             mouse_pos = sfm::vec2i(event.mouseButton.x - layer_pos.x, event.mouseButton.y - layer_pos.y);
//
//             if (mouse_pos.x + layer_pos.x >= pos.x && mouse_pos.x + layer_pos.x <= pos.x + size.x &&
//                 mouse_pos.y + layer_pos.y >= pos.y && mouse_pos.y + layer_pos.y <= pos.y + size.y) {
//                 is_pressed = true;
//                 return true;
//             }
//             is_pressed = false;
//             return true;
//         }
//         if (event.type == Event::MouseButtonReleased) {
//             mouse_pos = sfm::vec2i(event.mouseButton.x - layer_pos.x, event.mouseButton.y - layer_pos.y);
//
//             if (mouse_pos.x + layer_pos.x >= pos.x && mouse_pos.x + layer_pos.x <= pos.x + size.x &&
//                 mouse_pos.y + layer_pos.y >= pos.y && mouse_pos.y + layer_pos.y <= pos.y + size.y) {
//                 is_pressed = false;
//                 return true;
//             }
//             is_pressed = false;
//             return true;
//         }
//
//         return false;
//     }

    /**
     * @brief Gets the unique ID of the window.
     * @return The window ID, or kInvalidWindowId if not set.
     */
    wid_t Canvas::getId() const {
        return id;
    }

    /**
     * @brief Retrieves a window by its ID from this window or its children.
     * @param id The ID of the window to search for.
     * @return A pointer to the window if found, or nullptr if not found.
     */
    IWindow* Canvas::getWindowById(wid_t searchId) {
        if (id == searchId) {
            return this;
        }
        return nullptr;
    }

    /**
     * @brief Retrieves a window by its ID from this window or its children (const version).
     * @param id The ID of the window to search for.
     * @return A const pointer to the window if found, or nullptr if not found.
     */
    const IWindow* Canvas::getWindowById(wid_t searchId) const {
        if (id == searchId) {
            return this;
        }
        return nullptr;
    }

    /**
     * @brief Gets the position of the window.
     * @return The absolute window position as a vec2i.
     */
    vec2i Canvas::getPos() const {
        return vec2i(pos.x, pos.y);
    }

    /**
     * @brief Gets the size of the window.
     * @return The window size as a vec2i.
     */
    vec2u Canvas::getSize() const {
        return vec2u(size.x, size.y);
    }

    /**
     * @brief Sets the parent of this window.
     * @param parent A pointer to the parent window.
     */
    void Canvas::setParent(const IWindow* parent) {
        this -> parent = parent;
    }

    /**
     * @brief Forces the window to activate.
     */
    void Canvas::forceActivate() {
        is_active = true;
    }

    /**
     * @brief Forces the window to deactivate.
     */
    void Canvas::forceDeactivate() {
        is_active = false;
    }

    bool Canvas::isActive() const { return is_active == true; }

    /**
     * @brief Checks if the window is a window container.
     * @return True if the window is a window container, false otherwise.
     */
    bool Canvas::isWindowContainer() const {
        return false;
    }

          ILayer* Canvas::getLayer(size_t index) {
            return layers.at(index).get();
    }
    const ILayer* Canvas::getLayer(size_t index) const {
            return layers.at(index).get();
    }

          ILayer* Canvas::getTempLayer() {
            return temp_layer.get();
    }
    const ILayer* Canvas::getTempLayer() const {
        return temp_layer.get();
    }

    void Canvas::cleanTempLayer() {
        if (layers.size()) {
            vec2u size = temp_layer.get()->size;
            temp_layer = std::make_unique<Layer>(size);
            std::cout << "bed realised clean\n";
        }
    }

    size_t Canvas::getNumLayers() const {
        return layers.size();
    }

    size_t Canvas::getActiveLayerIndex() const {
        for (int i = 0; i < layers.size(); i++) {
            if (layers.at(i).get()->is_active) {
                return i;
            }
        }
        return 0;
    }
    void   Canvas::setActiveLayerIndex(size_t index) {
        if(index < layers.size()) {
            for (int i = 0; i < layers.size(); i++) {
                if (layers.at(i).get()->is_active) {
                    layers.at(i).get()->is_active = false;
                }
            }
            layers.at(index).get()->is_active = true;
        }
    }

    bool Canvas::insertLayer     (size_t index, std::unique_ptr<ILayer> layer) {
        if (index <= layers.size()) {
            std::unique_ptr<Layer> new_layer = std::make_unique<Layer>(size);
            for (int x = 0; x < size.x; x++) {
                for (int y = 0; y < size.y; y++) {
                    new_layer->setPixel(vec2i(x, y), layer->getPixel(vec2i(x, y)));
                }
            }
            layers.insert(layers.begin() + index, std::move(new_layer));
            return true;
        }
        return false;
    }
    bool Canvas::removeLayer     (size_t index) {
        if (index < layers.size()) {
            layers.erase(layers.begin() + index);
            return true;
        }
        return false;
    }
    bool Canvas::insertEmptyLayer(size_t index) {
        if (index <= layers.size()) {
            std::unique_ptr<Layer> new_layer = std::make_unique<Layer>(size);
            for (int x = 0; x < size.x; x++) {
                for (int y = 0; y < size.y; y++) {
                    new_layer->setPixel(vec2i(x, y), sfm::Color(0, 0, 0, 0));
                }
            }
            layers.insert(layers.begin() + index, std::move(new_layer));
            return true;
        }
        return false;
    }

    void Canvas::setPos  (sfm::vec2i pos) {
        this->pos = pos;
    }
    void Canvas::setSize (sfm::vec2u size) {
        this->size = size;
    }
    void Canvas::setScale(sfm::vec2f scale) {
        this->scale = scale;
    }

    sfm::vec2i Canvas::getMousePosition() const {
        return mouse_pos;
    }
    bool Canvas::isPressed() const {
        return is_pressed;
    }

    bool Canvas::scroll(const vec2i& vec) {
        //std::cout << "sprite_pos " << layer_pos.x << ", " << layer_pos.y << "\n";

        layer_pos.x = static_cast<int>(layer_pos.x - vec.x);
        layer_pos.y = static_cast<int>(layer_pos.y - vec.y);

        scroll_shift.x += vec.x;
        scroll_shift.y += vec.y;
        sprite.setTextureRect(sfm::IntRect({scroll_shift.x, scroll_shift.y}, {size.x, size.y}));

        return true;
    }

    void Canvas::setShift(vec2i shift_) {
        scroll_shift.x = shift_.x;
        scroll_shift.y = shift_.y;
    }

    std::unique_ptr<IAction> Canvas::createAction(const IRenderWindow* renderWindow_, const Event& event_) {
        return std::make_unique<CanvasAction>(this, renderWindow_, event_);
    }

    void Canvas::setSize(const vec2u& size_) {
        size = {size_.x, size_.y};
    }
    void Canvas::setPos(const vec2i& pos_) {
            pos = {pos_.x, pos_.y};
    }

    void Canvas::setZoom(sfm::vec2f zoom) {
        scale = zoom;
    }

    std::unique_ptr<ICanvasSnapshot> Canvas::save() {
        return std::make_unique<CanvasSnapshot>(std::move(objects), size);
    }

    void Canvas::restore(ICanvasSnapshot* snapshot) {
        LayerSnapshot* casted_snapshot = dynamic_cast<LayerSnapshot*>(snapshot);
        if (casted_snapshot) {
            objects = std::move(casted_snapshot->drawables);
            size = casted_snapshot->size;
        }
    }

    sfm::Color Canvas::getCanvasBaseColor() const {
        return base_color;
    }

    bool Canvas::isPressedLeftMouseButton() const {
        return left_button_pressed;
    }

    bool Canvas::isPressedRightMouseButton() const {
        return right_button_pressed;
    }

    bool Canvas::isPressedScrollButton() const {
        return scroll_button_pressed;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                     CanvasAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    CanvasAction::CanvasAction(Canvas* canvas_, const IRenderWindow *renderWindow_, const Event &event_)
                    : canvas(canvas_), AAction(renderWindow_, event_) {}

    bool CanvasAction::execute(const Key &key) {
        // std::cout << "\033[32mexecute CanvasAction\033[0m\n";
        if (event.type == Event::MouseMoved) {
            canvas->mouse_pos = sfm::vec2i(event.mouseMove.x - canvas->layer_pos.x, event.mouseMove.y - canvas->layer_pos.y);
            // std::cout << "\033[33mx: " << canvas->mouse_pos.x << ",y: " << canvas->mouse_pos.y << "\033[0m\n";
            return true;
        }
        if (event.type == Event::MouseButtonPressed) {
            canvas->mouse_pos = sfm::vec2i(event.mouseButton.x - canvas->layer_pos.x, event.mouseButton.y - canvas->layer_pos.y);
            // std::cout << "\033[34mx: " << canvas->mouse_pos.x << ",y: " << canvas->mouse_pos.y << "\033[0m\n";

            if (canvas->mouse_pos.x + canvas->layer_pos.x >= canvas->pos.x && canvas->mouse_pos.x + canvas->layer_pos.x <= canvas->pos.x + canvas->size.x &&
                canvas->mouse_pos.y + canvas->layer_pos.y >= canvas->pos.y && canvas->mouse_pos.y + canvas->layer_pos.y <= canvas->pos.y + canvas->size.y) {
                canvas->is_pressed = true;
                canvas->left_button_pressed = true;
                // std::cout << "\033[31mpressed\033[0m\n";
                return true;
            }
            canvas->is_pressed = false;
            return true;
        }
        if (event.type == Event::MouseButtonReleased) {
            canvas->mouse_pos = sfm::vec2i(event.mouseButton.x - canvas->layer_pos.x, event.mouseButton.y - canvas->layer_pos.y);
            // std::cout << "\033[35mx: " << canvas->mouse_pos.x << ",y: " << canvas->mouse_pos.y << "\033[0m\n";
            canvas->left_button_pressed = false;
            canvas->is_pressed = false;
            return true;
        }

        return false;
    }

    bool CanvasAction::isUndoable(const Key &key) {
        return false;
    }

}
