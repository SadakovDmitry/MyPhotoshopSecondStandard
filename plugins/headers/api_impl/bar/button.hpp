#include <Standard/api_bar.hpp>
#include "../sfm.hpp"
#include "../actions.hpp"


using psapi::IBarButton;
using psapi::IBar;
using psapi::IMenuButton;
using psapi::IWindow;
using psapi::IRenderWindow;
using psapi::Event;
using psapi::sfm::ITexture;
using psapi::sfm::ISprite;

using psapi::wid_t;

using psapi::sfm::IRectangleShape;

using psapi::IAction;

using psapi::sfm::vec2i;
using psapi::sfm::vec2u;
using psapi::sfm::vec2f;


class ABarButton : public IBarButton
{
protected:
    std::unique_ptr<ITexture> texture_;
    std::unique_ptr<ISprite> sprite_;
    State state_;

    wid_t id_;

    const IBar *parent_;

    friend class BarButtonAction;
public:
    ABarButton( wid_t init_id, std::unique_ptr<ITexture> &init_texture, std::unique_ptr<ISprite> &init_sprite);
    ~ABarButton() = default;

    virtual void draw(IRenderWindow* renderWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    virtual wid_t getId() const override;
    virtual IWindow* getWindowById(wid_t id) override;
    virtual const IWindow* getWindowById(wid_t id) const override;
    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;

    virtual void setParent(const IWindow* parent) override;
    virtual void setSize(const vec2u &size) override;
    virtual void setPos(const vec2i &pos) override;

    virtual void forceActivate() override;
    virtual void forceDeactivate() override;

    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

    virtual void setState(State state) override;
    virtual State getState() const override;
};


class AMenuButton : public IMenuButton
{
protected:
    std::unique_ptr<IRectangleShape> main_shape_;
    State state_ = IBarButton::State::Normal;

    wid_t id_;

    std::unique_ptr<IBar> bar_;
    bool is_bar_active_ = false;

    const IBar *parent_ = nullptr;

    friend class AMenuButtonAction;
public:
    AMenuButton(wid_t init_id, std::unique_ptr<IRectangleShape> init_shape, std::unique_ptr<IBar> nested_bar);
    virtual ~AMenuButton() = default;

    virtual void addMenuItem(std::unique_ptr<IWindow> item) override;

    virtual void activateMenu() override;
    virtual void deactivateMenu() override;

    virtual IBar *getMenu() override;
    virtual const IBar *getMenu() const override;

    virtual void draw(IRenderWindow* renderWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    virtual wid_t getId() const override;
    virtual IWindow* getWindowById(wid_t id) override;
    virtual const IWindow* getWindowById(wid_t id) const override;
    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;

    virtual void setParent(const IWindow* parent) override;
    virtual void setSize(const vec2u &size) override;
    virtual void setPos(const vec2i &pos) override;

    virtual void forceActivate() override;
    virtual void forceDeactivate() override;

    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

    virtual void setState(State state) override;
    virtual State getState() const override;
};


class BarButtonAction : public AAction
{
    ABarButton *button_;
public:
    BarButtonAction(ABarButton *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


class AMenuButtonAction : public AAction
{
    AMenuButton *button_;
public:
    AMenuButtonAction(AMenuButton *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};
