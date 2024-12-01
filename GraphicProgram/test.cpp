#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Hello World");

    // Create a font
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/liberation-sans/LiberationSans-Regular.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    // Create a text
    sf::Text text;
    text.setFont(font);
    text.setString("Hello World");
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);
    
    // Center the text
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(window.getSize().x/2.0f, window.getSize().y/2.0f);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(text);
        window.display();
    }

    return 0;
}
