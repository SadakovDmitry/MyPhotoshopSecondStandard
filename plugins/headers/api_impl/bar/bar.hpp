#ifndef PHOTOSHOP_BAR_IMPLEMENTATION
#define PHOTOSHOP_BAR_IMPLEMENTATION


#include <api/api_bar.hpp>
#include <list>
#include <api/api_sfm.hpp>
#include <api_impl/actions.hpp>
#include <api_impl/sfm.hpp>


using namespace psapi;


class ABar : public IBar
{
protected:
    wid_t id_;

    vec2u size_;
    vec2i pos_;
    vec2u buttons_size_;

    bool is_active_;

    const IWindow *parent_;

    std::list<std::unique_ptr<IBarButton>> buttons_;

    std::unique_ptr<sfm::RectangleShape> main_shape_;
    std::unique_ptr<sfm::RectangleShape> normal_;
    std::unique_ptr<sfm::RectangleShape> onHover_;
    std::unique_ptr<sfm::RectangleShape> pressed_;
    std::unique_ptr<sfm::RectangleShape> released_;

    wid_t last_pressed_id_ = -1;

    friend class ABarAction;
public:
    ABar( wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape,  std::unique_ptr<sfm::RectangleShape> &normal,
                                                            std::unique_ptr<sfm::RectangleShape> &onHover,
                                                            std::unique_ptr<sfm::RectangleShape> &pressed,
                                                            std::unique_ptr<sfm::RectangleShape> &released);
    virtual ~ABar() = default;

    virtual void draw(IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

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

    virtual void addWindow(std::unique_ptr<IWindow> window) override;
    virtual void removeWindow(wid_t id) override;

    virtual bool unPressAllButtons() override;
    virtual void finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const override;

    virtual vec2i calculateNextPos(vec2i init_pos);
};


class AOptionsBar : public IOptionsBar
{
protected:
    wid_t id_ = kOptionsBarWindowId;

    std::unique_ptr<sfm::RectangleShape> main_shape_;
    // std::unique_ptr<sfm::RectangleShape> normal_;
    // std::unique_ptr<sfm::RectangleShape> released_;
    // std::unique_ptr<sfm::RectangleShape> onHover_;
    // std::unique_ptr<sfm::RectangleShape> pressed_;
    vec2u size_;
    vec2i pos_;
    vec2u option_size_;

    bool is_active_ = false;

    const IWindow *parent_ = nullptr;

    std::list<std::unique_ptr<IWindow>> options_;

    friend class AOptionsBarAction;
public:
    AOptionsBar(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape);
    virtual ~AOptionsBar() = default;

    void draw(IRenderWindow* renderWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    wid_t getId() const override;
    IWindow* getWindowById(wid_t id) override;
    const IWindow* getWindowById(wid_t id) const override;
    vec2i getPos() const override;
    vec2u getSize() const override;

    void setParent(const IWindow* parent) override;
    void setSize(const vec2u &size) override;
    void setPos(const vec2i &pos) override;

    void forceActivate() override;
    void forceDeactivate() override;

    bool isActive() const override;
    bool isWindowContainer() const override;

    void addWindow(std::unique_ptr<IWindow> window) override;
    void removeWindow(wid_t id) override;

    void removeAllOptions() override;

    virtual vec2i calculateNextPos(vec2i init_pos);
};


class ABarAction : public AAction
{
    ABar *bar_;
    bool options_removed_ = false;
public:
    ABarAction(ABar *bar, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


class AOptionsBarAction : public AAction
{
    AOptionsBar *bar_;
public:
    AOptionsBarAction(AOptionsBar *bar, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


#endif // PHOTOSHOP_BAR_IMPLEMENTATION
