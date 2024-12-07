#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Grid.h"
#include "Pattern.h"
#include "Menu.h"
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

enum class GameState {
    Menu,
    Playing,
    Tutorial,
    MusicSettings
};

class Game {
public:
    Game();
    void run();

private:
    // Constants
    static const int CELL_SIZE = 15;
    static const int STATUS_FONT_SIZE = 20;
    static const int TOTAL_TUTORIAL_PAGES = 5;
    static const int TUTORIAL_TITLE_SIZE = 48;
    static const int TUTORIAL_TEXT_SIZE = 28;
    static const int TUTORIAL_SUBTITLE_SIZE = 32;

    const float MIN_ZOOM = 0.1f;
    const float MAX_ZOOM = 5.0f;
    const float MIN_SPEED = 0.1f;
    const float MAX_SPEED = 10.0f;
    const sf::Time SAVE_MESSAGE_DURATION = sf::seconds(2);

    // Colors
    const sf::Color BACKGROUND_COLOR = sf::Color(18, 18, 24);
    const sf::Color MENU_SELECTED_COLOR = sf::Color(129, 140, 248);
    const sf::Color MENU_HOVER_COLOR = sf::Color(99, 110, 218);
    const sf::Color MENU_NORMAL_COLOR = sf::Color(203, 213, 225);
    const sf::Color ACCENT_COLOR = sf::Color(244, 114, 182);
    const sf::Color SUBTITLE_COLOR = sf::Color(148, 163, 184);

    // Core components
    sf::RenderWindow window;
    sf::Font font;
    Menu menu;
    Grid grid;

    // Background
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::RenderTexture renderTexture;

    // Music System
    std::vector<std::string> musicPlaylist;
    sf::Music currentMusic;
    size_t currentTrackIndex;
    float musicVolume;
    bool isMusicPlaying;
    sf::Text musicInfo;
    sf::Clock musicDisplayClock;
    const sf::Time MUSIC_INFO_DISPLAY_TIME = sf::seconds(3);

    // Game state
    GameState gameState;
    bool isRunning;
    std::string selectedPattern;
    bool manualMode;
    int currentTutorialPage;

    // View control
    float zoomLevel;
    sf::Vector2f viewOffset;
    float simulationSpeed;

    // Statistics
    int generation;
    int livingCells;
    float fps;

    // Timing
    sf::Clock clock;
    sf::Clock fpsClock;

    // Save system
    std::string saveMessage;
    sf::Clock saveMessageClock;

    // Pattern storage
    std::map<std::string, std::vector<std::vector<bool>>> patterns;

    // Utility functions
    float clamp(float value, float min, float max) const {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    // Member functions
    void handleEvents();
    void update();
    void draw();
    void drawStats();

    // Tutorial functions
    void drawTutorial();
    void drawTutorialBackground();
    void drawTutorialHeader();
    void drawTutorialPage1();
    void drawTutorialPage2();
    void drawTutorialPage3();
    void drawTutorialPage4();
    void drawTutorialPage5();
    void drawTutorialNavigation();
    void drawTutorialProgress();

    // Music functions
    void initMusicSystem();
    void playNextTrack();
    void playPreviousTrack();
    void toggleMusic();
    void updateMusicVolume(float delta);
    void loadMusicPlaylist();
    void updateMusicInfo();
    void drawMusicInfo();

    // Background functions
    void initBackground();
    void updateBackground();

    // Utility functions
    void reset();
    void loadPatterns();
    void saveGame();
    void loadGame();
    void processMenuSelection();
    void handleZoom(float delta);
    void handlePan(const sf::Vector2f& delta);
    void updateStats();
    void loadCustomPattern();
    void handleMouseEvents(const sf::Event& event);
    sf::Vector2i lastMousePos;
    sf::Vector2i calculateOptimalGridSize() const;

    void drawMusicSettings();
    void handleMusicSettingsEvents(const sf::Event& event);

    // Additional member variables
    size_t selectedMusicTrack;
    std::vector<std::string> musicTrackNames;  // Friendly names for display
};

#endif
