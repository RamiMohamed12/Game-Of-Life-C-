#include "AliveCell.h"
#include "DeadCell.h"

bool AliveCell::estVivante() const {
    return true;
}

Cellule* AliveCell::prochaineGeneration(int voisinsVivants) const {
    if (voisinsVivants < 2 || voisinsVivants > 3) {
        return new DeadCell(); // Meurt de sous- ou surpopulation
    }
    return new AliveCell(); // Reste vivante
}
