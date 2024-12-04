#include "Grid.h"

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
    std::vector<std::vector<Cellule*>> prochaineGrille(hauteur, std::vector<Cellule*>(largeur));
    
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            int voisinsVivants = compterVoisinsVivants(x, y);
            
            if (cellules[y][x]->estVivante()) {
                // Règles pour les cellules vivantes
                if (voisinsVivants < 2 || voisinsVivants > 3) {
                    prochaineGrille[y][x] = new DeadCell();
                } else {
                    prochaineGrille[y][x] = new AliveCell();
                }
            } else {
                // Règles pour les cellules mortes
                if (voisinsVivants == 3) {
                    prochaineGrille[y][x] = new AliveCell();
                } else {
                    prochaineGrille[y][x] = new DeadCell();
                }
            }
        }
    }
    
    // Delete old cells and update grid
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            delete cellules[y][x];
        }
    }
    cellules = prochaineGrille;
}

void Grille::activerCellule(int x, int y) {
    if (x >= 0 && x < largeur && y >= 0 && y < hauteur) {
        // Toggle cell state
        delete cellules[y][x];
        if (celluleVivante(x, y)) {
            cellules[y][x] = new DeadCell();
        } else {
            cellules[y][x] = new AliveCell();
        }
    }
}

bool Grille::celluleVivante(int x, int y) const {
    if (x >= 0 && x < largeur && y >= 0 && y < hauteur) {
        return cellules[y][x]->estVivante();
    }
    return false;
}

int Grille::compterVoisinsVivants(int x, int y) const {
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
    return count;
}
