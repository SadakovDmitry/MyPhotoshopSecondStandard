//#include "api_sfm.hpp"
#include "../headers/my_sfm.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH  = 1200;


namespace psapi {
namespace sfm {

// //-----------------------------------------------------------------------------------------------------------------------------------------------------------------
// //                                                                         Color
// //-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Color::Color(uint8_t init_r, uint8_t init_g, uint8_t init_b, uint8_t init_a /*= 255u*/)
//         :   r(init_r), g(init_g), b(init_b), a(init_a) {}
//
// Color &Color::operator+=(const Color &color)
// {
//     r = uint8_t(std::min(uint16_t(r + color.r), uint16_t(255)));
//     g = uint8_t(std::min(uint16_t(g + color.g), uint16_t(255)));
//     b = uint8_t(std::min(uint16_t(b + color.b), uint16_t(255)));
//     a = std::max(a, color.a);
//
//     return *this;
// }
//
// Color &Color::operator*=(const Color &color)
// {
//     r = uint8_t(std::min(uint16_t(r * color.r), uint16_t(255)));
//     g = uint8_t(std::min(uint16_t(g * color.g), uint16_t(255)));
//     b = uint8_t(std::min(uint16_t(b * color.b), uint16_t(255)));
//     a = std::max(a, color.a);
//
//     return *this;
// }
//
// Color &Color::operator*=(const double cf)
// {
//     r = uint8_t(std::min(uint16_t(r * cf), uint16_t(255)));
//     g = uint8_t(std::min(uint16_t(g * cf), uint16_t(255)));
//     b = uint8_t(std::min(uint16_t(b * cf), uint16_t(255)));
//
//     return *this;
// }
//
// Color &Color::operator*=(const float cf)
// {
//     r = uint8_t(std::min(uint16_t(r * cf), uint16_t(255)));
//     g = uint8_t(std::min(uint16_t(g * cf), uint16_t(255)));
//     b = uint8_t(std::min(uint16_t(b * cf), uint16_t(255)));
//
//     return *this;
// }
//
// Color operator+(const Color &x, const Color &y)
// {
//     Color tmp = x;
//     tmp += y;
//     return tmp;
// }
//
// Color operator*(const Color &x, const Color &y)
// {
//     Color tmp = x;
//     tmp *= y;
//     return tmp;
// }
//
// Color operator*(const Color &x, const float cf)
// {
//     Color tmp = x;
//     tmp *= cf;
//     return tmp;
// }
//
// Color operator*(const Color &x, const double cf)
// {
//     Color tmp = x;
//     tmp *= cf;
//     return tmp;
// }
//
// Color operator*(const float cf, const Color &x)
// {
//     return x * cf;
// }
//
// Color operator*(const double cf, const Color &x)
// {
//     return x * cf;
// }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                       RenderWindow
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

RenderWindow::RenderWindow()
        : window(sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "PhotoShop")) {}

RenderWindow::RenderWindow(unsigned int width_, unsigned int height_, const std::string& name_)
        : window(sf::RenderWindow(sf::VideoMode(width_, height_), name_)) {}

bool RenderWindow::isOpen() const {
    return window.isOpen();
}

void RenderWindow::clear() {
    window.clear();
}

void RenderWindow::display() {
    window.display();
}

void RenderWindow::close() {
    window.close();
}

vec2u RenderWindow::getSize() const {
    return vec2u(WINDOW_HEIGHT, WINDOW_WIDTH);
}

bool RenderWindow::pollEvent(Event& event) {
    sf::Event sf_event;
    bool hasEvent = window.pollEvent(sf_event);

    if (!hasEvent) return false;

    switch (sf_event.type) {
         case sf::Event::MouseMoved:
            event.type = Event::MouseMoved;
            event.mouseMove.x = sf_event.mouseMove.x;
            event.mouseMove.y = sf_event.mouseMove.y;
            break;
        case sf::Event::Closed:
            event.type = Event::Closed;
            break;
        case sf::Event::Resized:
            event.type = Event::Resized;
            event.size.width = sf_event.size.width;
            event.size.height = sf_event.size.height;
            break;
        case sf::Event::LostFocus:
            event.type = Event::LostFocus;
            break;
        case sf::Event::GainedFocus:
            event.type = Event::GainedFocus;
            break;
        case sf::Event::TextEntered:
            event.type = Event::TextEntered;
            event.text.unicode = sf_event.text.unicode;
            break;
        case sf::Event::KeyPressed:
            event.type = Event::KeyPressed;
            event.key.code = static_cast<Keyboard::Key>(sf_event.key.code);
            event.key.alt = sf_event.key.alt;
            event.key.control = sf_event.key.control;
            event.key.shift = sf_event.key.shift;
            event.key.system = sf_event.key.system;
            break;
        case sf::Event::KeyReleased:
            event.type = Event::KeyReleased;
            event.key.code = static_cast<Keyboard::Key>(sf_event.key.code);
            event.key.alt = sf_event.key.alt;
            event.key.control = sf_event.key.control;
            event.key.shift = sf_event.key.shift;
            event.key.system = sf_event.key.system;
            break;
        case sf::Event::MouseWheelScrolled:
            event.type = Event::MouseWheelScrolled;
            event.mouseWheel.wheel = static_cast<Mouse::Wheel>(sf_event.mouseWheelScroll.wheel);
            event.mouseWheel.delta = sf_event.mouseWheelScroll.delta;
            event.mouseWheel.x = sf_event.mouseWheelScroll.x;
            event.mouseWheel.y = sf_event.mouseWheelScroll.y;
            break;
        case sf::Event::MouseButtonPressed:
            event.type = Event::MouseButtonPressed;
            event.mouseButton.button = static_cast<Mouse::Button>(sf_event.mouseButton.button);
            event.mouseButton.x = sf_event.mouseButton.x;
            event.mouseButton.y = sf_event.mouseButton.y;
            break;
        case sf::Event::MouseButtonReleased:
            event.type = Event::MouseButtonReleased;
            event.mouseButton.button = static_cast<Mouse::Button>(sf_event.mouseButton.button);
            event.mouseButton.x = sf_event.mouseButton.x;
            event.mouseButton.y = sf_event.mouseButton.y;
            break;
        case sf::Event::MouseEntered:
            event.type = Event::MouseEntered;
            break;
        case sf::Event::MouseLeft:
            event.type = Event::MouseLeft;
            break;
        default:
            event.type = Event::Unknown;
            break;
    }

    return true;
}

void RenderWindow::draw(Drawable *target) {
    target -> draw(this);
}

// bool Mouse::isButtonPressed(Mouse::Button button) {
//     switch (button)
//     {
//     case Button::Left:
//         return sf::Mouse::isButtonPressed(sf::Mouse::Left);
//     case Button::Right:
//         return sf::Mouse::isButtonPressed(sf::Mouse::Right);
//     case Button::Middle:
//         return sf::Mouse::isButtonPressed(sf::Mouse::Middle);
//     case Button::XButton1:
//         return sf::Mouse::isButtonPressed(sf::Mouse::XButton1);
//     case Button::XButton2:
//         return sf::Mouse::isButtonPressed(sf::Mouse::XButton2);
//     default:
//         return false;
//     }
// }

void RenderWindow::setFps(float fps) {
    std::cout << "not implemented setFps\n";
}
float RenderWindow::getFps() const {
    std::cout << "not implemented getFps\n";
    return 1.0;
}

std::unique_ptr<IRenderWindow> RenderWindow::create(unsigned int width, unsigned int height, const std::string& name) {
    return std::make_unique<RenderWindow>(width, height, name);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         Image
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void Image::create(unsigned int width, unsigned int height, const Color &color) {
        sf::Color sfColor(color.r, color.g, color.b, color.a);
        image.create(width, height, sfColor);
    }

    void Image::create(vec2u size, const Color &color) {
        create(size.x, size.y, color);
    }

    void Image::create(unsigned int width, unsigned int height, const Color *pixels) {
        std::vector<sf::Uint8> pixelData(width * height * 4);
        for (unsigned int i = 0; i < width * height; ++i) {
            pixelData[i * 4 + 0] = pixels[i].r;
            pixelData[i * 4 + 1] = pixels[i].g;
            pixelData[i * 4 + 2] = pixels[i].b;
            pixelData[i * 4 + 3] = pixels[i].a;
        }
        image.create(width, height, pixelData.data());
    }

    void Image::create(vec2u size, const Color *pixels) {
        create(size.x, size.y, pixels);
    }

    bool Image::loadFromFile(const std::string &filename) {
        return image.loadFromFile(filename);
    }

    vec2u Image::getSize() const {
        sf::Vector2u size = image.getSize();
        return {size.x, size.y};
    }

    vec2i Image::getPos() const {
        return {pos.x, pos.y};
    }

    void Image::setPos(const vec2i &pos) {
        this->pos = {pos.x, pos.y};
    }

    void Image::setPixel(unsigned int x, unsigned int y, const Color &color) {
        sf::Color sfColor(color.r, color.g, color.b, color.a);
        image.setPixel(x, y, sfColor);
    }

    void Image::setPixel(vec2u pos, const Color &color) {
        setPixel(pos.x, pos.y, color);
    }

    Color Image::getPixel(unsigned int x, unsigned int y) const {
        sf::Color sfColor = image.getPixel(x, y);
        return Color(sfColor.r, sfColor.g, sfColor.b, sfColor.a);
    }

    Color Image::getPixel(vec2u pos) const {
        return getPixel(pos.x, pos.y);
    }

    std::unique_ptr<IImage> Image::create() {
        return std::make_unique<Image>();
    }


// class RectangleShape : public IRectangleShape, public sf::RectangleShape {
// public:
//     explicit RectangleShape(const vec2u& size)
//         : sf::RectangleShape(sf::Vector2f(size.x, size.y)) {}
//
//     void setTexture(const ITexture* texture) override {
//         // Приведение типа необходимо для работы с интерфейсом ITexture
//         const sf::Texture* sfmlTexture = dynamic_cast<const sf::Texture*>(texture);
//         if (sfmlTexture) {
//             sf::RectangleShape::setTexture(sfmlTexture);
//         }
//     }
//
//     void setFillColor(const Color& color) override {
//         sf::RectangleShape::setFillColor(sf::Color(color.r, color.g, color.b, color.a));
//     }
//
//     void setPosition(const vec2i& pos) override {
//         sf::RectangleShape::setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
//     }
//
//     void setPosition(const vec2f& pos) override {
//         sf::RectangleShape::setPosition(pos.x, pos.y);
//     }
//
//     void setPosition(const vec2d& pos) override {
//         sf::RectangleShape::setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
//     }
//
//     void setScale(const vec2f& scale) override {
//         sf::RectangleShape::setScale(scale.x, scale.y);
//     }
//
//     void setSize(const vec2u& size) override {
//         sf::RectangleShape::setSize(sf::Vector2f(size.x, size.y));
//     }
//
//     void setRotation(float angle) override {
//         sf::RectangleShape::setRotation(angle);
//     }
//
//     void setOutlineColor(const Color& color) override {
//         sf::RectangleShape::setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
//     }
//
//     void setOutlineThickness(float thickness) override {
//         sf::RectangleShape::setOutlineThickness(thickness);
//     }
//
//     float getRotation() const override {
//         return sf::RectangleShape::getRotation();
//     }
//
//     vec2f getScale() const override {
//         sf::Vector2f scale = sf::RectangleShape::getScale();
//         return { scale.x, scale.y };
//     }
//
//     vec2f getPosition() const override {
//         sf::Vector2f pos = sf::RectangleShape::getPosition();
//         return { pos.x, pos.y };
//     }
//
//     const Color& getFillColor() const override {
//         static Color color;
//         sf::Color sfmlColor = sf::RectangleShape::getFillColor();
//         color = { sfmlColor.r, sfmlColor.g, sfmlColor.b, sfmlColor.a };
//         return color;
//     }
//
//     vec2u getSize() const override {
//         sf::Vector2f size = sf::RectangleShape::getSize();
//         return { static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y) };
//     }
//
//     float getOutlineThickness() const override {
//         return sf::RectangleShape::getOutlineThickness();
//     }
//
//     const Color& getOutlineColor() const override {
//         static Color color;
//         sf::Color sfmlColor = sf::RectangleShape::getOutlineColor();
//         color = { sfmlColor.r, sfmlColor.g, sfmlColor.b, sfmlColor.a };
//         return color;
//     }
//
//     IImage getImage() const override {
//         // Not implemented in SFML. Would require rendering to a texture and extracting the image.
//         throw std::runtime_error("getImage not implemented.");
//     }
//
//     void move(const vec2f& offset) override {
//         sf::RectangleShape::move(offset.x, offset.y);
//     }
//
//     static std::unique_ptr<IRectangleShape> create(const vec2u& size = vec2u(0, 0)) {
//         return std::make_unique<RectangleShape>(size);
//     }
// };

    void EllipseShape::draw(IRenderWindow *window) const
    {
        sf::RenderWindow &render_target = dynamic_cast<RenderWindow*>(window)->window;
        render_target.draw(shape_);
    }

    void EllipseShape::draw(ITexture *texture) const
    {
        // texture->renderWindow.draw(shape_);
    }


    void EllipseShape::setTexture(const ITexture *texture)
    {
        shape_.setTexture(&dynamic_cast<const Texture *>(texture)->texture);
        update_image_ = true;
    }


    void EllipseShape::setFillColor(const Color &color)
    {
        shape_.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
        update_image_ = true;
    }


    void EllipseShape::setPosition(const vec2i &pos)
    {
        shape_.setPosition(sf::Vector2f(pos.x, pos.y));
        update_image_ = true;
    }

    void EllipseShape::setPosition(const vec2f &pos)
    {
        shape_.setPosition(sf::Vector2f(pos.x, pos.y));
        update_image_ = true;
    }

    void EllipseShape::setPosition(const vec2d &pos)
    {
        shape_.setPosition(sf::Vector2f(pos.x, pos.y));
        update_image_ = true;
    }


    void EllipseShape::move(const vec2f &offset)
    {
        shape_.move(sf::Vector2f(offset.x, offset.y));
        update_image_ = true;
    }


    void EllipseShape::setScale(const vec2f &scale)
    {
        shape_.setScale(sf::Vector2f(scale.x, scale.y));
        update_image_ = true;
    }

    void EllipseShape::setSize(const vec2u &size)
    {
        unsigned diameter = std::max(size.x, size.y);

        double scale = static_cast<double>(size.y) / static_cast<double>(size.x);
        if(diameter != size.x) scale = 1.0 / scale;

        shape_.setRadius(diameter / 2);
        shape_.setScale(sf::Vector2f(1.0, scale));
        update_image_ = true;
    }


    void EllipseShape::setRotation(float angle)
    {
        shape_.setRotation(angle);
        update_image_ = true;
    }


    void EllipseShape::setOutlineColor(const Color &color)
    {
        shape_.setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
        update_image_ = true;
    }

    void EllipseShape::setOutlineThickness(float thickness)
    {
        shape_.setOutlineThickness(thickness);
        update_image_ = true;
    }


    float EllipseShape::getRotation() const
    {
        return shape_.getRotation();
    }


    vec2f EllipseShape::getScale() const
    {
        sf::Vector2f scale = shape_.getScale();
        return vec2f(scale.x, scale.y);
    }

    vec2u EllipseShape::getSize() const
    {
        double radius = shape_.getRadius();
        return vec2u(2 * radius * shape_.getScale().x,
                     2 * radius * shape_.getScale().y);
    }


    vec2f EllipseShape::getPosition() const
    {
        sf::Vector2f pos = shape_.getPosition();
        return vec2f(pos.x, pos.y);
    }


    const Color& EllipseShape::getFillColor() const
    {
        sf::Color color = shape_.getFillColor();
        Color sfm_color(color.r, color.g, color.b, color.a);
        return sfm_color;
    }


    float EllipseShape::getOutlineThickness() const
    {
        return shape_.getOutlineThickness();
    }

    const Color& EllipseShape::getOutlineColor() const
    {
        sf::Color color = shape_.getOutlineColor();
        Color sfm_color = {color.r, color.g, color.b, color.a};
        return sfm_color;
    }


    const IImage *EllipseShape::getImage() const
    {
        if(update_image_)
        {
            const_cast<EllipseShape *>(this)->update_image();
        }
        return image_.get();
    }


    void EllipseShape::update_image()
    {
        update_image_ = false;
        sf::FloatRect global_rect = shape_.getGlobalBounds();

        sf::RenderTexture render_texture;
        render_texture.create(global_rect.width, global_rect.height);

        sf::Vector2f old_pos = shape_.getPosition();
        shape_.setPosition(old_pos.x - global_rect.left, old_pos.y - global_rect.top);

        render_texture.clear(sf::Color::Transparent);
        render_texture.draw(shape_);
        render_texture.display();

        shape_.setPosition(old_pos);

        sf::Image sf_image = render_texture.getTexture().copyToImage();
        image_ = std::unique_ptr<IImage>(new Image(sf_image));
        image_->setPos(vec2i(global_rect.left, global_rect.top));
    }


    EllipseShape::EllipseShape(unsigned int width, unsigned int height)
    {
        setSize(vec2u(width, height));
    }

    EllipseShape::EllipseShape(const vec2u &size)
    {
        setSize(size);
    }

    std::unique_ptr<IEllipseShape> IEllipseShape::create(unsigned int width, unsigned int height)
    {
        return std::make_unique<EllipseShape>(width, height);
    }

    std::unique_ptr<IEllipseShape> IEllipseShape::create(const vec2u &size)
    {
        return std::make_unique<EllipseShape>(size);
    }

    std::unique_ptr<IEllipseShape> IEllipseShape::create(unsigned int radius)
    {
        return std::make_unique<EllipseShape>(radius * 2, radius * 2);
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         Mouse
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

vec2i Mouse::getPosition() {
    sf::Vector2i pos = sf::Mouse::getPosition();
    return vec2i( pos.x, pos.y);;
}

vec2i Mouse::getPosition(const IRenderWindow *relative_to) {
    sf::Vector2i pos = sf::Mouse::getPosition(static_cast<const RenderWindow*>(relative_to) -> window);
    return vec2i( pos.x, pos.y);;
}

void Mouse::setPosition(const vec2i &position) {
    sf::Mouse::setPosition(sf::Vector2i(position.x, position.y));
}

void Mouse::setPosition(const vec2i &position, const IRenderWindow *relative_to) {
    sf::Mouse::setPosition(sf::Vector2i(position.x, position.y), static_cast<const RenderWindow*>(relative_to) -> window);
}

bool Mouse::isButtonPressed(Button button)
{
    return sf::Mouse::isButtonPressed( sf::Mouse::Button( button));
}


// vec2i Mouse::getPosition()
// {
//     sf::Vector2i pos = sf::Mouse::getPosition();
//     return vec2i( pos.x, pos.y);
// }
//
//
// vec2i Mouse::getPosition(const IRenderWindow *relative_to)
// {
//     const RenderWindow *window = dynamic_cast<const RenderWindow *>( relative_to);
//     assert( window && "Failed to cast to RenderWindow" );
//
//     sf::Vector2i pos = sf::Mouse::getPosition( window->getWindow());
//
//     return vec2i( pos.x, pos.y);
// }
//
//
// void Mouse::setPosition(const vec2i &position)
// {
//     sf::Mouse::setPosition( sf::Vector2i( position.x, position.y));
// }
//
// void Mouse::setPosition(const vec2i &position, const IRenderWindow *relative_to)
// {
//     const RenderWindow *window = dynamic_cast<const RenderWindow *>( relative_to);
//     assert( window && "Failed to cast to RenderWindow" );
//
//     sf::Mouse::setPosition( sf::Vector2i( position.x, position.y), window->getWindow());
// }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         Keyboard
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool Keyboard::isKeyPressed(Key key) {
    switch (key) {
    case Key::A:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    case Key::B:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::B);
    case Key::C:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::C);
    case Key::D:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    case Key::E:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::E);
    case Key::F:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F);
    case Key::G:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::G);
    case Key::H:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::H);
    case Key::I:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::I);
    case Key::J:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::J);
    case Key::K:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::K);
    case Key::L:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::L);
    case Key::M:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::M);
    case Key::N:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::N);
    case Key::O:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::O);
    case Key::P:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::P);
    case Key::Q:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
    case Key::R:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::R);
    case Key::S:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    case Key::T:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::T);
    case Key::U:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::U);
    case Key::V:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::V);
    case Key::W:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    case Key::X:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::X);
    case Key::Y:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Y);
    case Key::Z:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
    case Key::Num0:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Num0);
    case Key::Num1:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Num1);
    case Key::Num2:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Num2);
    case Key::Num3:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Num3);
    case Key::Num4:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Num4);
    case Key::Num5:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Num5);
    case Key::Num6:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Num6);
    case Key::Num7:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Num7);
    case Key::Num8:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Num8);
    case Key::Num9:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Num9);
    case Key::Escape:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
    case Key::LControl:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
    case Key::LShift:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    case Key::LAlt:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
    case Key::LSystem:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::LSystem);
    case Key::RControl:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
    case Key::RShift:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
    case Key::RAlt:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt);
    case Key::RSystem:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::RSystem);
    case Key::Menu:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Menu);
    case Key::LBracket:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket);
    case Key::RBracket:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket);
    case Key::Semicolon:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Semicolon);
    case Key::Comma:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Comma);
    case Key::Period:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Period);
    case Key::Quote:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Quote);
    case Key::Slash:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Slash);
    case Key::Backslash:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Backslash);
    case Key::Tilde:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde);
    case Key::Equal:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Equal);
    case Key::Hyphen:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Hyphen);
    case Key::Space:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    case Key::Enter:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
    case Key::Backspace:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace);
    case Key::Tab:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Tab);
    case Key::PageUp:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp);
    case Key::PageDown:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown);
    case Key::End:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::End);
    case Key::Home:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Home);
    case Key::Insert:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Insert);
    case Key::Delete:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Delete);
    case Key::Add:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Add);
    case Key::Subtract:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract);
    case Key::Multiply:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Multiply);
    case Key::Divide:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Divide);
    case Key::Left:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    case Key::Right:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    case Key::Up:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    case Key::Down:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    case Key::Numpad0:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0);
    case Key::Numpad1:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1);
    case Key::Numpad2:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2);
    case Key::Numpad3:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3);
    case Key::Numpad4:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4);
    case Key::Numpad5:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5);
    case Key::Numpad6:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6);
    case Key::Numpad7:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7);
    case Key::Numpad8:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8);
    case Key::Numpad9:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9);
    case Key::F1:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F1);
    case Key::F2:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F2);
    case Key::F3:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F3);
    case Key::F4:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F4);
    case Key::F5:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F5);
    case Key::F6:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F6);
    case Key::F7:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F7);
    case Key::F8:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F8);
    case Key::F9:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F9);
    case Key::F10:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F10);
    case Key::F11:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F11);
    case Key::F12:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::F12);
    case Key::Pause:
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Pause);
    default:
        return false;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                          Texture
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    bool Texture::create(unsigned int width, unsigned int height) {
        return texture.create(width, height);
    }

    bool Texture::loadFromFile  (const std::string& filename,        const IntRect& area) {
        if (area.size.x == 0 || area.size.y == 0) {
            return texture.loadFromFile(filename);
        } else {
            return texture.loadFromFile(filename, sf::IntRect(area.pos.x, area.pos.y, area.size.x, area.size.y));
        }
    }

    bool Texture::loadFromMemory(const void* data, std::size_t size, const IntRect& area) {
        if (area.size.x == 0 || area.size.y == 0) {
            return texture.loadFromMemory(data, size);
        } else {
            return texture.loadFromMemory(data, size, sf::IntRect(area.pos.x, area.pos.y, area.size.x, area.size.y));
        }
    }

    bool Texture::loadFromImage (const IImage *image,                const IntRect& area) {
        // sf::Image sfImage = convertIImageToSFImage(image);
        // if (area.width == 0 || area.height == 0) {
        //     return texture.loadFromImage(sfImage);
        // } else {
        //     return texture.loadFromImage(sfImage, sf::IntRect(area.left, area.top, area.width, area.height));
        // }
        // assert("\033[31m NOT IMPLEMENTED" && 0);
        return false;
    }

    vec2u Texture::getSize() const {
        sf::Vector2u size = texture.getSize();
        return vec2u(size.x, size.y);
    }

    std::unique_ptr<IImage> Texture::copyToImage() const {
        // sf::Image sfImage = texture.copyToImage();
        // return std::make_unique<IImage>(convertSFImageToIImage(sfImage));
        std::cout << "not implemented copyToImage\n";
        return nullptr;
    }

    void Texture::update(const IImage *image) {
        std::cout << "not implemented update : Image\n";
        //texture.update(static_cast<sfm::Image*>(image));
    }

    void Texture::update(const Color *pixels) {
        texture.update(reinterpret_cast<const sf::Uint8*>(pixels));
    }

    void Texture::update(const Color *pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y) {
        texture.update(reinterpret_cast<const sf::Uint8*>(pixels), width, height, x, y);
    }

    ITexture& Texture::operator=(const ITexture& right) {
        const Texture* textureRight = dynamic_cast<const Texture*>(&right);
        if (textureRight) {
            texture = textureRight->texture;
        }
        return *this;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         Sprite
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void Sprite::setTexture(const ITexture *texture, bool reset_rect) {
        const sf::Texture *sfTexture = &(static_cast<const Texture*>(texture)->texture);
        sprite.setTexture(*sfTexture, reset_rect);
    }

    void Sprite::setTextureRect(const IntRect &rectangle) {
        sprite.setTextureRect(sf::IntRect(rectangle.pos.x, rectangle.pos.y, rectangle.size.x, rectangle.size.y));
    }

    void Sprite::setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    void Sprite::setPosition(const vec2f &pos) {
        sprite.setPosition(pos.x, pos.y);
    }

    void Sprite::setScale(float factorX, float factorY) {
        sprite.setScale(factorX, factorY);
    }

    vec2u Sprite::getSize() const {
        return vec2u(sprite.getTextureRect().width, sprite.getTextureRect().height);
    }

    void Sprite::setColor(const Color &color) {
        sprite.setColor(sf::Color(color.r, color.g, color.b, color.a));
    }

    Color Sprite::getColor() const {
        sf::Color sfColor =  sprite.getColor();
        return sfm::Color(sfColor.r, sfColor.g, sfColor.b, sfColor.a);
    }

    void Sprite::setRotation(float angle) {
        sprite.setRotation(angle);
    }

    const vec2f Sprite::getPosition() const {
        sf::Vector2f pos = sprite.getPosition();
        return vec2f(pos.x, pos.y);
    }

    IntRect Sprite::getGlobalBounds() const {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        vec2i pos = {static_cast<int>(bounds.left), static_cast<int>(bounds.top)};
        vec2u size = {static_cast<unsigned int>(bounds.width), static_cast<unsigned int>(bounds.height)};
        IntRect rect = {pos, size};
        return rect;
    }

    void Sprite::draw(IRenderWindow *window) const {
        static_cast<RenderWindow*>(window)->window.draw(sprite);
    }

    std::unique_ptr<ISprite> Sprite::create() {
        return std::make_unique<Sprite>();
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         Font
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    bool Font::loadFromFile(const std::string& filename) {
        return font.loadFromFile(filename);
    }

    const sf::Font& Font::getFont() const {
        return font;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         Text
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void Text::draw(IRenderWindow *window) {
        static_cast<RenderWindow*>(window)->window.draw(text);
    }

    void Text::setString(const std::string& string) {
        text.setString(string);
    }

    void Text::setFont(const IFont* font) {
        const Font* sfFont = static_cast<const Font*>(font);
        text.setFont(sfFont->getFont());
    }

    void Text::setCharacterSize(unsigned int size) {
        text.setCharacterSize(size);
    }

    void Text::setStyle(uint32_t style) {
        text.setStyle(style);
    }

    void Text::setFillColor(const Color* color) {
        if (color) {
            text.setFillColor(sf::Color(color->r, color->g, color->b, color->a));
        }
    }

    void Text::setOutlineColor(const Color* color) {
        if (color) {
            text.setOutlineColor(sf::Color(color->r, color->g, color->b, color->a));
        }
    }

    void Text::setOutlineThickness(float thickness) {
        text.setOutlineThickness(thickness);
    }

} // sfm
} // psapi
