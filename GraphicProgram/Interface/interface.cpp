#include "Interface.h"

Interface::Interface(int largeur, int hauteur, int tailleCellule)
    : largeur(largeur), hauteur(hauteur), tailleCellule(tailleCellule),
    fenetre(sf::VideoMode(largeur * tailleCellule, hauteur * tailleCellule), "Jeu de la Vie"),
    grille(largeur, hauteur) {
    
    // Set frame rate limit to make the simulation visible
    fenetre.setFramerateLimit(10);
}

void Interface::executer() {
    while (fenetre.isOpen()) {
        gererEvenements();
        grille.mettreAJour(); // Update the grid state
        fenetre.clear(sf::Color::White); // Clear with white background
        dessiner();
        fenetre.display();
    }
}

void Interface::gererEvenements() {
    sf::Event event;
    while (fenetre.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            fenetre.close();
        }
        // Handle mouse clicks to toggle cells
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x / tailleCellule;
                int y = event.mouseButton.y / tailleCellule;
                grille.activerCellule(x, y);
            }
        }
        // Handle space key to pause/resume
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                // Add pause/resume functionality if needed
            }
        }
    }
}

void Interface::dessiner() {
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            sf::RectangleShape cellule(sf::Vector2f(tailleCellule - 1, tailleCellule - 1));
            cellule.setPosition(x * tailleCellule, y * tailleCellule);
            
            // Living cells are black, dead cells are white with gray border
            if (grille.celluleVivante(x, y)) {
                cellule.setFillColor(sf::Color::Black);
            } else {
                cellule.setFillColor(sf::Color::White);
                cellule.setOutlineColor(sf::Color(200, 200, 200));
                cellule.setOutlineThickness(1);
            }
            
            fenetre.draw(cellule);
        }
    }
}
