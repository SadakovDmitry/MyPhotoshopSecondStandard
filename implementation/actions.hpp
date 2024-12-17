#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_actions.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_photoshop.hpp"
#include <vector>
#include <iostream>

namespace psapi
{

class AActionController;


class AAction : public IAction {
protected:
    const IRenderWindow *render_window;
    const Event &event;
public:
    virtual ~AAction() = default;
    AAction(const IRenderWindow *render_window_, const Event &event_);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
};

class AUndoableAction : public IUndoableAction {
protected:
    const IRenderWindow *render_window;
    const Event &event;
public:
    AUndoableAction(const IRenderWindow *render_window_, const Event &event_);

    virtual bool undo(const Key& key) override;
    virtual bool redo(const Key& key) override;
};

class ActionController : public AActionController{
private:
    std::vector<std::unique_ptr<IUndoableAction>> done;
    std::vector<std::unique_ptr<IUndoableAction>> undone;

public:
    virtual ~ActionController() = default;

    virtual bool execute(std::unique_ptr<IAction> action) override;

    virtual bool undo() override;
    virtual bool redo() override;
};

AActionController* getActionController();


} // namespace psapi

#endif
