#include "my_sfm.hpp"
#include "menubar.hpp"
#include <assert.h>
#include <dlfcn.h>
#include "canvas.hpp"

namespace psapi {

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          MenuBar
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

// MenuBar::MenuBar(vec2i pos_, vec2i size_) : pos(pos_), size(size_) {}

void MenuBar::draw(IRenderWindow* renderWindow) {

    //renderWindow->draw(&sprite);
    sprite.draw(renderWindow);
    for (const auto& button : menubar) {
        button->draw(renderWindow);
    }

    //std::cout << "draw menubar\n";
}

// bool MenuBar::update(const IRenderWindow* renderWindow, const Event& event) {
//     for (auto& button : menubar) {
//         if (button->update(renderWindow, event)) {
//             if (button->getState() == ABarButton::State::Press) {
//                 Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(psapi::kCanvasWindowId));
//                 canvas->setActiveTool(static_cast<ABarButton*>(button.get()));
//                 if(canvas->getActiveTool()) {
//                     canvas->getActiveTool()->action();
//                 }
//             }
//             return true;
//         }
//     }
//     return false;
// }

wid_t MenuBar::getId() const { return id; }

IWindow* MenuBar::getWindowById(wid_t id) {
    if (this->id == id) return this;
    for (auto& button : menubar) {
        IWindow* found = button->getWindowById(id);
        if (found) return found;
    }
    return nullptr;
}

const IWindow* MenuBar::getWindowById(wid_t id) const {
    if (this->id == id) return this;
    for (const auto& button : menubar) {
        const IWindow* found = button->getWindowById(id);
        if (found) return found;
    }
    return nullptr;
}

vec2i MenuBar::getPos() const { return pos; }
vec2u MenuBar::getSize() const { return size; }
void MenuBar::setParent(const IWindow* parent) { this->parent = parent; }
void MenuBar::forceActivate() { is_active = true; }
void MenuBar::forceDeactivate() { is_active = false; }
bool MenuBar::isActive() const { return is_active == true; }
bool MenuBar::isWindowContainer() const { return true; }

void MenuBar::addWindow(std::unique_ptr<IWindow> window) {
    menubar.push_back(std::unique_ptr<psapi::IMenuButton>(dynamic_cast<psapi::IMenuButton*>(window.release())));
}

void MenuBar::removeWindow(wid_t id) {
    menubar.erase(
        std::remove_if(
            menubar.begin(),
            menubar.end(),
            [id](const std::unique_ptr<IMenuButton>& button) {
                return button->getId() == id;
            }
        ),
        menubar.end()
    );
}

void MenuBar::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const {
    // renderWindow->draw(reinterpret_cast<const sfm::Drawable*>(&(static_cast<const ABarButton*>(button)->sprite)));
}

bool MenuBar::unPressAllButtons() {
    for (int i = 0; i < menubar.size(); i++) {
        menubar[i].get()->forceDeactivate();
    }
    return true;
}

std::unique_ptr<IAction> MenuBar::createAction(const IRenderWindow* renderWindow_, const Event& event_) {
    return std::make_unique<MenuBarAction>(&menubar, renderWindow_, event_);
}

void MenuBar::setSize(const vec2u& size_) {
    size = {size_.x, size_.y};
}
void MenuBar::setPos(const vec2i& pos_) {
    pos = {pos_.x, pos_.y};
}

MenuBarAction::MenuBarAction(std::vector<std::unique_ptr<IMenuButton>>* menubar_, const IRenderWindow *renderWindow_, const Event &event_)
                : menubar(menubar_), AAction(renderWindow_, event_) {}

bool MenuBarAction::execute(const Key &key) {
    // std::cout << "\033[32mexecute MenuBarAction\033[0m\n";
    for (auto& button : *menubar) {
        getActionController()->execute(button->createAction(render_window, event));
    }
    return true;
}

bool MenuBarAction::isUndoable(const Key &key) {
    return false;
}

}
