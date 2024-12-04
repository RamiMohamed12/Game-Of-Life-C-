#include "Grille.h"
#include "Patterns.h"

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
    std::vector<std::vector<Cellule*>> prochaineGrille = cellules;

    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            int voisinsVivants = compterVoisinsVivants(x, y);

            // Déterminer le prochain état de la cellule
            Cellule* nouvelleCellule = cellules[y][x]->prochainEtat(voisinsVivants);

            // Remplacer la cellule actuelle
            delete prochaineGrille[y][x];
            prochaineGrille[y][x] = nouvelleCellule;
        }
    }

    cellules = prochaineGrille;
}

void Grille::insereMotif(const std::string& motif, int startX, int startY) {
    auto patterns = Patterns::getPatterns();
    if (patterns.find(motif) != patterns.end()) {
        const auto& pattern = patterns.at(motif);
        for (size_t y = 0; y < pattern.size(); ++y) {
            for (size_t x = 0; x < pattern[y].size(); ++x) {
                int gridX = startX + x;
                int gridY = startY + y;
                if (gridX >= 0 && gridX < largeur && gridY >= 0 && gridY < hauteur) {
                    delete cellules[gridY][gridX];
                    cellules[gridY][gridX] = pattern[y][x] ? new AliveCell() : new DeadCell();
                }
            }
        }
    }
}

int Grille::compterVoisinsVivants(int x, int y) const {
    int voisinsVivants = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue; // Ignorer la cellule actuelle
            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < largeur && ny >= 0 && ny < hauteur) {
                if (cellules[ny][nx]->estVivante()) {
                    ++voisinsVivants;
                }
            }
        }
    }
    return voisinsVivants;
}

void Grille::afficher() const {
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            std::cout << (cellules[y][x]->estVivante() ? "O" : ".");
        }
        std::cout << "\n";
    }
}
