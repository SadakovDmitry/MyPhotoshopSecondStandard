#include "my_sfm.hpp"
#include "buttons.hpp"
#include <assert.h>
#include <dlfcn.h>
#include "canvas.hpp"

namespace psapi {

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         ABarButton
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void ABarButton::draw(IRenderWindow* renderWindow) {   // different sprites for each state
        renderWindow->draw(&sprite);
        switch (getState()) {
            case State::Hover:
                // std::cout << "Hover button\n";
                renderWindow->draw(&sprite);
                break;
            case State::Press:
                // std::cout << "Press button\n";
                renderWindow->draw(&sprite);
                break;
            case State::Released:
                // std::cout << "Released button\n";
                renderWindow->draw(&sprite);
                break;
            case State::Normal:
                // std::cout << "Normal button\n";
            default:
                // std::cout << "default button\n";
                renderWindow->draw(&sprite);
                break;
        }
    }


    std::unique_ptr<IAction> ABarButton::createAction(const IRenderWindow *renderWindow, const Event &event) {
        return std::make_unique<ABarButtonAction>(this, renderWindow, event);
    }

    wid_t ABarButton::getId() const {
        return id;
    }

    IWindow* ABarButton::getWindowById(wid_t id) {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    const IWindow* ABarButton::getWindowById(wid_t id) const {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    vec2i ABarButton::getPos() const {
        return pos;
    }

    vec2u ABarButton::getSize() const {
        return size;
    }

    void ABarButton::setParent(const IWindow* parent) {
       this->parent = parent;
    }

    void ABarButton::forceActivate() {is_active = true;}

    void ABarButton::forceDeactivate() {is_active = false;}

    bool ABarButton::isActive() const { return is_active == true; }

    bool ABarButton::isWindowContainer() const {
        return false;
    }

    void ABarButton::setState(ABarButton::State state) {
        this->state = state;
    }

    ABarButton::State ABarButton::getState() const {
        return state;
    }

    void ABarButton::setSize(const vec2u& size) {
        this->size = size;
    }
    void ABarButton::setPos(const vec2i& pos) {
        this->pos = pos;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          ABarButtonAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------


    ABarButtonAction::ABarButtonAction(ABarButton *button_, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button(button_) {}


    bool ABarButtonAction::isUndoable(const Key &key) {
        return false;
    }


    bool ABarButtonAction::execute(const Key &key)
    {
        // std::cout << "\033[32mexecute ABarButtonAction\033[0m\n";
        vec2i mouse_pos = sfm::Mouse::getPosition(render_window);
        vec2i pos = button->getPos();
        vec2u size = button->getSize();

        if (pos.x <= mouse_pos.x && mouse_pos.x <= pos.x + size.x &&
            pos.y <= mouse_pos.y && mouse_pos.y <= pos.y + size.y)
        {
            if (event.type == Event::MouseButtonPressed) {
                if (button->state != IBarButton::State::Press) {
                    button->state = IBarButton::State::Press;
                } else {
                    button->state = IBarButton::State::Released;
                    //static_cast<psapi::IOptionsBar *>(psapi::getRootWindow()->getWindowById(psapi::kOptionsBarWindowId))->removeAllOptions();
                }
            } else if (button->state != IBarButton::State::Press) {
                button->state = psapi::IBarButton::State::Hover;
            }
        } else if (button->state == psapi::IBarButton::State::Hover || button->state == psapi::IBarButton::State::Released) {
            button->state = psapi::IBarButton::State::Normal;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         AMenuButton
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void AMenuButton::draw(IRenderWindow* renderWindow) {   // different sprites for each state
        //std::cout << "draw button\n";
        renderWindow->draw(&sprite);
        switch (getState()) {
            case State::Hover:
                // std::cout << "Hover button\n";
                renderWindow->draw(&sprite);
                break;
            case State::Press:
                // std::cout << "Press button\n";
                renderWindow->draw(&sprite);
                break;
            case State::Released:
                // std::cout << "Released button\n";
                renderWindow->draw(&sprite);
                break;
            case State::Normal:
                // std::cout << "Normal button\n";
            default:
                // std::cout << "default button\n";
                renderWindow->draw(&sprite);
                break;
        }
        if(menu && menu->is_active) {
            menu->draw(renderWindow);
        }
    }

    std::unique_ptr<IAction> AMenuButton::createAction(const IRenderWindow *renderWindow, const Event &event) {
        return std::make_unique<AMenuButtonAction>(this, renderWindow, event);
    }

    wid_t AMenuButton::getId() const {
        return id;
    }

    IWindow* AMenuButton::getWindowById(wid_t id) {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    const IWindow* AMenuButton::getWindowById(wid_t id) const {
        if (this->id == id) {
            return this;
        }
        return nullptr;
    }

    vec2i AMenuButton::getPos() const {
        return pos;
    }

    vec2u AMenuButton::getSize() const {
        return size;
    }

    void AMenuButton::setParent(const IWindow* parent) {
       this->parent = parent;
    }

    void AMenuButton::forceActivate() {is_active = true;}

    void AMenuButton::forceDeactivate() {is_active = false;}

    bool AMenuButton::isActive() const { return is_active == true; }

    bool AMenuButton::isWindowContainer() const {
        return false;
    }

    void AMenuButton::setState(AMenuButton::State state) {
        this->state = state;
    }

    AMenuButton::State AMenuButton::getState() const {
        return state;
    }

    void AMenuButton::setSize(const vec2u& size) {
        this->size = size;
    }
    void AMenuButton::setPos(const vec2i& pos) {
        this->pos = pos;
    }

    void AMenuButton::addMenuItem(std::unique_ptr<IWindow> item) {
//         auto* barButton = dynamic_cast<ABarButton*>(item.get());
//         if (!barButton) {
//             throw std::runtime_error("item is not of type IBarButton\n");
//         }
//
//         menu->toolbar.push_back(std::make_unique<ABarButton>(*barButton));
        menu->addWindow(std::move(item));
    }

    void AMenuButton::activateMenu() {
        if (menu) {
            menu->forceActivate();
            // std::cout << "menuActivate\n";
        }
    }
    void AMenuButton::deactivateMenu() {
        if (menu) {
            menu->forceDeactivate();
            // std::cout << "menuDeactivate\n";
        }
    }

    IBar* AMenuButton::getMenu() {
        return menu;
    }

    const IBar* AMenuButton::getMenu() const {
        return menu;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          AMenuButtonAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------


    AMenuButtonAction::AMenuButtonAction(AMenuButton *button_, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button(button_) {}


    bool AMenuButtonAction::isUndoable(const Key &key) {
        return false;
    }


    bool AMenuButtonAction::execute(const Key &key)
    {
        // std::cout << "\033[32mexecute ABarButtonAction\033[0m\n";
        vec2i mouse_pos = sfm::Mouse::getPosition(render_window);
        vec2i pos = button->getPos();
        vec2u size = button->getSize();

        if (pos.x <= mouse_pos.x && mouse_pos.x <= pos.x + size.x &&
            pos.y <= mouse_pos.y && mouse_pos.y <= pos.y + size.y)
        {
            if (event.type == Event::MouseButtonPressed) {
                if (button->state != IBarButton::State::Press) {
                    // std::cout << "\033[36mPRESS\033[0m\n";
                    button->state = IBarButton::State::Press;
                } else {
                    button->state = IBarButton::State::Released;
                    // std::cout << "\033[36mRELISED\033[0m\n";
                    //static_cast<psapi::IOptionsBar *>(psapi::getRootWindow()->getWindowById(psapi::kOptionsBarWindowId))->removeAllOptions();
                }
            } else if (button->state != IBarButton::State::Press) {
                button->state = psapi::IBarButton::State::Hover;
            }
        } else if (button->state == psapi::IBarButton::State::Hover || button->state == psapi::IBarButton::State::Released) {
            button->state = psapi::IBarButton::State::Normal;
        }
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          AMenu
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------


void AMenu::draw(IRenderWindow* renderWindow) {

    // renderWindow->draw(&sprite);
    sprite.draw(renderWindow);
    for (const auto& button : toolbar) {
        button->draw(renderWindow);
    }

    // std::cout << "draw menu\n";
}

wid_t AMenu::getId() const { return id; }

IWindow* AMenu::getWindowById(wid_t id) {
    if (this->id == id) return this;
    for (auto& button : toolbar) {
        IWindow* found = button->getWindowById(id);
        if (found) return found;
    }
    return nullptr;
}

const IWindow* AMenu::getWindowById(wid_t id) const {
    if (this->id == id) return this;
    for (const auto& button : toolbar) {
        const IWindow* found = button->getWindowById(id);
        if (found) return found;
    }
    return nullptr;
}

vec2i AMenu::getPos() const { return pos; }
vec2u AMenu::getSize() const { return size; }
void AMenu::setParent(const IWindow* parent) { this->parent = parent; }
void AMenu::forceActivate() { is_active = true; }
void AMenu::forceDeactivate() { is_active = false; }
bool AMenu::isActive() const { return is_active == true; }
bool AMenu::isWindowContainer() const { return true; }

void AMenu::addWindow(std::unique_ptr<IWindow> window) {
    toolbar.push_back(std::unique_ptr<psapi::IBarButton>(dynamic_cast<psapi::IBarButton*>(window.release())));
}

void AMenu::removeWindow(wid_t id) {
    toolbar.erase(
        std::remove_if(
            toolbar.begin(),
            toolbar.end(),
            [id](const std::unique_ptr<IBarButton>& button) {
                return button->getId() == id;
            }
        ),
        toolbar.end()
    );
}

void AMenu::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const {
    // renderWindow->draw(reinterpret_cast<const sfm::Drawable*>(&(static_cast<const ABarButton*>(button)->sprite)));
}

bool AMenu::unPressAllButtons() {
    for (int i = 0; i < toolbar.size(); i++) {
        toolbar[i].get()->forceDeactivate();
    }
    return true;
}

std::unique_ptr<IAction> AMenu::createAction(const IRenderWindow* renderWindow_, const Event& event_) {
    // std::cout << "\033[32mcreate action menu\033[0m\n";
    return std::make_unique<AMenuAction>(&toolbar, renderWindow_, event_);
}

void AMenu::setSize(const vec2u& size_) {
    size = {size_.x, size_.y};
}
void AMenu::setPos(const vec2i& pos_) {
    pos = {pos_.x, pos_.y};
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                     AMenuAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

AMenuAction::AMenuAction(std::vector<std::unique_ptr<IBarButton>>* toolbar_, const IRenderWindow *renderWindow_, const Event &event_)
                : toolbar(toolbar_), AAction(renderWindow_, event_) {}

bool AMenuAction::execute(const Key &key) {
    // std::cout << "\033[32mexecute ToolBarAction\033[0m\n";
    for (auto& button : *toolbar) {
        getActionController()->execute(button->createAction(render_window, event));
    }
    return true;
}

bool AMenuAction::isUndoable(const Key &key) {
    return false;
}

}
