#include "Enemigo.h"

Enemigo::Enemigo(const sf::Texture& tex, float x, float y, float lim1, float lim2) {
    sprite.setTexture(tex);
    sprite.setPosition(x, y);
    sprite.setScale(1.0f, 1.0f);
    velocidad = 200.0f;
    limiteIzq = std::min(lim1, lim2);
    limiteDer = std::max(lim1, lim2);
    direccion = (x > (limiteIzq + limiteDer) / 2) ? -1 : 1;
    xInicio = x;
    estado = 0;
    activo = false;
}

void Enemigo::iniciarMovimiento() {
    if (estado == 0) {
        estado = 1;
        activo = true;
    }
}

void Enemigo::update(float dt) {
    if (estado == 1) {
        sprite.move(velocidad * direccion * dt, 0);
        float x = sprite.getPosition().x;
        float ancho = sprite.getGlobalBounds().width;

        if ((direccion == 1 && x >= limiteDer) ||
            (direccion == -1 && x + ancho <= limiteIzq)) {
            estado = 2;
            activo = false;
        }
    }
}

void Enemigo::draw(sf::RenderWindow& win) {
    win.draw(sprite);
}

sf::FloatRect Enemigo::getBounds() {
    return sprite.getGlobalBounds();
}

bool Enemigo::estaActivo() {
    return activo;
}

int Enemigo::getEstado() {
    return estado;
}

void Enemigo::setEstado(int est) {
    estado = est;
    if (estado == 1) activo = true;
}

void Enemigo::setDireccion(int dir) {
    direccion = dir;
}

void Enemigo::reiniciar(int dir) {
    direccion = dir;
    float y = sprite.getPosition().y;
    float ancho = sprite.getGlobalBounds().width;

    if (direccion == 1)
        sprite.setPosition(limiteIzq - ancho, y);  // fuera del borde izquierdo
    else
        sprite.setPosition(limiteDer + ancho, y);  // fuera del borde derecho

    estado = 0;
    activo = false;
}

sf::Sprite& Enemigo::getSprite() {
    return sprite;
}

void Enemigo::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Enemigo::getPosition() {
    return sprite.getPosition();
}
int Enemigo::getDireccion() {
    return direccion;
}
float Enemigo::getLimiteIzq() {
    return limiteIzq;
}
float Enemigo::getLimiteDer() {
    return limiteDer;
}