#ifndef COLA_H
#define COLA_H

#include "Nodo.h"
#include <SFML/Graphics.hpp>

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
    void dibujarCola(sf::RenderWindow& win, float y);
    Nodo* getFrente();
};

#endif