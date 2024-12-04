#ifndef INTERFACE_H
#define INTERFACE_H

#include "Grille.h"
#include <SFML/Graphics.hpp>

class Interface {
public:
    Interface(int largeur, int hauteur, int tailleCellule);
    void executer();

private:
    int largeur;
    int hauteur;
    int tailleCellule;
    sf::RenderWindow fenetre;
    Grille grille;
    void gererEvenements();
    void dessiner();
};

#endif
