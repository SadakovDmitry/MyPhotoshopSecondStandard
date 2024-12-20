#ifndef SCROLL_WINDOWS_IMPL
#define SCROLL_WINDOWS_IMPL


#include <api/api_system.hpp>
#include <api/api_sfm.hpp>
#include <api_impl/sfm.hpp>


using psapi::sfm::vec2i;
using psapi::sfm::vec2u;
using psapi::sfm::vec2f;
using psapi::sfm::RectangleShape;
using psapi::sfm::Color;

using psapi::sfm::Event;


class Scrollable : psapi::sfm::Drawable
{
protected:
    vec2i pos_;
    vec2u size_;
    vec2u window_size_;
    vec2i window_pos_;

    bool is_moved_ = false;
    vec2i last_mouse_pos_ = vec2i();

    std::unique_ptr<RectangleShape> scroll_bar_;
    std::unique_ptr<RectangleShape> scroll_bar_button_;

    bool isOnFocus(const vec2i &mouse_pos) const;
    virtual int getOffset( const psapi::sfm::IRenderWindow *renderWindow, const Event &event);
    bool isOnScrollButton(const vec2i &mouse_pos) const;

public:
    virtual ~Scrollable() {}

    void draw(psapi::sfm::IRenderWindow *renderWindow) const override;
    virtual bool update(const psapi::sfm::IRenderWindow *renderWindow, const Event &event, vec2i &pos) = 0;
};


class HorizontalScroll : public Scrollable
{
public:
    HorizontalScroll( vec2i sector_pos, vec2u sector_size, vec2i window_pos, vec2u window_size);

    bool update(const psapi::sfm::IRenderWindow *renderWindow, const Event &event, vec2i &pos) override;
private:
    int getOffset( const psapi::sfm::IRenderWindow *renderWindow, const Event &event) override;
};


class VerticalScroll : public Scrollable
{
public:
    VerticalScroll( vec2i sector_pos, vec2u sector_size, vec2i window_pos, vec2u window_size);

    bool update(const psapi::sfm::IRenderWindow *renderWindow, const Event &event, vec2i &pos) override;
private:
    int getOffset( const psapi::sfm::IRenderWindow *renderWindow, const Event &event) override;
};


#endif // SCROLL_WINDOWS_IMPL
