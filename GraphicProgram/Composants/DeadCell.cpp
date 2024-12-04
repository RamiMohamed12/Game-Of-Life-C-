#include "DeadCell.h"
#include "AliveCell.h"

bool DeadCell::estVivante() const {
    return false;
}

Cellule* DeadCell::prochaineGeneration(int voisinsVivants) const {
    if (voisinsVivants == 3) {
        return new AliveCell(); // Devient vivante
    }
    return new DeadCell(); // Reste morte
}
