#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Enemigo.h"

using namespace sf;

const float GRAVEDAD = 700.0f;
const float VELOCIDAD = 100.0f;
const float FUERZA_SALTO = 200.0f;

int main() {
    RenderWindow window(VideoMode(800, 600), "Juego con enemigos al borde fijo");
    window.setFramerateLimit(60);

    Texture fondoTexture, jugadorTexture, puertaTexture;
    fondoTexture.loadFromFile("assets/fondo_plataformas.png");
    jugadorTexture.loadFromFile("assets/jumper.png");
    puertaTexture.loadFromFile("assets/puerta.png");

    Sprite fondoSprite(fondoTexture);
    Sprite jugadorSprite(jugadorTexture);
    jugadorSprite.setPosition(380, 525);

    Sprite puertaSprite(puertaTexture);
    puertaSprite.setScale(1.5f, 1.5f);

    float plataformasY[7] = { 525, 450, 375, 300, 225, 150, 75 };
    puertaSprite.setPosition(100, plataformasY[6] - puertaSprite.getGlobalBounds().height);

    Vector2f velocidad(0.0f, 0.0f);
    int pisoActual = 0;
    bool enSuelo = true, saltoPresionado = false;

    Clock clock, victoriaClock, derrotaClock;
    bool victoria = false, derrota = false;
    bool sonidoReproducido = false;

    float tiempoLimite = 30.0f;

    Font font;
    font.loadFromFile("assets/Roboto.ttf");
    Text textoTiempo("", font, 24);
    textoTiempo.setFillColor(Color::White);
    textoTiempo.setPosition(20, 10);

    Text mensajeFinal("", font, 36);
    mensajeFinal.setFillColor(Color::Cyan);
    mensajeFinal.setPosition(200, 250);

    // Cargar sonidos
    SoundBuffer bufferVictory, bufferPerdiste, bufferGolpe, bufferSalto;
    Sound sonidoVictory, sonidoPerdiste, sonidoGolpe, sonidoSalto;

    bufferVictory.loadFromFile("assets/victoria.wav");
    bufferPerdiste.loadFromFile("assets/perdiste.wav");
    bufferGolpe.loadFromFile("assets/golpe.wav");
    bufferSalto.loadFromFile("assets/salto.wav");

    sonidoVictory.setBuffer(bufferVictory);
    sonidoPerdiste.setBuffer(bufferPerdiste);
    sonidoGolpe.setBuffer(bufferGolpe);
    sonidoSalto.setBuffer(bufferSalto);

    sonidoSalto.setVolume(50.f);

    // Cargar texturas enemigos
    Texture texturas[7];
    texturas[1].loadFromFile("assets/shell_amarillo.png");
    texturas[2].loadFromFile("assets/shell_azul.png");
    texturas[3].loadFromFile("assets/shell_rojo.png");
    texturas[4].loadFromFile("assets/shell_verde.png");
    texturas[5].loadFromFile("assets/shell_rojo.png");
    texturas[6].loadFromFile("assets/shell_azul.png");

    Enemigo* enemigos[7] = { nullptr };

    for (int piso = 1; piso < 7; piso++) {
        float y = plataformasY[piso] - 30;
        if (piso % 2 == 0) {
            enemigos[piso] = new Enemigo(texturas[piso], 0, y, 0, 800);
            enemigos[piso]->setDireccion(1);
        }
        else {
            enemigos[piso] = new Enemigo(texturas[piso], 800, y, 0, 800);
            enemigos[piso]->setDireccion(-1);
        }
        enemigos[piso]->iniciarMovimiento();
    }

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // Mensaje final
        if (victoria || derrota) {
            if (!sonidoReproducido) {
                if (victoria)
                    sonidoVictory.play();
                else
                    sonidoPerdiste.play();
                sonidoReproducido = true;
            }

            if ((victoria ? victoriaClock : derrotaClock).getElapsedTime().asSeconds() >= 6.0f)
                window.close();

            mensajeFinal.setString(victoria ? "¡Has ganado el juego!" : "¡Has perdido el juego!");
            window.clear();
            window.draw(fondoSprite);
            window.draw(puertaSprite);
            window.draw(jugadorSprite);
            window.draw(mensajeFinal);
            window.display();
            continue;
        }

        tiempoLimite -= dt;
        if (tiempoLimite <= 0) {
            derrota = true;
            derrotaClock.restart();
        }

        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << std::max(0.0f, tiempoLimite);
        textoTiempo.setString(ss.str());

        // Movimiento jugador
        velocidad.x = 0;
        if (Keyboard::isKeyPressed(Keyboard::A)) velocidad.x = -VELOCIDAD;
        if (Keyboard::isKeyPressed(Keyboard::D)) velocidad.x = VELOCIDAD;
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            if (enSuelo && !saltoPresionado) {
                velocidad.y = -FUERZA_SALTO;
                enSuelo = false;
                saltoPresionado = true;
                sonidoSalto.play();
            }
        }
        else saltoPresionado = false;

        velocidad.y += GRAVEDAD * dt;
        jugadorSprite.move(velocidad * dt);

        FloatRect bounds = jugadorSprite.getGlobalBounds();
        float jugadorY = bounds.top + bounds.height;

        if (jugadorY >= plataformasY[pisoActual]) {
            jugadorSprite.setPosition(jugadorSprite.getPosition().x, plataformasY[pisoActual] - bounds.height);
            velocidad.y = 0;
            enSuelo = true;
        }

        if (!enSuelo && velocidad.y < 0 && jugadorSprite.getPosition().y + bounds.height < plataformasY[pisoActual] - 10 && pisoActual < 6) {
            pisoActual++;
        }

        // Enemigos
        for (int p = 1; p < 7; p++) {
            enemigos[p]->update(dt);

            if (enemigos[p]->getEstado() == 2) {
                float y = plataformasY[p] - 30;
                if (enemigos[p]->getDireccion() == 1)
                    enemigos[p]->setPosition(0, y);
                else
                    enemigos[p]->setPosition(800, y);
                enemigos[p]->setEstado(0);
                enemigos[p]->iniciarMovimiento();
            }

            if (pisoActual == p && jugadorSprite.getGlobalBounds().intersects(enemigos[p]->getBounds())) {
                sonidoGolpe.play();
                pisoActual = 0;
                jugadorSprite.setPosition(380, plataformasY[0] - jugadorSprite.getGlobalBounds().height);
                velocidad.y = 0;
            }
        }

        if (pisoActual == 6 && jugadorSprite.getGlobalBounds().intersects(puertaSprite.getGlobalBounds())) {
            victoria = true;
            victoriaClock.restart();
        }

        window.clear();
        window.draw(fondoSprite);
        window.draw(puertaSprite);
        for (int p = 1; p < 7; p++)
            enemigos[p]->draw(window);
        window.draw(jugadorSprite);
        window.draw(textoTiempo);
        window.display();
    }

    for (int p = 1; p < 7; p++)
        delete enemigos[p];

    return 0;
}
