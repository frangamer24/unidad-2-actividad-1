// GestorColas.h
#ifndef GESTORCOLAS_H
#define GESTORCOLAS_H

#include<SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Cola.h"
#include "Enemigo.h"
#include "Jugador.h"

   using namespace std;
class GestorColas {
private:
 
    Cola colas[3]; // para pisos 1, 3, 5
    Enemigo* activos[3];
    float posicionesY[3];
    sf::Sound* sonidoGolpe;

public:
    GestorColas(const float* plataformasY, const sf::Texture* texturas, sf::Sound* golpe);
    void actualizar(float dt, Jugador& jugador);
    void dibujar(sf::RenderWindow& window);
};

#endif
