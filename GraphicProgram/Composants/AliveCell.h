#ifndef ALIVECELL_H
#define ALIVECELL_H

#include "Cellule.h"

class AliveCell : public Cellule {
public:
    bool estVivante() const override;
    Cellule* prochaineGeneration(int voisinsVivants) const override;
};

#endif
