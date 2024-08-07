//#include "Tile.hpp"
//#include <iostream>
//
//sf::Font Tile::m_font;
//
//Tile::Tile(float size, int value) : m_value(value) {
//    if (m_font.getInfo().family.empty()) {
//        if (!m_font.loadFromFile("ClearSans-Regular.ttf")) {
//            std::cerr << "Failed to load font" << std::endl;
//        }
//    }
//    m_shape.setSize(sf::Vector2f(size, size));
//    m_shape.setFillColor(getColorForValue(value));
//    setText(value);
//}
//
//Tile::Tile(float size, int value, sf::Vector2f& vec) : m_value(value) {
//    if (m_font.getInfo().family.empty()) {
//        if (!m_font.loadFromFile("ClearSans-Regular.ttf")) {
//            std::cerr << "Failed to load font" << std::endl;
//        }
//    }
//    m_shape.setSize(sf::Vector2f(size, size));
//    m_shape.setFillColor(getColorForValue(value));
//    m_shape.setPosition(vec.x, vec.y);
//    setText(value);
//}
//
//void Tile::setPosition(float x, float y) {
//    m_shape.setPosition(x, y);
//    centerText();
//}
//
//void Tile::setText(int value) {
//    m_text.setFont(m_font);
//    m_text.setString(std::to_string(value));
//    m_text.setCharacterSize(14);
//    m_text.setFillColor(sf::Color::Black);
//    centerText();
//}
//
//void Tile::setValue(int value) {
//    m_value = value;
//    setText(value); // Update text when value changes
//}
//
//sf::Vector2f Tile::getPosition() const {
//    return m_shape.getPosition();
//}
//
//int Tile::getValue() const {
//    return m_value;
//}
//
//void Tile::draw(sf::RenderWindow& window) const {
//    window.draw(m_shape);
//    window.draw(m_text);
//}
//
//sf::Color Tile::getColorForValue(int value) const {
//    switch (value) {
//    case 0: return sf::Color(205, 193, 180);
//    case 2: return sf::Color(238, 228, 218);
//    case 4: return sf::Color(237, 224, 200);
//    case 8: return sf::Color(242, 177, 121);
//    case 16: return sf::Color(245, 149, 99);
//    case 32: return sf::Color(246, 124, 95);
//    case 64: return sf::Color(246, 94, 59);
//    case 128: return sf::Color(237, 207, 114);
//    case 256: return sf::Color(237, 204, 97);
//    case 512: return sf::Color(237, 200, 80);
//    case 1024: return sf::Color(237, 197, 63);
//    case 2048: return sf::Color(237, 194, 46);
//    default: return sf::Color(205, 193, 180);
//    }
//}
//
//void Tile::centerText() {
//    if (m_text.getString().isEmpty()) {
//        std::cout << "empty string";
//        return;
//    }
//    sf::FloatRect textRect = m_text.getLocalBounds();
//    m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
//    m_text.setPosition(m_shape.getPosition().x + m_shape.getSize().x / 2.0f, m_shape.getPosition().y + m_shape.getSize().y / 2.0f);
//}