#ifndef DEADCELL_H
#define DEADCELL_H

#include "Cellule.h"

class DeadCell : public Cellule {
public:
    bool estVivante() const override;
    Cellule* prochaineGeneration(int voisinsVivants) const override;
};

#endif
