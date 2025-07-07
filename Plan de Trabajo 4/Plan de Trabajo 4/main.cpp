#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Enemigo.h"
#include "Cola.h"
#include "Pila.h"
#include "Nodo.h"

using namespace sf;

const float GRAVEDAD = 700.0f;
const float VELOCIDAD = 100.0f;
const float FUERZA_SALTO = 200.0f;

int main() {
    RenderWindow window(VideoMode(800, 600), "Juego fusionado con colas y pilas");
    window.setFramerateLimit(60);

    // ==== Texturas y sprites principales ====
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
    puertaSprite.setPosition(
        160 - puertaSprite.getGlobalBounds().width / 2,
        plataformasY[6] - puertaSprite.getGlobalBounds().height
    );

    // ==== Música y sonidos ====
    SoundBuffer bufferVictory, bufferPerdiste, bufferGolpe, bufferSalto;
    bufferVictory.loadFromFile("assets/victoria.wav");
    bufferPerdiste.loadFromFile("assets/perdiste.wav");
    bufferGolpe.loadFromFile("assets/golpe.wav");
    bufferSalto.loadFromFile("assets/salto.wav");

    Sound sonidoVictory(bufferVictory);
    Sound sonidoPerdiste(bufferPerdiste);
    Sound sonidoGolpe(bufferGolpe);
    Sound sonidoSalto(bufferSalto);
    sonidoSalto.setVolume(50.f);

    // ==== Textos ====
    Font font;
    font.loadFromFile("assets/Roboto.ttf");
    Text textoTiempo("", font, 24);
    textoTiempo.setFillColor(Color::White);
    textoTiempo.setPosition(20, 10);

    Text mensajeFinal("", font, 36);
    mensajeFinal.setFillColor(Color::Cyan);
    mensajeFinal.setPosition(200, 250);

    // ==== Variables de jugador ====
    Vector2f velocidad(0.0f, 0.0f);
    int pisoActual = 0;
    bool enSuelo = true, saltoPresionado = false;

    // ==== Tiempo y estados ====
    Clock clock, victoriaClock, derrotaClock;
    bool victoria = false, derrota = false, sonidoReproducido = false;
    float tiempoLimite = 30.0f;

    // ==== Texturas de enemigos ====
    Texture texVerde, texAzul, texRojo, texAmarillo;
    texVerde.loadFromFile("assets/shell_verde.png");
    texAzul.loadFromFile("assets/shell_azul.png");
    texRojo.loadFromFile("assets/shell_rojo.png");
    texAmarillo.loadFromFile("assets/shell_amarillo.png");

    Texture texturas[3] = { texAmarillo, texAzul, texRojo };

    // ================== COLAS EN PISOS IMPARES ===================
    Cola cola1, cola3, cola5;
    Enemigo* enemigoActivo1, * enemigoActivo3, * enemigoActivo5;
    float yPiso1 = plataformasY[1] - 30;
    float yPiso3 = plataformasY[3] - 30;
    float yPiso5 = plataformasY[5] - 30;

    for (int i = 0; i < 3; i++) {
        cola1.encolar(new Enemigo(texturas[i], 700 + i * 30, yPiso1, 0, 800));
        cola3.encolar(new Enemigo(texturas[i], 700 + i * 30, yPiso3, 0, 800));
        cola5.encolar(new Enemigo(texturas[i], 700 + i * 30, yPiso5, 0, 800));
    }
    enemigoActivo1 = cola1.desencolar();
    enemigoActivo1->setDireccion(-1);
    enemigoActivo1->iniciarMovimiento();

    enemigoActivo3 = cola3.desencolar();
    enemigoActivo3->setDireccion(-1);
    enemigoActivo3->iniciarMovimiento();

    enemigoActivo5 = cola5.desencolar();
    enemigoActivo5->setDireccion(-1);
    enemigoActivo5->iniciarMovimiento();

    // ================== PILAS EN PISOS PARES ===================
    struct PisoPilas {
        Pila izquierda;
        Pila derecha;
        Enemigo* moviendo = nullptr;
        bool haciaDerecha;
        float yPiso;
    };

    PisoPilas pisos[3];
    pisos[0] = { {}, {}, nullptr, true, plataformasY[2] - 30 };
    pisos[1] = { {}, {}, nullptr, false, plataformasY[4] - 30 };
    pisos[2] = { {}, {}, nullptr, true, plataformasY[6] - 30 };

    pisos[0].izquierda.push(new Enemigo(texVerde, 50, pisos[0].yPiso, 50, 750));
    pisos[0].izquierda.push(new Enemigo(texAzul, 80, pisos[0].yPiso, 50, 750));
    pisos[0].izquierda.push(new Enemigo(texRojo, 110, pisos[0].yPiso, 50, 750));

    pisos[1].derecha.push(new Enemigo(texAmarillo, 750, pisos[1].yPiso, 50, 750));
    pisos[1].derecha.push(new Enemigo(texVerde, 720, pisos[1].yPiso, 50, 750));
    pisos[1].derecha.push(new Enemigo(texAzul, 690, pisos[1].yPiso, 50, 750));

    pisos[2].izquierda.push(new Enemigo(texRojo, 50, pisos[2].yPiso, 50, 750));
    pisos[2].izquierda.push(new Enemigo(texAmarillo, 80, pisos[2].yPiso, 50, 750));
    pisos[2].izquierda.push(new Enemigo(texVerde, 110, pisos[2].yPiso, 50, 750));

    // ================== GAME LOOP ===================
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // ================== FIN DEL JUEGO ===================
        if (victoria || derrota) {
            if (!sonidoReproducido) {
                (victoria ? sonidoVictory : sonidoPerdiste).play();
                sonidoReproducido = true;
            }
            if ((victoria ? victoriaClock : derrotaClock).getElapsedTime().asSeconds() >= 6.0f)
                window.close();
            mensajeFinal.setString(victoria ? "¡Has ganado el juego!" : "¡Has perdido el juego!");
            window.clear(); window.draw(fondoSprite);
            window.draw(puertaSprite); window.draw(jugadorSprite);
            window.draw(mensajeFinal); window.display();
            continue;
        }

        // ================== TIEMPO ===================
        tiempoLimite -= dt;
        if (tiempoLimite <= 0) {
            derrota = true;
            derrotaClock.restart();
        }
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << std::max(0.0f, tiempoLimite);
        textoTiempo.setString(ss.str());

        // ================== MOVIMIENTO JUGADOR ===================
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
            velocidad.y = 0; enSuelo = true;
        }

        if (!enSuelo && velocidad.y < 0 && jugadorSprite.getPosition().y + bounds.height < plataformasY[pisoActual] - 10 && pisoActual < 6)
            pisoActual++;

        // ================== CHEQUEO PUERTA ===================
        if (pisoActual == 6 && jugadorSprite.getGlobalBounds().intersects(puertaSprite.getGlobalBounds())) {
            victoria = true;
            victoriaClock.restart();
        }

        // ================== COLAS IMPARES ===================
        auto moverCola = [&](Cola& cola, Enemigo*& activo, float yPiso, int piso) {
            if (activo != nullptr) {
                activo->update(dt);
                if (activo->getEstado() == 2) {
                    activo->setPosition(800, yPiso);
                    activo->setEstado(0);
                    cola.encolar(activo);
                    activo = cola.desencolar();
                    activo->iniciarMovimiento();
                }
                if (pisoActual == piso && jugadorSprite.getGlobalBounds().intersects(activo->getBounds())) {
                    sonidoGolpe.play();
                    pisoActual = 0;
                    jugadorSprite.setPosition(380, plataformasY[0] - jugadorSprite.getGlobalBounds().height);
                    velocidad.y = 0;
                }
            }
            };
        moverCola(cola1, enemigoActivo1, yPiso1, 1);
        moverCola(cola3, enemigoActivo3, yPiso3, 3);
        moverCola(cola5, enemigoActivo5, yPiso5, 5);

        // ================== PILAS PARES ===================
        for (int i = 0; i < 3; i++) {
            if (!pisos[i].moviendo) {
                if (pisos[i].haciaDerecha && !pisos[i].izquierda.estaVacia()) {
                    pisos[i].moviendo = pisos[i].izquierda.pop();
                    pisos[i].moviendo->setDireccion(1);
                    pisos[i].moviendo->iniciarMovimiento();
                }
                else if (!pisos[i].haciaDerecha && !pisos[i].derecha.estaVacia()) {
                    pisos[i].moviendo = pisos[i].derecha.pop();
                    pisos[i].moviendo->setDireccion(-1);
                    pisos[i].moviendo->iniciarMovimiento();
                }
            }
            else {
                pisos[i].moviendo->update(dt);
                if ((pisoActual == 2 && i == 0) ||
                    (pisoActual == 4 && i == 1) ||
                    (pisoActual == 6 && i == 2)) {
                    if (jugadorSprite.getGlobalBounds().intersects(pisos[i].moviendo->getBounds())) {
                        sonidoGolpe.play();
                        pisoActual = 0;
                        jugadorSprite.setPosition(380, plataformasY[0] - jugadorSprite.getGlobalBounds().height);
                        velocidad.y = 0;
                    }
                }
                if (pisos[i].haciaDerecha && pisos[i].moviendo->getPosition().x >= 750) {
                    pisos[i].moviendo->setEstado(0);
                    pisos[i].derecha.push(pisos[i].moviendo);
                    pisos[i].moviendo = nullptr;
                    if (pisos[i].izquierda.estaVacia()) pisos[i].haciaDerecha = false;
                }
                else if (!pisos[i].haciaDerecha && pisos[i].moviendo->getPosition().x <= 50) {
                    pisos[i].moviendo->setEstado(0);
                    pisos[i].izquierda.push(pisos[i].moviendo);
                    pisos[i].moviendo = nullptr;
                    if (pisos[i].derecha.estaVacia()) pisos[i].haciaDerecha = true;
                }
            }
        }

        // ================== DIBUJO ===================
        window.clear();
        window.draw(fondoSprite);
        window.draw(puertaSprite);

        auto dibujarCola = [&](Cola& cola, float yPiso) {
            Nodo* actual = cola.getFrente();
            int offsetX = 735;
            while (actual != nullptr) {
                actual->enemigo->setPosition(offsetX, yPiso);
                actual->enemigo->draw(window);
                offsetX += 30;
                actual = actual->siguiente;
            }
            };
        dibujarCola(cola1, yPiso1);
        dibujarCola(cola3, yPiso3);
        dibujarCola(cola5, yPiso5);

        if (enemigoActivo1) enemigoActivo1->draw(window);
        if (enemigoActivo3) enemigoActivo3->draw(window);
        if (enemigoActivo5) enemigoActivo5->draw(window);

        for (int i = 0; i < 3; i++) {
            Nodo* actual;
            int offsetX;

            actual = pisos[i].izquierda.getTope();
            offsetX = 50;
            while (actual != nullptr) {
                actual->enemigo->setPosition(offsetX, pisos[i].yPiso);
                actual->enemigo->draw(window);
                offsetX += 30;
                actual = actual->siguiente;
            }

            actual = pisos[i].derecha.getTope();
            offsetX = 750;
            while (actual != nullptr) {
                actual->enemigo->setPosition(offsetX, pisos[i].yPiso);
                actual->enemigo->draw(window);
                offsetX -= 30;
                actual = actual->siguiente;
            }

            if (pisos[i].moviendo)
                pisos[i].moviendo->draw(window);
        }

        window.draw(jugadorSprite);
        window.draw(textoTiempo);
        window.display();
    }

    return 0;
}