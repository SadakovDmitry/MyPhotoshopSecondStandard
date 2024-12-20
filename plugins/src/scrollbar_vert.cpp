// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "../headers/scrollbar_vert.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {
    const int kScrollBarVertWindowId  = kToolBarWindowId + 21;
    typedef IWindowContainer* (*RootWindow)();

    IWindowContainer* getRootWindowptr() {
        void* photoshopLib = dlopen("/Users/dima/MIPT/SecondSem/MyPaint/Standard/libphotoshop.dylib", RTLD_LAZY);
        if (!photoshopLib) {
            std::cerr << "Failed to load photoshop dylib in pencil: " << dlerror() << "\n";
        }
        RootWindow getRootWindowlib = reinterpret_cast<RootWindow>(dlsym(photoshopLib, "getRootWindow"));
        if (!getRootWindowlib) {
            std::cerr << "Failed to locate functions in photoshop dylib in pencil: " << dlerror() << "\n";
            dlclose(photoshopLib);
        }
        if (!getRootWindowlib()) {
            std::cerr << "Failed to initialize photoshop in pencil.\n";
            dlclose(photoshopLib);
        }

        return getRootWindowlib();
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          ScrollBar
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarVert::ScrollBarVert(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_) : id(id_), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr), scrollbar(), scroll_obj(scroll_obj_) {
        //texture.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y));
        std::vector<sfm::Color> pix_arr(size.x * size.y, sfm::Color(170, 170, 170, 255));
        texture.loadFromMemory(pix_arr.data(), size.x * size.y, sfm::IntRect(pos, size));
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(170, 170, 170, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    void ScrollBarVert::draw(IRenderWindow* renderWindow) {
        renderWindow->draw(&sprite);
        for (const auto& button : scrollbar) {
            button->draw(renderWindow);
        }
    }

    wid_t ScrollBarVert::getId() const { return id; }

    IWindow* ScrollBarVert::getWindowById(wid_t id) {
        if (this->id == id) return this;
        for (auto& button : scrollbar) {
            IWindow* found = button->getWindowById(id);
            if (found) return found;
        }
        return nullptr;
    }

    const IWindow* ScrollBarVert::getWindowById(wid_t id) const {
        if (this->id == id) return this;
        for (const auto& button : scrollbar) {
            const IWindow* found = button->getWindowById(id);
            if (found) return found;
        }
        return nullptr;
    }

    vec2i ScrollBarVert::getPos() const { return pos; }
    vec2u ScrollBarVert::getSize() const { return size; }
    void ScrollBarVert::setParent(const IWindow* parent) { this->parent = parent; }
    void ScrollBarVert::forceActivate() { is_active = true; }
    void ScrollBarVert::forceDeactivate() { is_active = false; }
    bool ScrollBarVert::isActive() const { return is_active == true; }
    bool ScrollBarVert::isWindowContainer() const { return true; }

    void ScrollBarVert::addWindow(std::unique_ptr<IWindow> window) {
        scrollbar.push_back(std::unique_ptr<ABarButton>(dynamic_cast<ABarButton*>(window.release())));

    }

    void ScrollBarVert::removeWindow(wid_t id) {
        scrollbar.erase(
            std::remove_if(
                scrollbar.begin(),
                scrollbar.end(),
                [id](const std::unique_ptr<ABarButton>& button) {
                    return button->getId() == id;
                }
            ),
            scrollbar.end()
        );
    }

    // ChildInfo ScrollBarVert::getNextChildInfo() const {
    //     ChildInfo info;
    //     int totalWidth = 0;
    //     for (const auto& button : scrollbar) {
    //         totalWidth += button->getSize().x;
    //     }
    //     info.pos = vec2i(pos.x + totalWidth % size.x, pos.y + totalWidth / size.x * 50);
    //     info.size = vec2u(50, 50);
    //     return info;
    // }

    void ScrollBarVert::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const {
        // renderWindow->draw(reinterpret_cast<const sfm::Drawable*>(&(static_cast<const BarButton*>(button)->sprite)));
    }

    bool ScrollBarVert::unPressAllButtons() {
        for (int i = 0; i < scrollbar.size(); i++) {
            scrollbar[i].get()->forceDeactivate();
        }
        return true;
    }

    std::unique_ptr<IAction> ScrollBarVert::createAction(const IRenderWindow* renderWindow_, const Event& event_) {
        return std::make_unique<ScrollBarVertAction>(this, renderWindow_, event_);
    }

    void ScrollBarVert::setSize(const vec2u& size_) {
        size = size_;
    }
    void ScrollBarVert::setPos(const vec2i& pos_) {
        pos = pos_;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                    ScrollBarAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarVertAction::ScrollBarVertAction(ScrollBarVert* scrollbar_, const IRenderWindow *renderWindow_, const Event &event_)
                : scrollbar(scrollbar_), AAction(renderWindow_, event_) {}

    bool ScrollBarVertAction::execute(const Key &key) {
        //std::cout << "\033[31msize: \033[0m" << scrollbar->scrollbar.size() << std::endl;
        for (auto& button : scrollbar->scrollbar) {
            getActionController()->execute(button->createAction(render_window, event));
        }
        return true;
    }

    bool ScrollBarVertAction::isUndoable(const Key &key) {
        return false;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      ScrollBarTools
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarSlider::ScrollBarSlider(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_)
        : ABarButton(pos_, size_, id_), is_dragging(false), scroll_obj(scroll_obj_) {
        //texture.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y));
        std::vector<sfm::Color> pix_arr(size.x * size.y, sfm::Color(100, 100, 100, 255));
        texture.loadFromMemory(pix_arr.data(), size.x * size.y, sfm::IntRect(pos, size));
        hovered_sprite.setTexture(&texture);
        hovered_sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        hovered_sprite.setScale(1, 1);
        hovered_sprite.setColor(sfm::Color(100, 100, 100, 255));
        hovered_sprite.setPosition(pos.x, pos.y);

        pressed_sprite.setTexture(&texture);
        pressed_sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        pressed_sprite.setScale(1, 1);
        pressed_sprite.setColor(sfm::Color(70, 70, 70, 255));
        pressed_sprite.setPosition(pos.x, pos.y);

        released_sprite.setTexture(&texture);
        released_sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        released_sprite.setScale(1, 1);
        released_sprite.setColor(sfm::Color(100, 100, 100, 255));
        released_sprite.setPosition(pos.x, pos.y);

        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(100, 100, 100, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    std::unique_ptr<IAction> ScrollBarSlider::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<ScrollBarSliderAction>(this, renderWindow_, event_);
    }

    void ScrollBarSlider::updateState(const IRenderWindow *render_window_, const Event &event_) {
        // getActionController()->execute(ABarButton::createAction(render_window_, event_));
        // std::cout << "\033[32mexecute ABarButtonAction\033[0m\n";
        vec2i mouse_pos = sfm::Mouse::getPosition(render_window_);
        vec2i pos = this->getPos();
        vec2u size = this->getSize();

        if (pos.x <= mouse_pos.x && mouse_pos.x <= pos.x + size.x &&
            pos.y <= mouse_pos.y && mouse_pos.y <= pos.y + size.y) {
            if (event_.type == Event::MouseButtonPressed) {
                this->state = IBarButton::State::Press;
            } else if (event_.type == Event::MouseButtonReleased) {
                this->state = IBarButton::State::Released;
            } else {
                this->state = IBarButton::State::Hover;
            }
        } else if (event_.type == Event::MouseButtonReleased) {
            this->state = IBarButton::State::Normal;
            this->state = IBarButton::State::Released;
        } else {
            this->state = IBarButton::State::Normal;
        }
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarArrUp::ScrollBarArrUp(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_, const std::string& file)
        : ABarButton(pos_, size_, id_), scroll_obj(scroll_obj_) {
        if (!texture.loadFromFile(("/Users/dima/MIPT/SecondSem/MyPaint2.0/images/" + file).c_str())) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(100, 100, 100, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    std::unique_ptr<IAction> ScrollBarArrUp::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<ScrollBarArrUpAction>(this, renderWindow_, event_);
    }

    void ScrollBarArrUp::updateState(const IRenderWindow *render_window_, const Event &event_) {
        getActionController()->execute(ABarButton::createAction(render_window_, event_));
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarArrDown::ScrollBarArrDown(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_, const std::string& file)
        : ABarButton(pos_, size_, id_), scroll_obj(scroll_obj_) {
        if (!texture.loadFromFile(("/Users/dima/MIPT/SecondSem/MyPaint2.0/images/" + file).c_str())) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(100, 100, 100, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    std::unique_ptr<IAction> ScrollBarArrDown::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<ScrollBarArrDownAction>(this, renderWindow_, event_);
    }

    void ScrollBarArrDown::updateState(const IRenderWindow *render_window_, const Event &event_) {
        getActionController()->execute(ABarButton::createAction(render_window_, event_));
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                 ScrollBarToolsActions
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarSliderAction::ScrollBarSliderAction(ScrollBarSlider* slider_, const IRenderWindow *renderWindow_, const Event &event_)
                : slider(slider_), AAction(renderWindow_, event_) {}

    bool ScrollBarSliderAction::execute(const Key &key) {
        slider->updateState(render_window, event);
        Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        vec2i mouse_pos    = canvas->getMousePosition();
        vec2i canvas_pos   = canvas->getPos();
        vec2u canvas_size  = canvas->getSize();

        vec2i cur_pos;
        slider->hovered_sprite.setPosition(slider->pos.x, slider->pos.y);
        slider->pressed_sprite.setPosition(slider->pos.x, slider->pos.y);
        slider->released_sprite.setPosition(slider->pos.x, slider->pos.y);
        slider->sprite.setPosition(slider->pos.x, slider->pos.y);

        if (slider->state == ABarButton::State::Released) {
            slider->is_dragging = false;
        }
        if (slider->state == ABarButton::State::Press) {
            slider->is_dragging = true;
            slider->drag_offset.y = mouse_pos.y + canvas->layer_pos.y - slider->pos.y;
            slider->scroll_obj->setShift({canvas_pos.x - canvas->layer_pos.x, canvas_pos.y - canvas->layer_pos.y});
        }
        else if (slider->is_dragging) {

            int old_slider_pos_y = slider->pos.y;
            int new_slider_pos_y = mouse_pos.y + canvas->layer_pos.y - slider->drag_offset.y;

            new_slider_pos_y = std::min(static_cast<int>(canvas_pos.y + canvas_size.y - slider->size.y),
                    std::max(new_slider_pos_y, canvas_pos.y));
            int slider_shift_y = (new_slider_pos_y - old_slider_pos_y);
            int layer_shift_y = slider_shift_y * (canvas->layer_size.y / canvas_size.y);
            slider->pos.y = new_slider_pos_y;
            slider->sprite.setPosition(slider->pos.x, slider->pos.y);
            slider->scroll_obj->scroll({0, layer_shift_y});
        }

        return true;
    }

    bool ScrollBarSliderAction::isUndoable(const Key &key) {
        return false;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarArrDownAction::ScrollBarArrDownAction(ScrollBarArrDown* arr_down_, const IRenderWindow *renderWindow_, const Event &event_)
                : arr_down(arr_down_), AAction(renderWindow_, event_) {}

    bool ScrollBarArrDownAction::execute(const Key &key) {
        arr_down->updateState(render_window, event);
        ScrollBarVert* scrollbar = static_cast<ScrollBarVert*>(getRootWindow()->getWindowById(kScrollBarVertWindowId));
        ScrollBarSlider* slider = static_cast<ScrollBarSlider*>(scrollbar->getWindowById(1));
        if (arr_down->state == ABarButton::State::Press) {
            vec2i new_pos(slider->getPos().x, slider->getPos().y + 5);
            slider->setPos(new_pos);
            arr_down->scroll_obj->scroll({0, 5});
            arr_down->state = ABarButton::State::Released;
            //slider->sprite.setPosition(slider->getPos().x, slider->getPos().y);
        }
        return true;
    }

    bool ScrollBarArrDownAction::isUndoable(const Key &key) {
        return false;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarArrUpAction::ScrollBarArrUpAction(ScrollBarArrUp* arr_up_, const IRenderWindow *renderWindow_, const Event &event_)
                : arr_up(arr_up_), AAction(renderWindow_, event_) {}

    bool ScrollBarArrUpAction::execute(const Key &key) {
        arr_up->updateState(render_window, event);
        ScrollBarVert* scrollbar = static_cast<ScrollBarVert*>(getRootWindow()->getWindowById(kScrollBarVertWindowId));
        ScrollBarSlider* slider = static_cast<ScrollBarSlider*>(scrollbar->getWindowById(1));
        if (arr_up->getState() == IBarButton::State::Press) {
            vec2i new_pos(slider->getPos().x, slider->getPos().y - 5);
            slider->setPos(new_pos);
            arr_up->scroll_obj->scroll({0, -5});
            arr_up->state = ABarButton::State::Released;
            //slider->sprite.setPosition(slider->getPos().x, slider->getPos().y);
        }
        return true;
    }

    bool ScrollBarArrUpAction::isUndoable(const Key &key) {
        return false;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      Load and Unload
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {

            auto canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(psapi::kCanvasWindowId));
            vec2u size = {20, canvas->getSize().y};
            vec2i pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x),
                         static_cast<int>(canvas->getPos().y)};
            Scrollable* scroll_obj = static_cast<Scrollable*>(canvas);
            scroll_obj->setShift({canvas->getPos().x - canvas->layer_pos.x, canvas->getPos().y - canvas->layer_pos.y});
            auto scrollbar = std::make_unique<ScrollBarVert>(pos, size, kScrollBarVertWindowId, scroll_obj);
            getRootWindow()->addWindow(std::move(scrollbar));
            vec2u layer_size = static_cast<Canvas*>(canvas)->layer_size;

            auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kScrollBarVertWindowId));
            assert(toolbar);

            float slider_scale = canvas->getSize().y / (layer_size.y * 1.0);
            size = {toolbar->getSize().x, static_cast<uint32_t>(slider_scale * canvas->getSize().y)};
            pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x),
                   static_cast<int>(canvas->getPos().y)};
            auto slider = std::make_unique<ScrollBarSlider>(pos, size, 1, scroll_obj);

            size = {toolbar->getSize().x, toolbar->getSize().x};
            pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x),
                   static_cast<int>(canvas->getPos().y)};
            auto arr_up = std::make_unique<ScrollBarArrUp>(pos, size, 2, scroll_obj, "arrow_up.jpg");

            pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x),
                   static_cast<int>(canvas->getPos().y + canvas->getSize().y - size.y)};
            auto arr_down = std::make_unique<ScrollBarArrDown>(pos, size, 3, scroll_obj, "arrow_down.jpg");

            toolbar->addWindow(std::move(slider));
            toolbar->addWindow(std::move(arr_up));
            toolbar->addWindow(std::move(arr_down));

            assert(canvas);
            return true;
        }

        __attribute__((visibility("default"))) void unloadPlugin() {
            IBar* toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            if (toolbar) {
                toolbar->removeWindow(4);
            }
        }
    }
}
