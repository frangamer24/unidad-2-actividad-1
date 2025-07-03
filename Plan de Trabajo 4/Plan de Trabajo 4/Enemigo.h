#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <SFML/Graphics.hpp>

class Enemigo {
private:
    sf::Sprite sprite;
    float velocidad;
    int direccion; // 1 = derecha, -1 = izquierda
    float limiteIzq, limiteDer;
    float xInicio;
    int estado;    // 0 = esperando, 1 = moviendo, 2 = terminado
    bool activo;

public:
    Enemigo(const sf::Texture& tex, float x, float y, float limite1, float limite2);

    void iniciarMovimiento();
    void update(float dt);
    void draw(sf::RenderWindow& win);

    sf::FloatRect getBounds();
    bool estaActivo();
    int getEstado();
    int getDireccion();
    float getLimiteIzq();
    float getLimiteDer();
    void setEstado(int est);

    void setDireccion(int dir);
    void reiniciar(int dir);

    void setPosition(float x, float y);
    sf::Vector2f getPosition();
    sf::Sprite& getSprite();
};

#endif
