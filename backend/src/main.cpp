#include "httplib.h"
#include <iostream>
#include "motor.h"

using namespace std;

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

    cout << "Servidor rodando em http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}