

#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <thread> 
#include <chrono>
sf::Font globalFont;
static int generateRandomInt() {
    // Use random_device to seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);
    // Generate and return the random number
    return dis(gen);
}
enum class Direction {
    Up,
    Down,
    Right,
    Left
};
class Tile {
public:
    Tile(float size, int value, sf::Font& font) : m_value(value), m_font{font} {
        m_shape.setSize(sf::Vector2f(size, size));
        m_shape.setFillColor(getColorForValue(value));
    }
    Tile(float size, int value, sf::Vector2f& vec, sf::Font& font) : m_value(value), m_font{ font } {
        m_shape.setSize(sf::Vector2f(size, size));
        m_shape.setFillColor(getColorForValue(value));
        m_shape.setPosition(vec.x, vec.y);
    }
    //this may seem redundant but for setting m_shape position for tile outside class 
    void setPosition(float x, float y) {
        m_shape.setPosition(x, y);
    }
    

    void setValue(int value) {
        m_value = value;
    }
    void changeColor(int value) {
        m_shape.setFillColor(getColorForValue(value));
        setText();
    }
    sf::Vector2f getPosition() const {
        return m_shape.getPosition();
    }
    int getValue() const {
        return m_value;
    }
    void setText() {
            m_text.setFont(m_font);
            m_value == 0 ? m_text.setString("") : m_text.setString(std::to_string(m_value));
            m_text.setCharacterSize(32); // Adjust size as needed
            m_text.setFillColor(sf::Color::Black);
            centerText();
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(m_shape);
        window.draw(m_text);
        

    }
    

private:
    int m_value;
    sf::RectangleShape m_shape;
    sf::Text m_text;
      sf::Font m_font;
    sf::Color getColorForValue(int value) const {
        switch (value) {
        case 0: return sf::Color(205, 193, 180);
        case 2: return sf::Color(238, 228, 218);
        case 4: return sf::Color(237, 224, 200);
        case 8: return sf::Color(242, 177, 121);
        case 16: return sf::Color(245, 149, 99);
        case 32: return sf::Color(246, 124, 95);
        case 64: return sf::Color(246, 94, 59);
        case 128: return sf::Color(237, 207, 114);
        case 256: return sf::Color(237, 204, 97);
        case 512: return sf::Color(237, 200, 80);
        case 1024: return sf::Color(237, 197, 63);
        case 2048: return sf::Color(237, 194, 46);
        default: return sf::Color(205, 193, 180);
        }
    }
    void centerText() {
        if (m_text.getString().isEmpty()) {
            std::cout << "empty string";
            return;
        }
        sf::FloatRect textRect = m_text.getLocalBounds();
        std::cout << textRect.left << "," << textRect.width << ',' << textRect.top << ',' << textRect.height << '\n';
        m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        std::cout << "shape: " << m_shape.getPosition().x << " size: " << m_shape.getSize().x << " position y: " << m_shape.getPosition().y << " size y:" << m_shape.getSize().y << '\n';
        m_text.setPosition(m_shape.getPosition().x + m_shape.getSize().x / 2.0f, m_shape.getPosition().y + m_shape.getSize().y / 2.0f);
    }
    void logFontStatus() const {
        if (m_font.getInfo().family.empty()) {
            std::cout << "Font is not loaded properly." << std::endl;
            
        }
        else {
            std::cout << "Font is loaded properly: " << m_font.getInfo().family << std::endl;
            std::cout << "Text before drawing: " << m_text.getString().toAnsiString() << std::endl;
        }
    }
};
class GameBoard {
public:
    GameBoard() {
        if (!m_font.loadFromFile("ClearSans-Regular.ttf")) {
            std::cerr << "Failed to load font" << std::endl;
        }
        m_board.resize(m_rows);
        for (int i = 0; i < m_rows; ++i) {
            for (int j = 0; j < m_cols; ++j) {
                m_board[i].emplace_back(m_rectSize, 0,m_font);
            }
        }
        initBoard();
        make2RandomPoints();
    }
    void run() {
        sf::RenderWindow window(sf::VideoMode(600, 600), "2048");
        
        
        m_text.setFont(m_font);
        m_score.setFont(m_font);
        m_text.setString("Score:");
        m_text.setCharacterSize(30);
        m_text.setFillColor(sf::Color::White);
        m_text.setPosition(10, 10);
        m_score.setString("0");
        m_score.setFillColor(sf::Color::White);
        m_score.setPosition(95, 10);
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (!m_lose.getString().isEmpty()) {
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1) {
                        clearBoard();
                    }
                }
                moveTiles(event);
            }
            window.clear(sf::Color(187, 173, 160)); 
            window.draw(m_text);
            window.draw(m_score);
            drawBoard(window);
            if (!m_lose.getString().isEmpty()) {
                window.clear(sf::Color::Transparent);
                window.draw(m_lose);
                window.draw(playAgain);
            }
            window.display();
  
            
        }
    }

private:
    const int m_rows{ 4 };
    const int m_cols{ 4 };
    const float m_rectSize{ 120.f };
    const float m_spacing {10.f};
    const float m_leftSpacing {45.f};
    const float m_topBottomSpacing{ 50.f };
    int m_totalScore{0};
    sf::Text m_text;
    sf::Text m_score;
    sf::Font m_font;
    sf::Text m_lose;
    sf::Text playAgain;
    std::vector<std::vector<Tile>> m_board;
    void initBoard() {
        for (int i = 0; i < m_rows; ++i) {
            for (int j = 0; j < m_cols; ++j) {
                m_board[i][j].setPosition(m_leftSpacing + j * (m_rectSize + m_spacing), m_topBottomSpacing + i * (m_rectSize + m_spacing));
            }
        }
    }
    void drawBoard(sf::RenderWindow& window) {
        for (const auto& row : m_board) {
            for (const auto& tile : row) {  
                    tile.draw(window);      
            }
        }
    }
    void make2RandomPoints() {
        //When initilizing board create 2 random tiles with value 2
        std::pair<int,int> randomPoints{generateRandomInt(),generateRandomInt()};
        std::pair<int, int> randomPoints2{};
        while (true) {
            int a{ generateRandomInt() };
            int b{ generateRandomInt() };
            if (randomPoints.first != a && randomPoints.second != b) {
                randomPoints2.first = a;
                randomPoints2.second = b;
                break;
            }
        }
        sf::Vector2f randomPoint1Vector{ m_board[randomPoints.first][randomPoints.second].getPosition()};
        sf::Vector2f randomPoint2Vector{ m_board[randomPoints2.first][randomPoints2.second].getPosition() };
        m_board[randomPoints.first][randomPoints.second] = Tile{ m_rectSize,2,randomPoint1Vector, m_font };
        m_board[randomPoints.first][randomPoints.second].setText();
        m_board[randomPoints2.first][randomPoints2.second] = Tile{ m_rectSize,2,randomPoint2Vector, m_font};
        m_board[randomPoints2.first][randomPoints2.second].setText();
    }
    void getTotalScore(int value){
        m_totalScore += value;
        m_score.setString(std::to_string(m_totalScore));
    }
    void clearBoard() {
        m_totalScore = 0;
        m_score.setString("0");
        m_lose.setString("");
        playAgain.setString("");

        for (auto& row : m_board) {
            for (auto& tile : row) {
                tile.setValue(0);
                tile.changeColor(0);
            }
        }

        make2RandomPoints();
    }
    void checkAndMoveTile(Direction dir) {
        bool tileMoved{ false };
        if (dir == Direction::Down) {
            for (int i{ m_rows - 2 }; i >= 0; --i) { // start from the second last row
                for (int j{ 0 }; j < m_cols; ++j) {
                    if (m_board[i][j].getValue() != 0) {
                        std::cout << "not zero\n";
                        int k = i;
                        while (k + 1 < m_rows && m_board[k + 1][j].getValue() == 0) { // keep looping moving tile down if k is in our board and tiles below it are 0
                            m_board[k + 1][j].setValue(m_board[k][j].getValue());
                            m_board[k + 1][j].changeColor(m_board[k][j].getValue());
                            m_board[k][j].setValue(0);
                            m_board[k][j].changeColor(0);
                            ++k;
                            tileMoved = true;
                        }
                        if (k + 1 < m_rows && m_board[k + 1][j].getValue() == m_board[k][j].getValue()) { // if tile below our current tile share the same value  merged them 
                            std::cout << "found\n";
                            int valueToMake{ m_board[k][j].getValue() * 2 };
                            m_board[k + 1][j].setValue(valueToMake);
                            m_board[k + 1][j].changeColor(valueToMake);
                            std::cout << "value: " << m_board[k][j].getValue() << '\n';
                            m_board[k][j].setValue(0);
                            m_board[k][j].changeColor(0);
                            getTotalScore(valueToMake);
                            tileMoved = true;

                        }
                    }
                }
            }
        }
        else if (dir == Direction::Up) {
            for (int i{ 1 }; i < m_rows; ++i) {
                for (int j{ 0 }; j < m_cols; ++j) {
                    if (m_board[i][j].getValue() != 0) {
                        std::cout << "not zero\n";
                        int k = i;
                        while (k - 1 >= 0 && m_board[k - 1][j].getValue() == 0) {
                            m_board[k - 1][j].setValue(m_board[k][j].getValue());
                            m_board[k - 1][j].changeColor(m_board[k][j].getValue());
                            m_board[k][j].setValue(0);
                            m_board[k][j].changeColor(0);
                            --k;
                            tileMoved = true;
                        }
                        if (k - 1 >= 0 && m_board[k - 1][j].getValue() == m_board[k][j].getValue()) {
                            std::cout << "found\n";
                            int valueToMake{ m_board[k][j].getValue() * 2 };
                            m_board[k - 1][j].setValue(valueToMake);
                            m_board[k - 1][j].changeColor(valueToMake);
                            std::cout << "value: " << m_board[k][j].getValue() << '\n';
                            m_board[k][j].setValue(0);
                            m_board[k][j].changeColor(0);
                            getTotalScore(valueToMake);
                            tileMoved = true;
                        }
                    }
                }
            }
        }
        else if (dir == Direction::Right) {
            for (int i{ 0 }; i < m_rows; ++i) {
                for (int j{ m_cols - 2 }; j >= 0; --j) {
                    if (m_board[i][j].getValue() != 0) {
                        std::cout << "not zero\n";
                        int k = j;
                        while (k + 1 < m_cols && m_board[i][k + 1].getValue() == 0) {
                            m_board[i][k + 1].setValue(m_board[i][k].getValue());
                            m_board[i][k + 1].changeColor(m_board[i][k].getValue());
                            m_board[i][k].setValue(0);
                            m_board[i][k].changeColor(0);
                            ++k;
                            tileMoved = true;
                        }
                        if (k + 1 < m_cols && m_board[i][k + 1].getValue() == m_board[i][k].getValue()) {
                            std::cout << "found\n";
                            int valueToMake{ m_board[i][k].getValue() * 2 };
                            m_board[i][k + 1].setValue(valueToMake);
                            m_board[i][k + 1].changeColor(valueToMake);
                            std::cout << "value: " << m_board[i][k].getValue() << '\n';
                            m_board[i][k].setValue(0);
                            m_board[i][k].changeColor(0);
                            getTotalScore(valueToMake);
                            tileMoved = true;
                        }
                    }
                }
            }
        }
        else if (dir == Direction::Left) {
            for (int i{ 0 }; i < m_rows; ++i) {
                for (int j{ 1 }; j < m_cols; ++j) {
                    if (m_board[i][j].getValue() != 0) {
                        std::cout << "not zero\n";
                        int k = j;
                        while (k - 1 >= 0 && m_board[i][k - 1].getValue() == 0) {
                            m_board[i][k - 1].setValue(m_board[i][k].getValue());
                            m_board[i][k - 1].changeColor(m_board[i][k].getValue());
                            m_board[i][k].setValue(0);
                            m_board[i][k].changeColor(0);
                            --k;
                            tileMoved = true;
                        }
                        if (k - 1 >= 0 && m_board[i][k - 1].getValue() == m_board[i][k].getValue()) {
                            std::cout << "found\n";
                            int valueToMake{ m_board[i][k].getValue() * 2 };
                            m_board[i][k - 1].setValue(valueToMake);
                            m_board[i][k - 1].changeColor(valueToMake);
                            std::cout << "value: " << m_board[i][k].getValue() << '\n';
                            m_board[i][k].setValue(0);
                            m_board[i][k].changeColor(0);
                            getTotalScore(valueToMake);
                            tileMoved = true;
                        }
                    }
                }
            }
        }
        if (checkWin()) {
            displayWinScreen();
        }
        if (tileMoved) {
           // std::this_thread::sleep_for(std::chrono::milliseconds(100)); // added timeout because tiles were spawning in too fast after moving a tile causing it to look like undefined behavior 
            generateTile();
        }
        else if (checkLoss()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            displayLoseScreen();
        }

    }
    void moveTiles(sf::Event event) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                checkAndMoveTile(Direction::Up);
            }
            else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
                checkAndMoveTile(Direction::Down);
            }
            else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
                checkAndMoveTile(Direction::Right);
            }
            else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                checkAndMoveTile(Direction::Left);
            }
        }
    }

    void generateTile() {
        // check if there is at least one empty tile
        bool hasEmptyTile = false;
        for (const auto& row : m_board) {
            for (const auto& tile : row) {
                if (tile.getValue() == 0) {
                    hasEmptyTile = true;
                    break;
                }
            }
            if (hasEmptyTile) break;
        }

        if (!hasEmptyTile) {
            return; // exit the function if no empty tiles are available
        }

        // try to find an empty tile
        while (true) {
            std::pair<int, int> randomPoints{ generateRandomInt(), generateRandomInt() };

            if (m_board[randomPoints.first][randomPoints.second].getValue() == 0) {
                // set the value and change the color of the tile
                m_board[randomPoints.first][randomPoints.second].setValue(2);
                m_board[randomPoints.first][randomPoints.second].changeColor(2);
                break; // exit the loop once an empty tile is found
            }
        }
    }
    bool checkWin() {
        // loop through each row and column checking if tile value is 2048
        for (const auto& row : m_board) {
            for (const auto& tile : row) {
                if (tile.getValue() == 2048) {
                    return true;
                }
            }
        }
        return false;
    }
    bool checkLoss() {
        for (int i = 0; i < m_rows; ++i) {
            for (int j = 0; j < m_cols; ++j) {
                int currentValue = m_board[i][j].getValue();
                // Check if the current tile can be merged with any of its neighbors
                if ((i > 0 && currentValue == m_board[i - 1][j].getValue()) || // Up
                    (i < m_rows - 1 && currentValue == m_board[i + 1][j].getValue()) || // Down
                    (j > 0 && currentValue == m_board[i][j - 1].getValue()) || // Left
                    (j < m_cols - 1 && currentValue == m_board[i][j + 1].getValue()) || currentValue == 0) { // Right
                    return false;
                }
            }
        }
        return true;
    }
    void displayLoseScreen() {

        m_lose.setFont(m_font);
        playAgain.setFont(m_font);
        m_lose.setString("You Lost!");
        playAgain.setString("Press 1 to play again.");
        m_lose.setCharacterSize(100);
        playAgain.setCharacterSize(40);
        m_lose.setFillColor(sf::Color::White);
        playAgain.setFillColor(sf::Color::White);
        m_lose.setPosition(100, 200);
        playAgain.setPosition(100, 400);

    }
    void displayWinScreen() {

        m_lose.setFont(m_font);
        playAgain.setFont(m_font);
        m_lose.setString("You Win!");
        playAgain.setString("Press 1 to play again.");
        m_lose.setCharacterSize(100);
        playAgain.setCharacterSize(40);
        m_lose.setFillColor(sf::Color::White);
        playAgain.setFillColor(sf::Color::White);
        m_lose.setPosition(100, 200);
        playAgain.setPosition(100, 400);

    }
};

int main()
{
    
    GameBoard board{};
    board.run();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
