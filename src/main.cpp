#include "httplib.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

class Motor {
public:
    void iniciar() {
        if (rodando) return;
        rodando = true;
        threadLoop = std::thread(&Motor::loop, this);
    }

    void parar() {
        rodando = false;
        if (threadLoop.joinable())
            threadLoop.join();
    }

    bool status() const {
        return rodando;
    }

private:
    std::atomic<bool> rodando{false};
    std::thread threadLoop;

    void loop() {
        while (rodando) {
            std::cout << "[Motor] Rodando..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout << "[Motor] Parado." << std::endl;
    }
};

int main() {
    httplib::Server svr;
    Motor motor;

    // Habilita CORS
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    svr.Options(R"(.*)", [](const httplib::Request&, httplib::Response& res) {
        res.status = 200;
    });

    svr.Post("/api/start", [&](const httplib::Request&, httplib::Response& res) {
        motor.iniciar();
        res.set_content("Motor iniciado", "text/plain");
    });

    svr.Post("/api/stop", [&](const httplib::Request&, httplib::Response& res) {
        motor.parar();
        res.set_content("Motor parado", "text/plain");
    });

    svr.Get("/api/status", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(motor.status() ? "Rodando" : "Parado", "text/plain");
    });

    //INCLUIR NOSSAS ROTAS AQUI!!!!!!!!!!!!!!!!!!!!!!!!

    std::cout << "Servidor rodando em http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}