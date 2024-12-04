#include "Interface.h"

Interface::Interface(int largeur, int hauteur, int tailleCellule)
    : largeur(largeur), hauteur(hauteur), tailleCellule(tailleCellule),
    fenetre(sf::VideoMode(largeur* tailleCellule, hauteur* tailleCellule), "Jeu de la Vie"),
    grille(largeur, hauteur) {
}

void Interface::executer() {
    while (fenetre.isOpen()) {
        gererEvenements();
        fenetre.clear();
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
    }
}

void Interface::dessiner() {
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            sf::RectangleShape cellule(sf::Vector2f(tailleCellule - 1, tailleCellule - 1));
            cellule.setPosition(x * tailleCellule, y * tailleCellule);
            cellule.setFillColor(grille.celluleVivante(x, y) ? sf::Color::Green : sf::Color::Black);
            fenetre.draw(cellule);
        }
    }
}
