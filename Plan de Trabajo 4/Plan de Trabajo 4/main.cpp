#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Jugador.h"
#include "GestorColas.h"
#include "GestorPilas.h"

using namespace sf;

Clock relojFinal;

int main() {
    RenderWindow window(VideoMode(800, 600), "Juego limpio con clases");
    window.setFramerateLimit(60);

    bool victoria = false, derrota = false;
    SoundBuffer bufferVictoria, bufferPerdiste;
    bufferVictoria.loadFromFile("assets/victoria.wav");
    bufferPerdiste.loadFromFile("assets/perdiste.wav");
    Sound sonidoVictoria(bufferVictoria);
    Sound sonidoPerdiste(bufferPerdiste);

    // ==== Texturas y sonidos ====
    Texture fondoTexture, puertaTexture;
    fondoTexture.loadFromFile("assets/fondo_plataformas.png");
    puertaTexture.loadFromFile("assets/puerta.png");

    // Texturas de enemigos
    Texture texAmarillo, texAzul, texRojo, texVerde;
    texAmarillo.loadFromFile("assets/shell_amarillo.png");
    texAzul.loadFromFile("assets/shell_azul.png");
    texRojo.loadFromFile("assets/shell_rojo.png");
    texVerde.loadFromFile("assets/shell_verde.png");

    Texture texturasColas[3] = { texAmarillo, texAzul, texRojo };
    Texture texturasPilas[3] = { texVerde, texAzul, texRojo };

    SoundBuffer bufferGolpe;
    bufferGolpe.loadFromFile("assets/golpe.wav");
    Sound sonidoGolpe(bufferGolpe);

    // ==== Fondo y puerta ====
    Sprite fondoSprite(fondoTexture);
    Sprite puertaSprite(puertaTexture);
    puertaSprite.setScale(1.5f, 1.5f);

    float plataformasY[7] = { 525, 450, 375, 300, 225, 150, 75 };
    puertaSprite.setPosition(160 - puertaSprite.getGlobalBounds().width / 2,
        plataformasY[6] - puertaSprite.getGlobalBounds().height);

    // ==== Instancias de clases ====
    Jugador jugador("assets/jumper.png");
    GestorColas gestorColas(plataformasY, texturasColas, &sonidoGolpe);
    GestorPilas gestorPilas(plataformasY, texturasPilas, &sonidoGolpe);

    // ==== Texto de tiempo ====
    Font font;
    font.loadFromFile("assets/Roboto.ttf");
    Text textoTiempo("", font, 24);
    textoTiempo.setFillColor(Color::White);
    textoTiempo.setPosition(20, 10);

    // ==== Control del tiempo ====
    Clock clock;
    float tiempoLimite = 30.0f;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
        }

        // === Tiempo ===
        if (!derrota && !victoria) {
            tiempoLimite -= dt;
            if (tiempoLimite <= 0) {
                derrota = true;
                sonidoPerdiste.play();
                relojFinal.restart();
            }
        }

        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << std::max(0.0f, tiempoLimite);
        textoTiempo.setString(ss.str());

        // === Movimiento y lógica ===
        if (!derrota && !victoria) {
            jugador.manejarInput();
            jugador.actualizar(dt, plataformasY);

            gestorColas.actualizar(dt, jugador);
            gestorPilas.actualizar(dt, jugador);

            if (jugador.getPisoActual() == 6 && jugador.getBounds().intersects(puertaSprite.getGlobalBounds())) {
                victoria = true;
                sonidoVictoria.play();
                relojFinal.restart();
            }
        }

        // === Dibujar ===
        window.clear();
        window.draw(fondoSprite);
        window.draw(puertaSprite);
        gestorColas.dibujar(window);
        gestorPilas.dibujar(window);
        jugador.dibujar(window);
        window.draw(textoTiempo);

        if (victoria) {
            Text msg("¡Ganaste!", font, 48);
            msg.setFillColor(Color::Green);
            msg.setPosition(400 - msg.getGlobalBounds().width / 2, 240);
            window.draw(msg);
        }
        else if (derrota) {
            Text msg("¡Perdiste!", font, 48);
            msg.setFillColor(Color::Red);
            msg.setPosition(400 - msg.getGlobalBounds().width / 2, 240);
            window.draw(msg);
        }

        window.display();

        // Cierre automático después de mostrar mensaje final
        if ((victoria || derrota) && relojFinal.getElapsedTime().asSeconds() > 7.f) {
            window.close();
        }
    }

    return 0;
}
