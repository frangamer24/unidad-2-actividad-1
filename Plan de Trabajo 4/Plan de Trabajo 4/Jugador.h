#pragma once
#ifndef JUGADOR_H
#define JUGADOR_H

#include <SFML/Graphics.hpp>

class Jugador {
private:
    sf::Sprite sprite;
    sf::Texture textura;
    sf::Vector2f velocidad;
    bool enSuelo;
    bool saltoPresionado;
    int pisoActual;
    float gravedad;
    float fuerzaSalto;
    float velocidadMovimiento;

public:
    Jugador(const std::string& rutaTextura);
    void actualizar(float dt, const float* plataformasY);
    void manejarInput();
    void dibujar(sf::RenderWindow& window);

    int getPisoActual() const;
    void setPisoActual(int piso);

    void reiniciar(int piso, float y);
    sf::FloatRect getBounds() const;
    sf::Sprite& getSprite();
};

#endif

