#ifndef COLA_H
#define COLA_H

#include "Nodo.h"

class Cola {
private:
    Nodo* frente;
    Nodo* fin;

public:
    Cola();
    ~Cola();

    void encolar(Enemigo* e);
    Enemigo* desencolar();
    bool estaVacia();
};

#endif
