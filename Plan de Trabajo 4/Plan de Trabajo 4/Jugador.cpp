// Jugador.cpp
#include "Jugador.h"
#include <iostream>

Jugador::Jugador(const std::string& rutaTextura) {
    if (!textura.loadFromFile(rutaTextura)) {
        std::cerr << "Error al cargar la textura del jugador\n";
    }
    sprite.setTexture(textura);
    sprite.setPosition(380, 525);
    velocidad = { 0.f, 0.f };
    enSuelo = true;
    saltoPresionado = false;
    pisoActual = 0;
    gravedad = 700.f;
    fuerzaSalto = 200.f;
    velocidadMovimiento = 100.f;
}

void Jugador::actualizar(float dt, const float* plataformasY) {
    velocidad.y += gravedad * dt;
    sprite.move(velocidad * dt);

    sf::FloatRect bounds = sprite.getGlobalBounds();
    float jugadorY = bounds.top + bounds.height;

    if (jugadorY >= plataformasY[pisoActual]) {
        sprite.setPosition(sprite.getPosition().x, plataformasY[pisoActual] - bounds.height);
        velocidad.y = 0;
        enSuelo = true;
    }

    if (!enSuelo && velocidad.y < 0 && jugadorY < plataformasY[pisoActual] - 10 && pisoActual < 6) {
        pisoActual++;
    }
}

void Jugador::manejarInput() {
    velocidad.x = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) velocidad.x = -velocidadMovimiento;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) velocidad.x = velocidadMovimiento;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (enSuelo && !saltoPresionado) {
            velocidad.y = -fuerzaSalto;
            enSuelo = false;
            saltoPresionado = true;
        }
    }
    else saltoPresionado = false;
}

void Jugador::dibujar(sf::RenderWindow& window) {
    window.draw(sprite);
}

int Jugador::getPisoActual() const {
    return pisoActual;
}

void Jugador::setPisoActual(int piso) {
    pisoActual = piso;
}

void Jugador::reiniciar(int piso, float y) {
    pisoActual = piso;
    sprite.setPosition(380, y);
    velocidad.y = 0;
}

sf::FloatRect Jugador::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Sprite& Jugador::getSprite() {
    return sprite;
}
