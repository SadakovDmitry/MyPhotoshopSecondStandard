#ifndef PHOTOSHOP_HPP
#define PHOTOSHOP_HPP

#include <iostream>
#include "actions.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_photoshop.hpp"

using namespace psapi;

class RootWindow : public psapi::IRootWindow {
protected:
    vec2i pos;
    vec2u size;
    layer_id_t layer_id = 0;
    std::vector<std::unique_ptr<IWindow>> windows;
    friend class RootWindowAction;
public:
    RootWindow();

    virtual void draw  (      psapi::IRenderWindow* renderWindow);
    virtual std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event);
    virtual int64_t getId() const;

    virtual void addWindow(std::unique_ptr<IWindow> window);
    virtual psapi::vec2i getPos()  const;
    virtual psapi::vec2u getSize() const;
    virtual void setSize(const vec2u& size);
    virtual void setPos(const vec2i& pos);
    virtual IWindow* getWindowById(wid_t id);
    virtual const IWindow* getWindowById(wid_t id) const;
    virtual void setParent(const IWindow* parent);
    virtual void forceActivate();
    virtual void forceDeactivate();
    virtual bool isActive() const;
    virtual void removeWindow(wid_t id);

    virtual ~RootWindow() = default;

    virtual layer_id_t getUpperLayerId() const;
    virtual layer_id_t increaseLayerId();
    virtual layer_id_t decreaseLayerId();

private:
    bool is_active_ = true;
};

class RootWindowAction : public AAction {
    std::vector<std::unique_ptr<IWindow>> *windows;
public:
    RootWindowAction(std::vector<std::unique_ptr<IWindow>> *windows_, const IRenderWindow *renderWindow_, const Event &event_);
    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

class AWindow : public IWindow {
private:
    wid_t id;
    bool is_active = true;
    vec2u size;
    vec2i pos;
    vec2f scale;
    const IWindow* parent;
public:
    virtual ~AWindow() = default;

    virtual void draw(IRenderWindow* renderWindow) = 0;

    virtual std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event);

    virtual wid_t getId() const;

    virtual IWindow* getWindowById(wid_t id) = 0;

    virtual const IWindow* getWindowById(wid_t id) const = 0;

    virtual vec2i getPos() const;

    virtual vec2u getSize() const;

    virtual void setParent(const IWindow* parent);

    virtual void forceActivate();

    virtual void forceDeactivate();

    virtual bool isActive() const;

    virtual bool isWindowContainer() const;
};

#endif
