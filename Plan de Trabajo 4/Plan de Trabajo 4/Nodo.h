#ifndef NODO_H
#define NODO_H

#include "Enemigo.h"

struct Nodo {
    Enemigo* enemigo;
    Nodo* siguiente;

    Nodo(Enemigo* e) {
        enemigo = e;
        siguiente = nullptr;
    }
};

#endif