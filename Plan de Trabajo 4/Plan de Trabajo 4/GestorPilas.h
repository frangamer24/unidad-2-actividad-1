#pragma once
#ifndef GESTORPILAS_H
#define GESTORPILAS_H

#include<SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Pila.h"
#include "Enemigo.h"
#include "Jugador.h"

class GestorPilas {
private:
    struct PisoPila {
        Pila izquierda;
        Pila derecha;
        Enemigo* moviendo = nullptr;
        bool haciaDerecha;
        float yPiso;
    };

    PisoPila pisos[3]; // pisos 2, 4, 6 → índices 0,1,2
    const float* plataformasY;
    sf::Sound* sonidoGolpe;

public:
    GestorPilas(const float* plataformas, const sf::Texture* texturas, sf::Sound* golpe);
    void actualizar(float dt, Jugador& jugador);
    void dibujar(sf::RenderWindow& window);
};

#endif

