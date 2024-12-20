#include "brush.hpp"
#include <cassert>
#include <iostream>


#include "headers/windows_id.hpp"
#include "headers/api_impl/bar/options.hpp"


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool onLoadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    psapi::IBar *toolbar = dynamic_cast<psapi::IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::ITexture> texture = sfm::ITexture::create();
    std::unique_ptr<sfm::ISprite> sprite = sfm::ISprite::create();
    texture->loadFromFile("../assets/images/brush48_48.png");
    sprite->setTexture( texture.get());

    std::unique_ptr<ABarButton> brush = std::make_unique<Brush>( kBrushButtonId, texture, sprite);
    brush->setParent( toolbar);
    toolbar->addWindow( std::move( brush));

    return true;
}

void onUnloadPlugin()
{

}


Brush::Brush( wid_t init_id, std::unique_ptr<sfm::ITexture> &init_texture, std::unique_ptr<sfm::ISprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite),
        canvas_( dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId))),
        options_bar_( dynamic_cast<IOptionsBar *>(getRootWindow()->getWindowById( kOptionsBarWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
    assert( options_bar_ && "Failed to cast to options bar" );

    canvas_rect_ = getCanvasIntRect();
    canvas_rect_.pos += sfm::vec2i(0, 20);
    canvas_rect_.size -= sfm::vec2u(20, 20);
}


Brush::~Brush()
{

}


std::unique_ptr<IAction> Brush::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<BrushAction>( this, renderWindow, event);
}


void Brush::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
}


void Brush::drawPoint( ILayer *layer, sfm::vec2i pos)
{
    int radius = 2;
    int radius2 = 4;

    for ( int x = -radius; x < radius; x++ )
    {
        for ( int y = -radius; y < radius; y++ )
        {
            if ( x * x + y * y <= radius2 )
            {
                layer->setPixel( pos, color_);
            }
            pos.y++;
        }
        pos.y -= 2 * radius;
        pos.x++;
    }
}


sfm::vec2i Brush::interpolate( sfm::vec2i p0, sfm::vec2i p1, sfm::vec2i p2, sfm::vec2i p3, float t)
{
    double t2   = t  * t,
           t3   = t2 * t,
           t3_2 = t3 * 2,
           t2_3 = t2 * 3;

    double h00 =  t3_2 - t2_3 + 1;
    double h10 =  t3   - t2 * 2 + t;
    double h01 = -t3_2 + t2_3;
    double h11 =  t3   - t2;

    sfm::vec2f p0f = sfm::vec2f( p0.x, p0.y);
    sfm::vec2f p1f = sfm::vec2f( p1.x, p1.y);
    sfm::vec2f p2f = sfm::vec2f( p2.x, p2.y);
    sfm::vec2f p3f = sfm::vec2f( p3.x, p3.y);

    sfm::vec2f m0 = (p2f - p0f) * 0.5;
    sfm::vec2f m1 = (p3f - p1f) * 0.5;

    sfm::vec2f res = h00 * p1f + h10 * m0 + h01 * p2f + h11 * m1;
    return sfm::vec2i( std::round( res.x), std::round( res.y));
}


void Brush::drawInterpolatedPoints( ILayer *layer, sfm::vec2i new_point)
{
    points_.push_back( new_point);

    if ( points_.size() < 4 )
    {
        drawPoint( layer, new_point);
        return;
    }

    sfm::vec2i &p0 = points_[0];
    sfm::vec2i &p1 = points_[1];
    sfm::vec2i &p2 = points_[2];
    sfm::vec2i &p3 = points_[3];

    float steps = static_cast<float>( std::min<double>( sfm::len( p0, p1) + sfm::len( p1, p2) + sfm::len( p2, p3), 100.f));

    for ( float t = 0; t < steps; t += 0.01 )
    {
        drawPoint( layer, interpolate( p0, p1, p2, p3, t - size_t( t)));
    }
    points_.pop_front();
}


void Brush::addOptions()
{
    for ( auto &option : options_ )
    {
        static_cast<IOptionsBar *>(psapi::getRootWindow()->getWindowById(kOptionsBarWindowId))->addWindow(std::move(option));
    }
    options_.clear();
    options_added_ = true;
}


void Brush::removeOptions()
{
    IOptionsBar *bar = dynamic_cast<IOptionsBar *>(psapi::getRootWindow()->getWindowById(kOptionsBarWindowId));
    assert( bar && "Failed to cast to options bar" );
    bar->removeAllOptions();
    options_added_ = false;
}



void Brush::createOptions()
{
    std::unique_ptr<IColorPalette> palette = IColorPalette::create();
    palette->setColor(color_);
    palette_ = palette.get();
    options_.push_back(std::move(palette));
}


void Brush::setColor( const sfm::Color &new_color)
{
    color_ = new_color;
}


const sfm::Color &Brush::getColor() const
{
    return color_;
}


void Brush::updateState(const IRenderWindow *render_window, const Event &event)
{
    getActionController()->execute(ABarButton::createAction(render_window, event));
}


BrushAction::BrushAction(Brush *init_brush, const IRenderWindow *render_window, const Event &event)
    :   AUndoableAction(render_window, event), brush_(init_brush) {}


bool BrushAction::undo(const Key &key)
{
    std::cout << "Undo brush";

    return true;
}


bool BrushAction::redo(const Key &key)
{
    std::cout << "Redo brush";

    return true;
}


bool BrushAction::isUndoable(const Key &key)
{
    return false;
}


bool BrushAction::execute(const Key &key)
{
    brush_->updateState(render_window_, event_);

    if ( brush_->getState() != IBarButton::State::Press )
    {
        brush_->options_added_ = false;
        return true;
    }
    if ( !brush_->options_added_ )
    {
        brush_->createOptions();
        brush_->addOptions();
    }
    brush_->color_ = brush_->palette_->getColor();
    if ( !( sfm::Mouse::isButtonPressed( sfm::Mouse::Button::Left) || sfm::Mouse::isButtonPressed( sfm::Mouse::Button::Right) ) )
    {
        brush_->points_.clear();
        return true;
    }

    sfm::vec2i mouse_pos = sfm::Mouse::getPosition( render_window_);
    sfm::vec2i relative_pos = mouse_pos - brush_->canvas_rect_.pos;

    brush_->drawInterpolatedPoints( brush_->canvas_->getLayer( brush_->canvas_->getActiveLayerIndex()), relative_pos);

    return true;
}
