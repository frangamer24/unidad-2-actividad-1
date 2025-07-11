#include "Pila.h"

Pila::Pila() {
    tope = nullptr;
}

Pila::~Pila() {
    while (!estaVacia())
        pop();
}

void Pila::push(Enemigo* e) {
    Nodo* nuevo = new Nodo(e);
    nuevo->siguiente = tope;
    tope = nuevo;
}

Enemigo* Pila::pop() {
    if (estaVacia()) return nullptr;
    Nodo* temp = tope;
    Enemigo* e = temp->enemigo;
    tope = tope->siguiente;
    delete temp;
    return e;
}

bool Pila::estaVacia() {
    return tope == nullptr;
}

Nodo* Pila::getTope() {
    return tope;
}


void Pila::dibujar(sf::RenderWindow& window, float y, bool izquierda) {
    Nodo* actual = tope;
    int offsetX = izquierda ? 50 : 750;

    while (actual != nullptr) {
        actual->enemigo->setPosition(offsetX, y);
        actual->enemigo->draw(window);
        offsetX += (izquierda ? 30 : -30);
        actual = actual->siguiente;
    }
}
