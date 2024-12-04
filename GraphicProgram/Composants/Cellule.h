#ifndef CELLULE_H
#define CELLULE_H

class Cellule {
public:
    virtual ~Cellule() = default;
    virtual bool estVivante() const = 0;
    virtual Cellule* prochaineGeneration(int voisinsVivants) const = 0;
};

#endif
