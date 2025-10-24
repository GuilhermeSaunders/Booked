#ifndef MOTOR_H
#define MOTOR_H

#include <thread>
#include <atomic>
using namespace std;

class Motor {
public:
    // Construtor e Destrutor (boas práticas, especialmente com threads)
    Motor();
    ~Motor();

    void iniciar();
    void parar();
    bool status() const;

private:
    atomic<bool> rodando;
    thread threadLoop;

    void loop();
};

#endif // MOTOR_H