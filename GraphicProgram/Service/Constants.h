#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/Graphics.hpp>
#include <string>

namespace Constants {
    // Menu Constants
    constexpr int MENU_FONT_SIZE = 36;
    constexpr int MENU_ITEM_HEIGHT = 60;
    const sf::Color MENU_SELECTED_COLOR = sf::Color::White;
    const sf::Color MENU_NORMAL_COLOR = sf::Color(200, 200, 200);

    // Audio Constants
    constexpr float DEFAULT_MUSIC_VOLUME = 50.0f;
    constexpr float MUSIC_FADE_SPEED = 100.0f;
    constexpr float VOLUME_CHANGE_STEP = 5.0f;
    constexpr float MAX_VOLUME = 100.0f;
    constexpr float MIN_VOLUME = 0.0f;

    // Menu Items
    const std::string VOLUME_PREFIX = "Volume: ";
    const std::string MUSIC_PREFIX = "Music: ";
}

template<typename T>
T clamp(const T& value, const T& min, const T& max) {
    return value < min ? min : (value > max ? max : value);
}

#endif