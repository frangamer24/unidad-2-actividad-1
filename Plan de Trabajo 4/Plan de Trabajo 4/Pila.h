#ifndef PILA_H
#define PILA_H

#include "Nodo.h"

class Pila {
private:
    Nodo* tope;

public:
    Pila();
    ~Pila();

    void push(Enemigo* e);
    Enemigo* pop();
    bool estaVacia();
    Nodo* getTope();
};

#endif