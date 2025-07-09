#include "GestorColas.h"

GestorColas::GestorColas(const float* plataformasY, const sf::Texture* texturas, sf::Sound* golpe) {
    sonidoGolpe = golpe;
    for (int i = 0; i < 3; i++) {
        int piso = 1 + i * 2;
        posicionesY[i] = plataformasY[piso] - 30;
        for (int j = 0; j < 3; j++) {
            Enemigo* e = new Enemigo(texturas[j], 700 + j * 30, posicionesY[i], 0, 800);
            e->setDireccion(-1);
            colas[i].encolar(e);
        }
        activos[i] = colas[i].desencolar();
        activos[i]->iniciarMovimiento();
    }
}

void GestorColas::actualizar(float dt, Jugador& jugador) {
    for (int i = 0; i < 3; i++) {
        int piso = 1 + i * 2;
        if (activos[i]) {
            activos[i]->update(dt);
            if (activos[i]->getEstado() == 2) {
                activos[i]->setPosition(800, posicionesY[i]);
                activos[i]->setEstado(0);
                colas[i].encolar(activos[i]);
                activos[i] = colas[i].desencolar();
                activos[i]->iniciarMovimiento();
            }
            if (jugador.getPisoActual() == piso &&
                jugador.getBounds().intersects(activos[i]->getBounds())) {
                if (sonidoGolpe) sonidoGolpe->play();
                jugador.reiniciar(0, posicionesY[0] + 30);
            }
        }
    }
}

void GestorColas::dibujar(sf::RenderWindow& window) {
    for (int i = 0; i < 3; i++) {
        Nodo* actual = colas[i].getFrente();
        int offsetX = 735;
        while (actual != nullptr) {
            actual->enemigo->setPosition(offsetX, posicionesY[i]);
            actual->enemigo->draw(window);
            offsetX += 30;
            actual = actual->siguiente;
        }
        if (activos[i]) activos[i]->draw(window);
    }
}