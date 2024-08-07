#include <SFML/Graphics.hpp>

class Tile {
public:
    Tile(float size, int value);
    Tile(float size, int value, sf::Vector2f& vec);
    void setPosition(float x, float y);
    void setText(int value);
    void setValue(int value);
    sf::Vector2f getPosition() const;
    int getValue() const;
    void draw(sf::RenderWindow& window) const;

private:
    int m_value;
    sf::RectangleShape m_shape;
    sf::Text m_text;
    static sf::Font m_font;
    sf::Color getColorForValue(int value) const;
    void centerText();
    void logFontStatus() const;
};