#ifndef PILA_H
#define PILA_H

#include "Nodo.h"
#include <SFML/Graphics.hpp>

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

    // NUEVO MÉTODO
    void dibujar(sf::RenderWindow& window, float y, bool izquierda = true);
};

#endif