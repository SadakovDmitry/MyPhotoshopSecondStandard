// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "../headers/scrollbar_hor.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>

namespace psapi {
    const int kScrollBarHorWindowId  = kToolBarWindowId + 11;
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

    ScrollBarHor::ScrollBarHor(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_) : id(id_), is_active(true), pos(pos_), size(size_), scale(vec2f(1, 1)), parent(nullptr), scrollbar(), scroll_obj(scroll_obj_) {
        //texture.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y));
        std::vector<sfm::Color> pix_arr(size.x * size.y, sfm::Color(170, 170, 170, 255));
        texture.loadFromMemory(pix_arr.data(), size.x * size.y, sfm::IntRect(pos, size));
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(170, 170, 170, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    void ScrollBarHor::draw(IRenderWindow* renderWindow) {
        renderWindow->draw(&sprite);
        for (const auto& button : scrollbar) {
            button->draw(renderWindow);
        }
    }

    wid_t ScrollBarHor::getId() const { return id; }

    IWindow* ScrollBarHor::getWindowById(wid_t id) {
        if (this->id == id) return this;
        for (auto& button : scrollbar) {
            IWindow* found = button->getWindowById(id);
            if (found) return found;
        }
        return nullptr;
    }

    const IWindow* ScrollBarHor::getWindowById(wid_t id) const {
        if (this->id == id) return this;
        for (const auto& button : scrollbar) {
            const IWindow* found = button->getWindowById(id);
            if (found) return found;
        }
        return nullptr;
    }

    vec2i ScrollBarHor::getPos() const { return pos; }
    vec2u ScrollBarHor::getSize() const { return size; }
    void ScrollBarHor::setParent(const IWindow* parent) { this->parent = parent; }
    void ScrollBarHor::forceActivate() { is_active = true; }
    void ScrollBarHor::forceDeactivate() { is_active = false; }
    bool ScrollBarHor::isActive() const { return is_active == true; }
    bool ScrollBarHor::isWindowContainer() const { return true; }

    void ScrollBarHor::addWindow(std::unique_ptr<IWindow> window) {
        scrollbar.push_back(std::unique_ptr<ABarButton>(dynamic_cast<ABarButton*>(window.release())));

    }

    void ScrollBarHor::removeWindow(wid_t id) {
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

    // ChildInfo ScrollBarHor::getNextChildInfo() const {
    //     ChildInfo info;
    //     int totalWidth = 0;
    //     for (const auto& button : scrollbar) {
    //         totalWidth += button->getSize().x;
    //     }
    //     info.pos = vec2i(pos.x + totalWidth % size.x, pos.y + totalWidth / size.x * 50);
    //     info.size = vec2u(50, 50);
    //     return info;
    // }

    void ScrollBarHor::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const {
        // renderWindow->draw(reinterpret_cast<const sfm::Drawable*>(&(static_cast<const BarButton*>(button)->sprite)));
    }

    bool ScrollBarHor::unPressAllButtons() {
        for (int i = 0; i < scrollbar.size(); i++) {
            scrollbar[i].get()->forceDeactivate();
        }
        return true;
    }

    std::unique_ptr<IAction> ScrollBarHor::createAction(const IRenderWindow* renderWindow_, const Event& event_) {
        return std::make_unique<ScrollBarHorAction>(this, renderWindow_, event_);
    }

    void ScrollBarHor::setSize(const vec2u& size_) {
        size = size_;
    }
    void ScrollBarHor::setPos(const vec2i& pos_) {
        pos = pos_;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                    ScrollBarAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarHorAction::ScrollBarHorAction(ScrollBarHor* scrollbar_, const IRenderWindow *renderWindow_, const Event &event_)
                : scrollbar(scrollbar_), AAction(renderWindow_, event_) {}

    bool ScrollBarHorAction::execute(const Key &key) {
        //std::cout << "\033[31msize: \033[0m" << scrollbar->scrollbar.size() << std::endl;
        for (auto& button : scrollbar->scrollbar) {
            getActionController()->execute(button->createAction(render_window, event));
        }
        return true;
    }

    bool ScrollBarHorAction::isUndoable(const Key &key) {
        return false;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      ScrollBarTools
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarSlider::ScrollBarSlider(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_)
        : ABarButton(pos_, size_, id_), is_dragging(false), scroll_obj(scroll_obj_) {
        //texture.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y));
        std::vector<sfm::Color> pix_arr(size.x * size.y, sfm::Color(255, 255, 255, 255));
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

    ScrollBarArrRight::ScrollBarArrRight(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_, const std::string& file)
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

    std::unique_ptr<IAction> ScrollBarArrRight::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<ScrollBarArrRightAction>(this, renderWindow_, event_);
    }

    void ScrollBarArrRight::updateState(const IRenderWindow *render_window_, const Event &event_) {
        getActionController()->execute(ABarButton::createAction(render_window_, event_));
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarArrLeft::ScrollBarArrLeft(vec2i pos_, vec2u size_, wid_t id_, Scrollable* scroll_obj_, const std::string& file)
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

    std::unique_ptr<IAction> ScrollBarArrLeft::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<ScrollBarArrLeftAction>(this, renderWindow_, event_);
    }

    void ScrollBarArrLeft::updateState(const IRenderWindow *render_window_, const Event &event_) {
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
            slider->drag_offset.x = mouse_pos.x + canvas->layer_pos.x - slider->pos.x;
            slider->scroll_obj->setShift({canvas_pos.x - canvas->layer_pos.x, canvas_pos.y - canvas->layer_pos.y});
        }
        else if (slider->is_dragging) {
            int old_slider_pos_x = slider->pos.x;
            int new_slider_pos_x = mouse_pos.x + canvas->layer_pos.x - slider->drag_offset.x;

            new_slider_pos_x = std::min(static_cast<int>(canvas_pos.x + canvas_size.x - slider->size.x),
                    std::max(new_slider_pos_x, canvas_pos.x));
            int slider_shift_x = (new_slider_pos_x - old_slider_pos_x);
            int layer_shift_x = slider_shift_x * (canvas->layer_size.x / canvas_size.x);
            slider->pos.x = new_slider_pos_x;
            slider->sprite.setPosition(slider->pos.x, slider->pos.y);
            slider->scroll_obj->scroll({layer_shift_x, 0});
        }

        return true;
    }

    bool ScrollBarSliderAction::isUndoable(const Key &key) {
        return false;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarArrLeftAction::ScrollBarArrLeftAction(ScrollBarArrLeft* arr_left_, const IRenderWindow *renderWindow_, const Event &event_)
                : arr_left(arr_left_), AAction(renderWindow_, event_) {}

    bool ScrollBarArrLeftAction::execute(const Key &key) {
        arr_left->updateState(render_window, event);
        ScrollBarHor* scrollbar = static_cast<ScrollBarHor*>(getRootWindow()->getWindowById(kScrollBarHorWindowId));
        ScrollBarSlider* slider = static_cast<ScrollBarSlider*>(scrollbar->getWindowById(1));
        if (arr_left->state == ABarButton::State::Press) {
            vec2i new_pos(slider->getPos().x - 5, slider->getPos().y);
            slider->setPos(new_pos);
            arr_left->scroll_obj->scroll({-5, 0});
            arr_left->state = ABarButton::State::Released;
            //slider->sprite.setPosition(slider->getPos().x, slider->getPos().y);
        }
        return true;
    }

    bool ScrollBarArrLeftAction::isUndoable(const Key &key) {
        return false;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    ScrollBarArrRightAction::ScrollBarArrRightAction(ScrollBarArrRight* arr_right_, const IRenderWindow *renderWindow_, const Event &event_)
                : arr_right(arr_right_), AAction(renderWindow_, event_) {}

    bool ScrollBarArrRightAction::execute(const Key &key) {
        arr_right->updateState(render_window, event);
        ScrollBarHor* scrollbar = static_cast<ScrollBarHor*>(getRootWindow()->getWindowById(kScrollBarHorWindowId));
        ScrollBarSlider* slider = static_cast<ScrollBarSlider*>(scrollbar->getWindowById(1));
        if (arr_right->getState() == IBarButton::State::Press) {
            vec2i new_pos(slider->getPos().x + 5, slider->getPos().y);
            slider->setPos(new_pos);
            arr_right->scroll_obj->scroll({5, 0});
            arr_right->state = ABarButton::State::Released;
            //slider->sprite.setPosition(slider->getPos().x, slider->getPos().y);
        }
        return true;
    }

    bool ScrollBarArrRightAction::isUndoable(const Key &key) {
        return false;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      Load and Unload
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {

            auto canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
            vec2u size = {canvas->getSize().x, 20};
            vec2i pos = {static_cast<int>(canvas->getPos().x),
                         static_cast<int>(canvas->getPos().y + canvas->getSize().y)};
            Scrollable* scroll_obj = static_cast<Scrollable*>(canvas);
            scroll_obj->setShift({canvas->getPos().x - canvas->layer_pos.x, canvas->getPos().y - canvas->layer_pos.y});
            auto scrollbar = std::make_unique<ScrollBarHor>(pos, size, kScrollBarHorWindowId, scroll_obj);
            getRootWindow()->addWindow(std::move(scrollbar));
            vec2u layer_size = static_cast<Canvas*>(canvas)->layer_size;

            auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kScrollBarHorWindowId));
            assert(toolbar);

            float slider_scale = canvas->getSize().x / (layer_size.x * 1.0);
            size = {static_cast<uint32_t>(slider_scale * canvas->getSize().x), toolbar->getSize().y};
            pos = {static_cast<int>(canvas->getPos().x),
                   static_cast<int>(canvas->getPos().y + canvas->getSize().y)};
            auto slider = std::make_unique<ScrollBarSlider>(pos, size, 1, scroll_obj);

            size = {toolbar->getSize().y, toolbar->getSize().y};
            pos = {static_cast<int>(canvas->getPos().x + canvas->getSize().x - size.x),
                   static_cast<int>(canvas->getPos().y + canvas->getSize().y)};
            auto arr_up = std::make_unique<ScrollBarArrRight>(pos, size, 2, scroll_obj, "arrow_right.jpg");

            pos = {static_cast<int>(canvas->getPos().x),
                   static_cast<int>(canvas->getPos().y + canvas->getSize().y)};
            auto arr_down = std::make_unique<ScrollBarArrLeft>(pos, size, 3, scroll_obj, "arrow_left.jpg");

            toolbar->addWindow(std::move(slider));
            toolbar->addWindow(std::move(arr_up));
            toolbar->addWindow(std::move(arr_down));

// //             auto thicknessWindow = static_cast<ChoiceThicknessWindow*>(getRootWindow()->getWindowById(201));
// //             size = {thicknessWindow->getSize().x, 10};
// //             pos = {static_cast<int>(thicknessWindow->getPos().x),
// //                          static_cast<int>(thicknessWindow->getPos().y + thicknessWindow->getSize().y)};
// //             scroll_obj = static_cast<Scrollable*>(thicknessWindow);
// //             scroll_obj->setShift({thicknessWindow->getPos().x, thicknessWindow->getPos().y});
// //             scrollbar = std::make_unique<ScrollBarHor>(pos, size, scroll_obj);
// //             getRootWindow()->addWindow(std::move(scrollbar));
// //             toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kScrollBarHorWindowId));
// //             assert(toolbar);
// //
// //             size = {10, 10};
// //             pos = {static_cast<int>(thicknessWindow->getPos().x),
// //                    static_cast<int>(thicknessWindow->getPos().y + thicknessWindow->getSize().y)};
// //             slider = std::make_unique<ScrollBarSlider>(pos, size, 1, scroll_obj);
// //
// //             size = {toolbar->getSize().y, toolbar->getSize().y};
// //             pos = {static_cast<int>(thicknessWindow->getPos().x + thicknessWindow->getSize().x - size.x),
// //                    static_cast<int>(thicknessWindow->getPos().y + thicknessWindow->getSize().y)};
// //             arr_up = std::make_unique<ScrollBarArrRight>(pos, size, 2, scroll_obj, "arrow_right.jpg");
// //
// //             pos = {static_cast<int>(thicknessWindow->getPos().x),
// //                    static_cast<int>(thicknessWindow->getPos().y + thicknessWindow->getSize().y)};
// //             arr_down = std::make_unique<ScrollBarArrLeft>(pos, size, 3, scroll_obj, "arrow_left.jpg");
// //
// //             toolbar->addWindow(std::move(slider));
// //             toolbar->addWindow(std::move(arr_up));
// //             toolbar->addWindow(std::move(arr_down));
//
//             assert(canvas);
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

