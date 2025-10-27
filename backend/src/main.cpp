#include "httplib.h"
#include <iostream>
#include "motor.h"
#include "../Product/Product.h"
#include "../Product/Book.h"
#include "../Product/Movie.h"
#include "json.hpp"
#include <memory>

using namespace std;
using nlohmann::json;

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
    
    
    // Filtrar Produto
    svr.Get("/api/produto/filtro", [&](const httplib::Request& req, httplib::Response& res) {
        std::string categoria;
        if (req.has_param("categoria")) {
            categoria = req.get_param_value("categoria");
        }
        // =========================
        // [PSEUDOCÓDIGO - BANCO]
        // Aqui você consulta o banco:
        // auto resultados = banco.filtrarProdutosPorCategoria(categoria);
        // json j_resposta = resultados;
        // =========================
        json j_resposta;
        // Por enquanto, enquanto não tem banco:
        // você pode enviar um array vazio ou algum placeholder indicando "nenhum produto"
        j_resposta = json::array();
        res.set_content(j_resposta.dump(4), "application/json");
    });

    // Cadastrar Produto
    svr.Post("/api/produto/novo", [&](const httplib::Request& req, httplib::Response& res) {
        json data = json::parse(req.body);

        // =========================
        // Atributos gerais
        // =========================
        string tipo = data.value("tipo", "");
        string nome = data.value("nome", "");
        string descricao = data.value("descricao", "");
        double preco = data.value("preco", 0.0);
        string categoria = data.value("categoria", "");

        // =========================
        // Criação dinâmica do objeto Produto (modelo comentado)
        // =========================
        Product* produto = nullptr;

        /*
        if (tipo == "livro") {
            produto = new Book(
                data.value("name", ""),
                data.value("description", ""),
                data.value("author", ""),
                data.value("genre", ""),
                data.value("idiom", ""),
                data.value("rentValue", 0),
                data.value("recommendedAge", ""),
                data.value("id", 0)
            );
        } else if (tipo == "filme") {
            produto = new Movie(
                data.value("name", ""),
                data.value("description", ""),
                data.value("director", ""),
                data.value("mainActors", ""),
                data.value("genre", ""),
                data.value("idiom", ""),
                data.value("rentValue", 0),
                data.value("recommendedAge", ""),
                data.value("id", 0)
            );
        } else if (tipo == "jogo_tabuleiro") {
            produto = new JogoTabuleiro(nome, descricao, preco, categoria,
                                        data.value("numero_jogadores",0),
                                        data.value("idade_minima",0));
        } else if (tipo == "jogo_videogame") {
            produto = new JogoVideogame(nome, descricao, preco, categoria,
                                        data.value("plataforma",""),
                                        data.value("classificacao",""));
        }
        */

        // =========================
        // [PSEUDOCÓDIGO - BANCO]
        // bool sucesso = banco.cadastrarProduto(usuarioLogado, produto);
        // json j_resposta = { {"status", sucesso ? "ok" : "erro"} };
        // =========================

        // Mock de resposta enquanto não tem banco
        json j_resposta = {
            {"status", "ok"},
            {"mensagem", "Produto cadastrado com sucesso (mock)"},
            {"tipo", tipo},
            {"nome", nome},
            {"categoria", categoria}
        };

        res.set_content(j_resposta.dump(4), "application/json");

        // Libera memória (não faz nada enquanto produto é nullptr)
        delete produto;
    });


    cout << "Servidor rodando em http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}