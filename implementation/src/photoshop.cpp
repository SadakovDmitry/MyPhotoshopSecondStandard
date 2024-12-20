
#include "../headers/photoshop.hpp"
#include <assert.h>

using namespace psapi;

sfm::vec2i getScreenSize() {
    assert(0 && "getScreenSize");
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                       IWindowContainer
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
// extern "C" {

/* __attribute__((visibility("default"))) */ psapi::IRootWindow* psapi::getRootWindow() {
    static RootWindow rootWindow;
    return &rootWindow;
}
// }

bool IWindowContainer::isWindowContainer() const {
    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          IWindowVector
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

// void IWindowVector::addWindow(std::unique_ptr<IWindow> window) {
//     windows_.push_back(std::move(window));
// }
//
// void IWindowVector::removeWindow(wid_t id) {
//     windows_.erase(std::remove_if(windows_.begin(), windows_.end(),
//         [id](const std::unique_ptr<IWindow>& win) { return win->getId() == id; }),
//         windows_.end());
// }
//
// IWindow* IWindowVector::getWindowById(wid_t id) {
//     for (const auto& window : windows_) {
//         if (window->getId() == id) {
//             return window.get();
//         }
//     }
//     return nullptr;
// }
//
// const IWindow* IWindowVector::getWindowById(wid_t id) const {
//     for (const auto& window : windows_) {
//         if (window->getId() == id) {
//             return window.get();
//         }
//     }
//     return nullptr;
// }
//
// bool IWindowVector::isWindowContainer() const {
//     return true;
// }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          RootWindow
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

RootWindow::RootWindow() = default;

void RootWindow::addWindow(std::unique_ptr<IWindow> window) {
    windows.push_back(std::move(window));
}

void RootWindow::removeWindow(wid_t id) {
    windows.erase(std::remove_if(windows.begin(), windows.end(),
        [id](const std::unique_ptr<IWindow>& win) { return win->getId() == id; }),
        windows.end());
}

IWindow* RootWindow::getWindowById(wid_t id) {
    for (const auto& window : windows) {
        if (window->getId() == id) {
            return window.get();
        }
    }
    return nullptr;
}

const IWindow* RootWindow::getWindowById(wid_t id) const {
    for (const auto& window : windows) {
        if (window->getId() == id) {
            return window.get();
        }
    }
    return nullptr;
}

void RootWindow::draw(IRenderWindow* renderWindow) {
    renderWindow->clear();
    for (const auto& window : windows) {
        if (window->isActive()) {
            window->draw(renderWindow);
        }
    }
}

// bool RootWindow::update(const IRenderWindow* renderWindow, const sfm::Event& event) {
//     for (const auto& window : windows) {
//         if (window->update(renderWindow, event)) {
//             //return true;
//         }
//     }
//     return false;
// }
std::unique_ptr<IAction> RootWindow::createAction(const IRenderWindow* renderWindow_, const Event& event_) {
    return std::make_unique<RootWindowAction>(&windows, renderWindow_, event_);
}

int64_t RootWindow::getId() const {
    return kRootWindowId;
}

vec2i RootWindow::getPos() const {
    return pos;
}

vec2u RootWindow::getSize() const {
    return size;
}

void RootWindow::setSize(const vec2u& size_) {
    size = size_;
}

void RootWindow::setPos(const vec2i& pos_) {
    pos = pos_;
}

void RootWindow::setParent(const IWindow* parent) {
    std::cout << "\033[31mno no no parent in root window\033[0m\n";
    abort();
}

void RootWindow::forceActivate() {
    std::cout << "no no no root window is always active\n";
    abort();
}

void RootWindow::forceDeactivate() {
    std::cout << "no no no root window is always active\n";
    abort();
}

bool RootWindow::isActive() const { return is_active_ == true; }

//RootWindow::~RootWindow() = default;

layer_id_t RootWindow::getUpperLayerId() const {
    return layer_id;
}
layer_id_t RootWindow::increaseLayerId() {
    layer_id--;
    return layer_id;
}

layer_id_t RootWindow::decreaseLayerId() {
    layer_id--;
    return layer_id;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      RootWindowAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

RootWindowAction::RootWindowAction(std::vector<std::unique_ptr<IWindow>> *windows_, const IRenderWindow *renderWindow_, const Event &event_)
                    : windows(windows_), AAction(renderWindow_, event_) {}

bool RootWindowAction::execute(const Key &key) {
    // std::cout << "execute RootWindowAction\n";
    for (auto& window : *windows) {
        getActionController()->execute(window->createAction(render_window, event));
    }
    return false;
}
bool RootWindowAction::isUndoable(const Key &key) {
    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          AWindow
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------


std::unique_ptr<IAction> AWindow::createAction(const IRenderWindow* renderWindow, const Event& event) {
    return std::make_unique<AAction>(renderWindow, event);
}
wid_t AWindow::getId() const {return id;}

vec2i AWindow::getPos() const { return pos; }
vec2u AWindow::getSize() const { return size; }
void AWindow::setParent(const IWindow* parent) { this->parent = parent; }
void AWindow::forceActivate() { is_active = true; }
void AWindow::forceDeactivate() { is_active = false; }
bool AWindow::isActive() const { return is_active == true; }
bool AWindow::isWindowContainer() const { return true; }

