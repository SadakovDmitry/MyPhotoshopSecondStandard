#ifndef SFM_IMPLEMENTATION
#define SFM_IMPLEMENTATION


#include <Standard/api_sfm.hpp>
#include <SFML/Graphics.hpp>


namespace psapi
{


namespace sfm
{


class RenderWindow : public IRenderWindow
{
    sf::RenderWindow window_;

    float fps_ = NAN;

public:
    RenderWindow( unsigned int init_width, unsigned int init_height, const std::string &title = "PhotoShop");
    ~RenderWindow() = default;

    bool isOpen() const override;
    void clear()   override;
    void display() override;
    void close()   override;

    void setFps( float fps) override;
    float getFps() const override;

    sf::RenderWindow &getWindow();
    const sf::RenderWindow &getWindow() const;

    vec2u getSize() const override;

    bool pollEvent( Event& event) override;

    void draw( sfm::Drawable *target) override;
};


class PixelsArray : public IPixelsArray
{
    sf::VertexArray pixels_;
    size_t size_;

public:
    PixelsArray( size_t init_size);
    ~PixelsArray() = default;

    void setColor( const Color &color, size_t ind) override;
    Color getColor( size_t ind) const override;

    void setPosition( const vec2i &coord, size_t ind) override;
    void setPosition( const vec2f &coord, size_t ind) override;
    void setPosition( const vec2d &coord, size_t ind) override;
    void setPosition( int x, int y, size_t ind) override;
    void setPosition( float x, float y, size_t ind) override;
    void setPosition( double x, double y, size_t ind) override;

    void draw( IRenderWindow *window) const override;

    // TODO: define static function create
};


class Image : public IImage
{
    sf::Image image_;

    vec2i pos_ = vec2i();

    friend class Texture;
public:
    Image() = default;
    ~Image() = default;

    void create(unsigned int width, unsigned int height, const Color &color=Color(0, 0, 0)) override;
    void create(vec2u size, const Color &color=Color(0, 0, 0)) override;

    void create(unsigned int width, unsigned int height, const Color *pixels) override;
    void create(vec2u size, const Color *pixels) override;

    bool loadFromFile(const std::string &filename) override;
    bool saveToFile(const std::string &filename);

    vec2u getSize() const override;
    void setPixel(unsigned int x, unsigned int y, const Color &color) override;
    void setPixel(vec2u pos, const Color &color) override;

    Color getPixel(unsigned int x, unsigned int y) const override;
    Color getPixel(vec2u pos) const override;

    vec2i getPos() const override;
    void setPos(const vec2i &pos) override;
};


class Texture : public ITexture
{
    sf::Texture texture_;

    friend class Sprite;
public:
    Texture() = default;
    ~Texture() = default;

    bool create(unsigned int width, unsigned int height)                                     override;
    bool loadFromFile  (const std::string& filename,        const IntRect& area = IntRect()) override;
    bool loadFromMemory(const void* data, std::size_t size, const IntRect& area = IntRect()) override;
    vec2u getSize() const                                                                    override;
    std::unique_ptr<IImage> copyToImage() const                                              override;
    void update(const IImage *image)                                                         override;
    void update(const Color *pixels)                                                         override;
    void update(const Color *pixels, unsigned int width, unsigned int height,
                                             unsigned int x,     unsigned int y) override;

    const sf::Texture *get() const;
};


class Sprite : public ISprite
{
    sf::Sprite sprite_;
public:
    Sprite() = default;
    ~Sprite() = default;

    void setTexture(const ITexture *texture, bool reset_rect = false) override;
    void setTextureRect(const IntRect &rectangle) override;

    void setPosition(float x, float y) override;
    void setPosition(const vec2f &pos) override;

    void setScale(float factorX, float factorY) override;
    vec2u getSize() const override;

    void setColor(const Color &color) override;
    Color getColor() const override;

    void setRotation(float angle) override;

    const vec2f getPosition() const override;
    IntRect getGlobalBounds() const override;

    void draw( IRenderWindow *renderWindow) const override;
};


class Font : public IFont
{
    sf::Font font_;
public:
    Font() = default;
    ~Font() = default;

    bool loadFromFile(const std::string& filename) override;

    const sf::Font &getFont() const;
};


class Text : public IText
{
    sf::Text text_;
public:
    Text() = default;
    ~Text() = default;

    void setString(const std::string& string) override;
    void setFont(const IFont* font)           override;
    void setCharacterSize(unsigned int size)  override;
    void setStyle(uint32_t style)             override;
    void setFillColor(const Color* color)     override;
    void setOutlineColor(const Color* color)  override;
    void setOutlineThickness(float thickness) override;
    virtual void setPos(const vec2f &pos)     override;
    virtual void setSize(const vec2f &size)   override;
    void draw(IRenderWindow *renderWindow) const override;
    virtual IntRect getGlobalBounds() const override;

    sf::Text &getText();
};



class RectangleShape : public IRectangleShape
{
    sf::RectangleShape shape_;
    Color color_;
    Color outline_color_;
    vec2u size_;
    vec2f pos_;
    vec2f scale_;

    mutable std::unique_ptr<Image> image_;
    mutable bool is_created_ = false;
public:
    RectangleShape();
    ~RectangleShape();

    void setTexture(const ITexture *texture) override;
    void setFillColor(const Color &color) override;

    void setPosition(const vec2i &pos) override;
    void setPosition(const vec2f &pos) override;
    void setPosition(const vec2d &pos) override;
    void setScale(const vec2f &scale) override;
    void setSize(const vec2u &size) override;
    void setRotation(float angle) override;
    void setOutlineColor(const Color &color) override;
    void setOutlineThickness(float thickness) override;

    float getRotation() const override;
    vec2f getScale() const override;
    vec2f getPosition() const override;
    const Color &getFillColor() const override;
    vec2u getSize() const override;
    float getOutlineThickness() const override;
    const Color &getOutlineColor() const override;
    const IImage *getImage() const override;

    void move(const vec2f &offset) override;

    void draw( IRenderWindow *renderWindow) const override;
};


class EllipseShape : public IEllipseShape
{
public:
    EllipseShape() = default;
    ~EllipseShape() = default;

    void setTexture(const ITexture *texture) override;
    void setFillColor(const Color &color) override;

    void setPosition(const vec2i &pos) override;
    void setPosition(const vec2f &pos) override;
    void setPosition(const vec2d &pos) override;
    void setScale(const vec2f &scale) override;
    void setSize(const vec2u &size) override;
    void setRotation(float angle) override;
    void setOutlineColor(const Color &color) override;
    void setOutlineThickness(float thickness) override;

    float getRotation() const override;
    vec2f getScale() const override;
    vec2f getPosition() const override;
    const Color &getFillColor() const override;
    vec2u getSize() const override;
    float getOutlineThickness() const override;
    const Color &getOutlineColor() const override;
    const IImage *getImage() const override;

    void move(const vec2f &offset) override;
};

// class EllipseShape : public sf::Shape
// {
// public :
//
//     explicit EllipseShape(const sf::Vector2f& radius = sf::Vector2f(0, 0)) :
//     m_radius(radius)
//     {
//         update();
//     }
//
//     void setRadius(const sf::Vector2f& radius)
//     {
//         m_radius = radius;
//         update();
//     }
//
//     const sf::Vector2f& getRadius() const
//     {
//         return m_radius;
//     }
//
//     virtual std::size_t getPointCount() const
//     {
//         return 30; // fixed, but could be an attribute of the class if needed
//     }
//
//     virtual sf::Vector2f getPoint(unsigned int index) const
//     {
//         static const float pi = 3.141592654f;
//
//         float angle = index * 2 * pi / getPointCount() - pi / 2;
//         float x = std::cos(angle) * m_radius.x;
//         float y = std::sin(angle) * m_radius.y;
//
//         return sf::Vector2f(m_radius.x + x, m_radius.y + y);
//     }
//
// private :
//
//     sf::Vector2f m_radius;
// };


} // namespace sfm


} // namespace psapi


#endif // SFM_IMPLEMENTATION
