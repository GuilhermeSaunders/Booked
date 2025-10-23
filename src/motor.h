#ifndef MOTOR_H
#define MOTOR_H

#include <thread>
#include <atomic>

class Motor {
public:
    // Construtor e Destrutor (boas práticas, especialmente com threads)
    Motor();
    ~Motor();

    void iniciar();
    void parar();
    bool status() const;

private:
    std::atomic<bool> rodando;
    std::thread threadLoop;

    void loop(); // Método privado
};

#endif // MOTOR_H