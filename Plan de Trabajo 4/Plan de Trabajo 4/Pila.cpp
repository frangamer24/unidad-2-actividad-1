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