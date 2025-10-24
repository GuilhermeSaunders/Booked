#include "motor.h"
#include <iostream>
#include <chrono>
using namespace std;

// Construtor: inicializa a flag 'rodando'
Motor::Motor() : rodando(false) {
    // Construtor pode ficar vazio se a inicialização já foi feita na lista
}

// Destrutor: garante que a thread pare ao destruir o objeto
Motor::~Motor() {
    parar();
}

void Motor::iniciar() {
    // Usa .load() para ler um atomic (boa prática)
    if (rodando.load()) return;

    rodando = true; // Usa = (store) para escrever em um atomic
    threadLoop = thread(&Motor::loop, this);
}

void Motor::parar() {
    rodando = false;
    if (threadLoop.joinable()) {
        threadLoop.join();
    }
}

bool Motor::status() const {
    return rodando.load(); // Usa .load() para ler
}



void Motor::loop() {
    while (rodando.load()) {
        // Adiciona para cout, endl, this_thread, e chrono
        cout << "[Motor] Rodando..." <<endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "[Motor] Parado." <<endl;
}