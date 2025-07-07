#include "Cola.h"

Cola::Cola() {
    frente = fin = nullptr;
}

Cola::~Cola() {
    while (!estaVacia())
        desencolar();
}

void Cola::encolar(Enemigo* e) {
    Nodo* nuevo = new Nodo(e);
    if (estaVacia()) {
        frente = fin = nuevo;
    }
    else {
        fin->siguiente = nuevo;
        fin = nuevo;
    }
}

Enemigo* Cola::desencolar() {
    if (estaVacia()) return nullptr;
    Nodo* temp = frente;
    Enemigo* e = temp->enemigo;
    frente = frente->siguiente;
    if (frente == nullptr) fin = nullptr;
    delete temp;
    return e;
}

bool Cola::estaVacia() {
    return frente == nullptr;
}

void Cola::dibujarCola(sf::RenderWindow& win, float y) {
    Nodo* actual = frente;
    float offsetX = 700; // donde dibuja la cola parada
    while (actual != nullptr) {
        actual->enemigo->setPosition(offsetX, y);
        actual->enemigo->draw(win);
        offsetX += 40;
        actual = actual->siguiente;
    }
}
Nodo* Cola::getFrente() {  // NUEVO
    return frente;
}