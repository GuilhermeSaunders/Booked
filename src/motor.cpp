#include "motor.h"
#include <iostream>
#include <chrono>

// Construtor: inicializa a flag 'rodando'
Motor::Motor() : rodando(false) {
    // Construtor pode ficar vazio se a inicialização já foi feita na lista
}

// Destrutor: garante que a thread pare ao destruir o objeto
Motor::~Motor() {
    parar();
}

void Motor::iniciar() {
    // Usa .load() para ler um std::atomic (boa prática)
    if (rodando.load()) return;

    rodando = true; // Usa = (store) para escrever em um std::atomic
    threadLoop = std::thread(&Motor::loop, this);
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

// --- Métodos Privados ---

void Motor::loop() {
    while (rodando.load()) {
        // Adiciona std:: para cout, endl, this_thread, e chrono
        std::cout << "[Motor] Rodando..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "[Motor] Parado." << std::endl;
}