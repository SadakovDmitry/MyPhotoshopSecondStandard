#include "../headers/my_sfm.hpp"
#include "../headers/optionsbar.hpp"
#include <assert.h>
#include <dlfcn.h>
#include "../headers/canvas.hpp"

namespace psapi {

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          OptionsBar
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

// OptionsBar::OptionsBar(vec2i pos_, vec2i size_) : pos(pos_), size(size_) {}

void OptionsBar::draw(IRenderWindow* renderWindow) {

    //renderWindow->draw(&sprite);
    sprite.draw(renderWindow);
    for (const auto& button : optionsbar) {
        assert(button);
        button->draw(renderWindow);
    }

    //std::cout << "draw optionsbar\n";
}

// bool OptionsBar::update(const IRenderWindow* renderWindow, const Event& event) {
//     for (auto& button : optionsbar) {
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

wid_t OptionsBar::getId() const { return id; }

IWindow* OptionsBar::getWindowById(wid_t id) {
    if (this->id == id) return this;
    for (auto& button : optionsbar) {
        IWindow* found = button->getWindowById(id);
        if (found) return found;
    }
    return nullptr;
}

const IWindow* OptionsBar::getWindowById(wid_t id) const {
    if (this->id == id) return this;
    for (const auto& button : optionsbar) {
        const IWindow* found = button->getWindowById(id);
        if (found) return found;
    }
    return nullptr;
}

vec2i OptionsBar::getPos() const { return pos; }
vec2u OptionsBar::getSize() const { return size; }
void OptionsBar::setParent(const IWindow* parent) { this->parent = parent; }
void OptionsBar::forceActivate() { is_active = true; }
void OptionsBar::forceDeactivate() { is_active = false; }
bool OptionsBar::isActive() const { return is_active == true; }
bool OptionsBar::isWindowContainer() const { return true; }

void OptionsBar::addWindow(std::unique_ptr<IWindow> window) {
    optionsbar.push_back(std::unique_ptr<psapi::IWindow>(dynamic_cast<psapi::IWindow*>(window.release())));
}

void OptionsBar::removeWindow(wid_t id) {
    optionsbar.erase(
        std::remove_if(
            optionsbar.begin(),
            optionsbar.end(),
            [id](const std::unique_ptr<IWindow>& button) {
                return button->getId() == id;
            }
        ),
        optionsbar.end()
    );
}

void OptionsBar::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const {
    // renderWindow->draw(reinterpret_cast<const sfm::Drawable*>(&(static_cast<const ABarButton*>(button)->sprite)));
}

bool OptionsBar::unPressAllButtons() {
    for (int i = 0; i < optionsbar.size(); i++) {
        optionsbar[i].get()->forceDeactivate();
    }
    return true;
}

void OptionsBar::removeAllOptions() {
    optionsbar.clear();
    // for (int i = 0; i < optionsbar.size(); i++) {
    //     optionsbar[i].get()->forceDeactivate();
    // }
}

std::unique_ptr<IAction> OptionsBar::createAction(const IRenderWindow* renderWindow_, const Event& event_) {
    return std::make_unique<OptionsBarAction>(&optionsbar, renderWindow_, event_);
}

void OptionsBar::setSize(const vec2u& size_) {
    size = {size_.x, size_.y};
}
void OptionsBar::setPos(const vec2i& pos_) {
    pos = {pos_.x, pos_.y};
}

OptionsBarAction::OptionsBarAction(std::vector<std::unique_ptr<IWindow>>* optionsbar_, const IRenderWindow *renderWindow_, const Event &event_)
                : optionsbar(optionsbar_), AAction(renderWindow_, event_) {}

bool OptionsBarAction::execute(const Key &key) {
    // std::cout << "\033[32mexecute OptionsBarAction\033[0m\n";
    for (auto& button : *optionsbar) {
        getActionController()->execute(button->createAction(render_window, event));
    }
    return true;
}

bool OptionsBarAction::isUndoable(const Key &key) {
    return false;
}

}
