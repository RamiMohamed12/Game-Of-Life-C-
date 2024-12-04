#ifndef GRILLE_H
#define GRILLE_H

#include "Composants/Cellule.h"
#include "Composants/AliveCell.h"
#include "Composants/DeadCell.h"
#include <vector>

class Grille {
public:
    Grille(int largeur, int hauteur);
    ~Grille();
    void mettreAJour();
    void activerCellule(int x, int y);
    bool celluleVivante(int x, int y) const;

private:
    int largeur;
    int hauteur;
    std::vector<std::vector<Cellule*>> cellules;
    void calculerVoisins(std::vector<std::vector<int>>& voisins) const;
};

#endif