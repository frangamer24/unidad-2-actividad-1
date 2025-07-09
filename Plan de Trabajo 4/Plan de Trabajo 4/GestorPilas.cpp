#include "GestorPilas.h"
#include <iostream>

GestorPilas::GestorPilas(const float* plataformas, const sf::Texture* texturas, sf::Sound* golpe)
    : plataformasY(plataformas), sonidoGolpe(golpe) {

    // Configuración pisos pares: 2, 4, 6
    pisos[0] = { {}, {}, nullptr, true, plataformas[2] - 30 };
    pisos[1] = { {}, {}, nullptr, false, plataformas[4] - 30 };
    pisos[2] = { {}, {}, nullptr, true, plataformas[6] - 30 };

    // Piso 2: inicia desde izquierda
    pisos[0].izquierda.push(new Enemigo(texturas[0], 50, pisos[0].yPiso, 50, 750));
    pisos[0].izquierda.push(new Enemigo(texturas[1], 80, pisos[0].yPiso, 50, 750));
    pisos[0].izquierda.push(new Enemigo(texturas[2], 110, pisos[0].yPiso, 50, 750));

    // Piso 4: inicia desde derecha
    pisos[1].derecha.push(new Enemigo(texturas[0], 750, pisos[1].yPiso, 50, 750));
    pisos[1].derecha.push(new Enemigo(texturas[1], 720, pisos[1].yPiso, 50, 750));
    pisos[1].derecha.push(new Enemigo(texturas[2], 690, pisos[1].yPiso, 50, 750));

    // Piso 6: inicia desde izquierda
    pisos[2].izquierda.push(new Enemigo(texturas[0], 50, pisos[2].yPiso, 50, 750));
    pisos[2].izquierda.push(new Enemigo(texturas[1], 80, pisos[2].yPiso, 50, 750));
    pisos[2].izquierda.push(new Enemigo(texturas[2], 110, pisos[2].yPiso, 50, 750));
}

void GestorPilas::actualizar(float dt, Jugador& jugador) {
    for (int i = 0; i < 3; i++) {
        int piso = 2 + i * 2;

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

            // Colisión con jugador
            if (jugador.getPisoActual() == piso && jugador.getBounds().intersects(pisos[i].moviendo->getBounds())) {
                if (sonidoGolpe) sonidoGolpe->play();
                jugador.reiniciar(0, plataformasY[0] - 40);
            }

            // Cuando termina su recorrido
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
}

void GestorPilas::dibujar(sf::RenderWindow& window) {
    for (int i = 0; i < 3; i++) {
        Nodo* actual;
        int offsetX;

        // Dibujar pila izquierda
        actual = pisos[i].izquierda.getTope();
        offsetX = 50;
        while (actual) {
            actual->enemigo->setPosition(offsetX, pisos[i].yPiso);
            actual->enemigo->draw(window);
            offsetX += 30;
            actual = actual->siguiente;
        }

        // Dibujar pila derecha
        actual = pisos[i].derecha.getTope();
        offsetX = 750;
        while (actual) {
            actual->enemigo->setPosition(offsetX, pisos[i].yPiso);
            actual->enemigo->draw(window);
            offsetX -= 30;
            actual = actual->siguiente;
        }

        if (pisos[i].moviendo) {
            pisos[i].moviendo->draw(window);
        }
    }
}
