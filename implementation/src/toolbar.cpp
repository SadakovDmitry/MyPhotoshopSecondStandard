#include "../headers/my_sfm.hpp"
#include "../headers/toolbar.hpp"
#include <assert.h>
#include <dlfcn.h>
#include "../headers/canvas.hpp"

namespace psapi {

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          ToolBar
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ToolBar::ToolBar(vec2i pos_, vec2i size_) : pos(pos_), size(size_) {}

void ToolBar::draw(IRenderWindow* renderWindow) {

    //renderWindow->draw(&sprite);
    sprite.draw(renderWindow);
    for (const auto& button : toolbar) {
        button->draw(renderWindow);
    }

    //std::cout << "draw toolbar\n";
}

// bool ToolBar::update(const IRenderWindow* renderWindow, const Event& event) {
//     for (auto& button : toolbar) {
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

wid_t ToolBar::getId() const { return id; }

IWindow* ToolBar::getWindowById(wid_t id) {
    if (this->id == id) return this;
    for (auto& button : toolbar) {
        IWindow* found = button->getWindowById(id);
        if (found) return found;
    }
    return nullptr;
}

const IWindow* ToolBar::getWindowById(wid_t id) const {
    if (this->id == id) return this;
    for (const auto& button : toolbar) {
        const IWindow* found = button->getWindowById(id);
        if (found) return found;
    }
    return nullptr;
}

vec2i ToolBar::getPos() const { return pos; }
vec2u ToolBar::getSize() const { return size; }
void ToolBar::setParent(const IWindow* parent) { this->parent = parent; }
void ToolBar::forceActivate() { is_active = true; }
void ToolBar::forceDeactivate() { is_active = false; }
bool ToolBar::isActive() const { return is_active == true; }
bool ToolBar::isWindowContainer() const { return true; }

void ToolBar::addWindow(std::unique_ptr<IWindow> window) {
    toolbar.push_back(std::unique_ptr<psapi::IBarButton>(dynamic_cast<psapi::IBarButton*>(window.release())));
}

void ToolBar::removeWindow(wid_t id) {
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

void ToolBar::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const {
    // renderWindow->draw(reinterpret_cast<const sfm::Drawable*>(&(static_cast<const ABarButton*>(button)->sprite)));
}

bool ToolBar::unPressAllButtons() {
    for (int i = 0; i < toolbar.size(); i++) {
        toolbar[i].get()->forceDeactivate();
    }
    return true;
}

std::unique_ptr<IAction> ToolBar::createAction(const IRenderWindow* renderWindow_, const Event& event_) {
    return std::make_unique<ToolBarAction>(&toolbar, renderWindow_, event_);
}

void ToolBar::setSize(const vec2u& size_) {
    size = {size_.x, size_.y};
}
void ToolBar::setPos(const vec2i& pos_) {
    pos = {pos_.x, pos_.y};
}

ToolBarAction::ToolBarAction(std::vector<std::unique_ptr<IBarButton>>* toolbar_, const IRenderWindow *renderWindow_, const Event &event_)
                : toolbar(toolbar_), AAction(renderWindow_, event_) {}

bool ToolBarAction::execute(const Key &key) {
    // std::cout << "\033[32mexecute ToolBarAction\033[0m\n";
    for (auto& button : *toolbar) {
        getActionController()->execute(button->createAction(render_window, event));
    }
    return true;
}

bool ToolBarAction::isUndoable(const Key &key) {
    return false;
}

}
