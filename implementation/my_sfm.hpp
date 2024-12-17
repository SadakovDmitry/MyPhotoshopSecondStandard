#ifndef SFM_PROT_HPP
#define SFM_PROT_HPP

#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_sfm.hpp"
#include <SFML/Graphics.hpp>

namespace psapi {
namespace sfm {

//class RenderWindow;
class Sprite;
class Image;

class RenderWindow : public IRenderWindow {
public:
    sf::RenderWindow window;
    RenderWindow();
    RenderWindow(unsigned int width_, unsigned int height_, const std::string& name_);
    virtual ~RenderWindow() = default;

    virtual bool isOpen() const;
    virtual void clear();
    virtual void display();
    virtual void close();

    virtual vec2u getSize() const;

    virtual bool pollEvent(Event& event);

    virtual void draw(Drawable *target);

    virtual void setFps(float fps);
    virtual float getFps() const;

    static std::unique_ptr<IRenderWindow> create(unsigned int width, unsigned int height, const std::string& name);
};


class Image : public IImage {
    sf::Image image;
    vec2i pos;
public:
    virtual ~Image() = default;

    virtual void create(unsigned int width, unsigned int height, const Color &color=Color(0, 0, 0)) override;
    virtual void create(vec2u size, const Color &color=Color(0, 0, 0)) override;

    virtual void create(unsigned int width, unsigned int height, const Color *pixels) override;
    virtual void create(vec2u size, const Color *pixels) override;

    virtual bool loadFromFile(const std::string &filename) override;

    virtual vec2u getSize() const override;
    virtual void setPixel(unsigned int x, unsigned int y, const Color &color) override;
    virtual void setPixel(vec2u pos, const Color &color) override;

    virtual Color getPixel(unsigned int x, unsigned int y) const override;
    virtual Color getPixel(vec2u pos) const override;

    virtual vec2i getPos() const override;
    virtual void setPos(const vec2i &pos) override;

    static std::unique_ptr<IImage> create();
};

// class Image : public IImage {
//     sf::Image image;
//
// public:
//     virtual ~Image() = default;
//
//     // Implement the 'create' methods
//     virtual void create(unsigned int width, unsigned int height, const Color& color = Color(0, 0, 0)) override {
//         sf::Uint8* pixels = new sf::Uint8[width * height * 4];
//         for (unsigned int i = 0; i < width * height; ++i) {
//             pixels[i * 4 + 0] = color.r;
//             pixels[i * 4 + 1] = color.g;
//             pixels[i * 4 + 2] = color.b;
//             pixels[i * 4 + 3] = color.a;
//         }
//         image.create(width, height, pixels);
//         delete[] pixels;
//     }
//
//     virtual void create(vec2u size, const Color& color = Color(0, 0, 0)) override {
//         create(size.x, size.y, color);
//     }
//
//     virtual void create(unsigned int width, unsigned int height, const Color* pixels) override {
//         image.create(width, height, reinterpret_cast<const sf::Uint8*>(pixels));
//     }
//
//     virtual void create(vec2u size, const Color* pixels) override {
//         create(size.x, size.y, pixels);
//     }
//
//     // Implement the 'loadFromFile' method
//     virtual bool loadFromFile(const std::string& filename) override {
//         return image.loadFromFile(filename);
//     }
//
//     // Implement the 'getSize' method
//     virtual vec2u getSize() const override {
//         auto size = image.getSize();
//         return vec2u(size.x, size.y);
//     }
//
//     // Implement the 'setPixel' methods
//     virtual void setPixel(unsigned int x, unsigned int y, const Color& color) override {
//         image.setPixel(x, y, sf::Color(color.r, color.g, color.b, color.a));
//     }
//
//     virtual void setPixel(vec2u pos, const Color& color) override {
//         setPixel(pos.x, pos.y, color);
//     }
//
//     // Implement the 'getPixel' methods
//     virtual Color getPixel(unsigned int x, unsigned int y) const override {
//         auto color = image.getPixel(x, y);
//         return Color(color.r, color.g, color.b, color.a);
//     }
//
//     virtual Color getPixel(vec2u pos) const override {
//         return getPixel(pos.x, pos.y);
//     }
//
//     // Factory method
//     static std::unique_ptr<IImage> create() {
//         return std::make_unique<Image>();
//     }
// };

//
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

class EllipseShape : public IEllipseShape, public sf::CircleShape {
public:
    explicit EllipseShape(const vec2u& size)
        : sf::CircleShape(size.x / 2.0f) {
        sf::CircleShape::setScale(1.0f, static_cast<float>(size.y) / size.x);
    }

    explicit EllipseShape(unsigned int radius)
        : sf::CircleShape(radius) {}

    virtual void draw(IRenderWindow* window) const override;

    virtual void setTexture(const ITexture* texture) override;

    virtual void setFillColor(const Color& color) override;

    virtual void setPosition(const vec2i& pos) override;

    virtual void setPosition(const vec2f& pos) override;

    virtual void setPosition(const vec2d& pos) override;

    virtual void setScale(const vec2f& scale) override;

    virtual void setSize(const vec2u& size) override;

    virtual void setRotation(float angle) override;

    virtual void setOutlineColor(const Color& color) override;

    virtual void setOutlineThickness(float thickness) override;

    virtual float getRotation() const override;

    virtual vec2f getScale() const override;

    virtual vec2f getPosition() const override;

    virtual const Color& getFillColor() const override;

    virtual vec2u getSize() const override;

    virtual float getOutlineThickness() const override;

    virtual const Color& getOutlineColor() const override;

    virtual IImage* getImage() const override;

    virtual void move(const vec2f& offset) override;

    static std::unique_ptr<IEllipseShape> create(const vec2u& size = vec2u(0, 0));

    static std::unique_ptr<IEllipseShape> create(unsigned int radius);
};

class Texture : public ITexture {
public:
    sf::Texture texture;

    virtual ~Texture() = default;

    virtual bool create(unsigned int width, unsigned int height);
    virtual bool loadFromFile  (const std::string& filename,        const IntRect& area = IntRect());
    virtual bool loadFromMemory(const void* data, std::size_t size, const IntRect& area = IntRect());
    virtual bool loadFromImage (const IImage *image,                const IntRect& area = IntRect());
    virtual vec2u getSize() const;
    virtual std::unique_ptr<IImage> copyToImage() const;
    virtual void update(const IImage *image);
    virtual void update(const Color *pixels);
    virtual void update(const Color *pixels, unsigned int width, unsigned int height,
                                             unsigned int x,     unsigned int y);
    ITexture& operator=(const ITexture& right);
};

class Sprite : public ISprite {
private:
    sf::Sprite sprite;

public:
    virtual ~Sprite() = default;

    virtual void setTexture(const ITexture *texture, bool reset_rect = false);
    virtual void setTextureRect(const IntRect &rectangle);

    virtual void setPosition(float x, float y);
    virtual void setPosition(const vec2f &pos);

    virtual void setScale(float factorX, float factorY);
    virtual vec2u getSize() const;

    virtual void setColor(const Color &color);
    virtual Color getColor() const;

    virtual void setRotation(float angle);

    virtual const vec2f getPosition() const;
    virtual IntRect getGlobalBounds() const;

    virtual void draw(IRenderWindow *window) const;

    static std::unique_ptr<ISprite> create();
};

class Font : public IFont {
private:
    sf::Font font;

public:
    virtual ~Font() = default;

    virtual bool loadFromFile(const std::string& filename);

    const sf::Font& getFont() const;

    static IFont* create();
};

class Text : public IText {
private:
    sf::Text text;

public:
    enum Style
    {
        Regular       = 0,
        Bold          = 1 << 0,
        Italic        = 1 << 1,
        Underlined    = 1 << 2,
        StrikeThrough = 1 << 3
    };

    virtual ~Text() = default;

    virtual void draw(IRenderWindow *window);

    virtual void setString(const std::string& string);

    virtual void setFont(const IFont* font);

    virtual void setCharacterSize(unsigned int size);

    virtual void setStyle(uint32_t style);

    virtual void setFillColor(const Color* color);

    virtual void setOutlineColor(const Color* color);

    virtual void setOutlineThickness(float thickness);
};

};
};


#endif
