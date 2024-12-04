#include "Grid.h"
#include"Pattern.h"

Grille::Grille(int largeur, int hauteur)
    : largeur(largeur), hauteur(hauteur), cellules(hauteur, std::vector<Cellule*>(largeur, nullptr)) {
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            cellules[y][x] = new DeadCell();
        }
    }
}

Grille::~Grille() {
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            delete cellules[y][x];
        }
    }
}

void Grille::mettreAJour() {
    std::vector<std::vector<int>> voisins(hauteur, std::vector<int>(largeur, 0));
    calculerVoisins(voisins);

    std::vector<std::vector<Cellule*>> prochaineGrille = cellules;

    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            delete prochaineGrille[y][x];
            if (cellules[y][x]->estVivante()) {
                // Règles pour les cellules vivantes
                if (voisins[y][x] < 2 || voisins[y][x] > 3) {
                    prochaineGrille[y][x] = new DeadCell();
                } else {
                    prochaineGrille[y][x] = new AliveCell();
                }
            } else {
                // Règles pour les cellules mortes
                if (voisins[y][x] == 3) {
                    prochaineGrille[y][x] = new AliveCell();
                } else {
                    prochaineGrille[y][x] = new DeadCell();
                }
            }
        }
    }

    cellules = prochaineGrille;
}

void Grille::activerCellule(int x, int y) {
    if (x >= 0 && x < largeur && y >= 0 && y < hauteur) {
        delete cellules[y][x];
        cellules[y][x] = new AliveCell();
    }
}

bool Grille::celluleVivante(int x, int y) const {
    if (x >= 0 && x < largeur && y >= 0 && y < hauteur) {
        return cellules[y][x]->estVivante();
    }
    return false;
}

void Grille::calculerVoisins(std::vector<std::vector<int>>& voisins) const {
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            int count = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    
                    int nx = x + dx;
                    int ny = y + dy;
                    
                    if (nx >= 0 && nx < largeur && ny >= 0 && ny < hauteur) {
                        if (cellules[ny][nx]->estVivante()) {
                            count++;
                        }
                    }
                }
            }
            voisins[y][x] = count;
        }
    }
}
