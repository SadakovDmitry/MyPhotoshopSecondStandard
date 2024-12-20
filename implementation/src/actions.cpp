#include "../headers/actions.hpp"

namespace psapi {
// class Key
// {
//     friend class AActionController;
//     Key() {}
// };

AAction::AAction(const IRenderWindow *render_window_, const Event &event_)
    :   render_window(render_window_), event(event_) {}

bool AAction::execute   (const Key& key) {
    std::cout << "execute AAction\n";
    abort();
    return false;
}
bool AAction::isUndoable(const Key& key) {
    return false;
}

AUndoableAction::AUndoableAction(const IRenderWindow *render_window_, const Event &event_)
    :   render_window(render_window_), event(event_) {}

bool AUndoableAction::undo(const Key& key) {
    return false;
}
bool AUndoableAction::redo(const Key& key) {
    return false;
}

bool ActionController::execute(std::unique_ptr<IAction> action) {
    return actionExecute(action.get());
}
bool ActionController::undo() {
    return false;
}
bool ActionController::redo() {
    return false;
}

AActionController *getActionController()
{
    static ActionController controller;
    return &controller;
}

}
