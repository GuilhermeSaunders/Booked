#include "httplib.h"      // Biblioteca do Servidor Web
#include "json (2).hpp"     // Biblioteca JSON (nlohmann/json)
#include <iostream>
#include <string>
#include <vector>
#include <optional>     // Para C++17+
#include <memory>       // Para std::shared_ptr

// --- Inclusão dos Seus Headers ---
#include "motor.h"
#include "../BusinessRules/login.h"
#include "../BusinessRules/validate.h"
#include "../Customer/Account.h"
#include "../Customer/user.h"
#include "../Product/Product.h"
#include "../Product/Movie.h"
#include "../Product/Book.h"
#include "../BusinessRules/rental.h"

using namespace std;
using nlohmann::json;

// =======================================================================
// === SERIALIZADORES JSON (DE/PARA JSON) ===
// (Código da resposta anterior, mantido por completude)
// =======================================================================

inline void to_json(json& j, const User& u) {
    j = json{
        {"cpf", u.getCpf()},
        {"fullname", u.getFullname()},
        {"email", u.getEmail()},
        {"birthday", u.getBirthday()}
    };
}

inline void to_json(json& j, const Account& a) {
    j = json{
        {"id", a.getId()},
        {"username", a.getUsername()},
        {"user", a.getUser()} 
    };
}

inline void to_json(json& j, const Rental& r) {
    j = json{
        {"transactionId", r.getTransactionId()},
        {"duration", r.getDuration()},
        {"lender", r.getLender()},
        {"borrower", r.getBorrower()},
        {"dailyRate", r.getDailyRate()},
        {"startDate", r.getStartDate()},
        {"status", r.getStatus()},
        {"productType", r.getProductType()},
    };
}

inline void serialize_product_base(json& j, const Product& p) {
    j["id"] = p.getId();
    j["name"] = p.getName();
    j["description"] = p.getDescription();
    j["rentValue"] = p.getRentValue();
    j["consideredValue"] = p.getConsideredValue();
    j["owner"] = p.getOwner();
    j["genre"] = p.getGenre();
    j["idiom"] = p.getIdiom();
    j["recommendedAge"] = p.getRecommendedAge();
    j["ownersConsiderations"] = p.getOwnersConsiderations();
    j["rentalPeriod"] = p.getRentalPeriod();
}

inline void to_json(json& j, const Book& b) {
    serialize_product_base(j, b); 
    j["productType"] = "Book";
    j["author"] = b.getAuthor();
}

inline void to_json(json& j, const Movie& m) {
    serialize_product_base(j, m); 
    j["productType"] = "Movie";
    j["director"] = m.getDirector();
    j["mainActors"] = m.getMainActors();
}


// =======================================================================
// === SERVIDOR PRINCIPAL ===
// =======================================================================

int main() {
    httplib::Server svr;

    // --- Instâncias dos seus "Serviços" / "Controladores" ---
    Motor motor;
    Validate validateService;
    Login loginService; 

    // --- BANCO DE DADOS FALSO (MOCK) ---
    // [TODO: Substituir isso pelo seu Banco de Dados SQL]
    vector<Account> mockAccountDatabase;
    vector<shared_ptr<Product>> mockProductDatabase;
    int nextProductId = 1;


    // --- Habilita CORS ---
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type, Authorization"}
    });

    svr.Options(R"(.*)", [](const httplib::Request&, httplib::Response& res) {
        res.status = 200;
    });

    // --- Rotas do Motor (Existentes) ---
    svr.Post("/api/start", [&](const httplib::Request&, httplib::Response& res) { /* ... */ });
    svr.Post("/api/stop", [&](const httplib::Request&, httplib::Response& res) { /* ... */ });
    svr.Get("/api/status", [&](const httplib::Request&, httplib::Response& res) { /* ... */ });

    // =======================================================================
    // === ROTAS DE AUTENTICAÇÃO (Entrar, Cadastrar, Sair) ===
    // =======================================================================

    /**
     * @route POST /api/signup
     * @desc "Cadastrar" um novo usuário e conta.
     * @body { "email": "...", "cpf": "...", "fullname": "...", "birthday": "...", "username": "...", "password": "..." }
     */
    svr.Post("/api/signup", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);

            // 1. Extrair dados do JSON
            string email = j.at("email");
            string cpf = j.at("cpf");
            string fullname = j.at("fullname");
            string birthday = j.at("birthday");
            string username = j.at("username");
            string password = j.at("password");

            // 2. [TODO: Adicionar validações com 'validateService' aqui]
            // ex: if (!validateService.validateEmail(email)) { ... }

            // 3. Criar os objetos
            User newUser = validateService.createUser(email, cpf, fullname, birthday);
            Account newAccount = validateService.createAccount(password, username, newUser);

            // 4. Salvar no Banco de Dados
            // [TODO: Integrar com Banco de Dados SQL aqui]
            mockAccountDatabase.push_back(newAccount); // Salva no DB Falso

            cout << "Novo usuário cadastrado: " << newAccount.getUsername() << endl;

            // 5. Retornar o objeto 'Account' criado (sem o hash!)
            json j_response = newAccount;
            res.status = 201; // 201 Created
            res.set_content(j_response.dump(4), "application/json");

        } catch (json::parse_error& e) {
            res.status = 400; res.set_content(json{{"error", "JSON inválido"}}.dump(), "application/json");
        } catch (json::type_error& e) {
            res.status = 400; res.set_content(json{{"error", "Campos ausentes no JSON"}}.dump(), "application/json");
        } catch (exception& e) {
            res.status = 400; res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });

    /**
     * @route POST /api/login
     * @desc "Entrar" (Autenticar um usuário).
     * @body { "username": "...", "password": "..." }
     */
    svr.Post("/api/login", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            string username = j.at("username");
            string password = j.at("password");

            // 1. Encontrar o usuário no DB
            // [TODO: Integrar com Banco de Dados SQL aqui]
            Account* foundAccount = nullptr;
            for (auto& acc : mockAccountDatabase) {
                if (acc.getUsername() == username) {
                    foundAccount = &acc;
                    break;
                }
            }

            if (foundAccount == nullptr) {
                throw runtime_error("Usuário não encontrado");
            }

            // 2. Chamar sua lógica de login
            if (loginService.login(*foundAccount, username, password)) {
                
                // [!!! ATENÇÃO !!!]
                // Como sua classe Login é STATEFUL (guarda 'loggedIn'), ela não é ideal para
                // um servidor web. O CORRETO seria gerar um Token (JWT) aqui e enviá-lo.
                // Vou apenas retornar "OK" por enquanto.
                //
                // [TODO: Gerar um Token JWT e retornar para o cliente]
                // string token = jwt::create()...
                
                json j_response = {
                    {"message", "Login bem-sucedido!"},
                    {"user", *foundAccount}
                    // {"token", token} // O frontend salvaria isso
                };
                res.status = 200;
                res.set_content(j_response.dump(4), "application/json");

            } else {
                throw runtime_error("Senha incorreta");
            }

        } catch (json::parse_error& e) {
            res.status = 400; res.set_content(json{{"error", "JSON inválido"}}.dump(), "application/json");
        } catch (json::type_error& e) {
            res.status = 400; res.set_content(json{{"error", "Campos ausentes no JSON"}}.dump(), "application/json");
        } catch (exception& e) {
            res.status = 401; res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });

    /**
     * @route POST /api/logout
     * @desc "Sair"
     */
    svr.Post("/api/logout", [&](const httplib::Request&, httplib::Response& res) {
        // [!!! ATENÇÃO !!!]
        // Esta rota só faz sentido por causa da sua classe 'Login' stateful.
        // Em um sistema de Token, o frontend apenas "esquece" o token.
        loginService.logout();
        res.status = 200;
        res.set_content(json{{"message", "Logout realizado"}}.dump(), "application/json");
    });


    // =======================================================================
    // === ROTAS DE PRODUTOS (Catálogo, Cadastrar, Meus Produtos) ===
    // =======================================================================

    /**
     * @route GET /api/products
     * @desc "Exibir Catálogo", "Pesquisar", "Filtro".
     * @query ?type=Book (ou Movie)
     * @query ?genre=Fantasia
     * @query ?q=Harry (para busca)
     */
    svr.Get("/api/products", [&](const httplib::Request& req, httplib::Response& res) {
        // [TODO: Buscar produtos do Banco de Dados SQL aqui]
        auto& database = mockProductDatabase; // Usando nosso DB falso

        json j_response = json::array();

        // Lógica de Filtro e Pesquisa
        bool filter_type = req.has_param("type");
        string type = req.get_param_value("type");
        bool filter_genre = req.has_param("genre");
        string genre = req.get_param_value("genre");
        bool filter_q = req.has_param("q");
        string q = req.get_param_value("q"); // Termo da pesquisa

        for (const auto& product_ptr : database) {
            
            // Lógica de Filtro
            if (filter_type && product_ptr->getProductType() != type) continue;
            if (filter_genre && product_ptr->getGenre() != genre) continue;
            if (filter_q && product_ptr->getName().find(q) == string::npos) continue;

            // Serialização Polimórfica (precisamos saber se é Book ou Movie)
            if (auto book = dynamic_pointer_cast<Book>(product_ptr)) {
                j_response.push_back(*book);
            } else if (auto movie = dynamic_pointer_cast<Movie>(product_ptr)) {
                j_response.push_back(*movie);
            }
        }

        res.status = 200;
        res.set_content(j_response.dump(4), "application/json");
    });

    /**
     * @route POST /api/products
     * @desc "Cadastrar Produto"
     * @body { "productType": "Book", "name": "...", "author": "...", ... }
     * @body { "productType": "Movie", "name": "...", "director": "...", ... }
     */
    svr.Post("/api/products", [&](const httplib::Request& req, httplib::Response& res) {
        // [TODO: Esta rota precisa de AUTENTICAÇÃO]
        // (Verificar o Token JWT do usuário para saber quem é o 'owner')
        
        try {
            json j = json::parse(req.body);
            string type = j.at("productType");

            shared_ptr<Product> newProduct = nullptr;
            int newId = nextProductId++;

            string owner = j.value("owner", "user_desconhecido"); // [TODO: Pegar do Token]

            if (type == "Book") {
                // O construtor do Book parece OK
                auto book = make_shared<Book>(
                    j.at("name"), j.value("description", ""), j.at("author"),
                    j.value("ownersConsiderations", ""), owner,
                    j.value("genre", ""), j.value("idiom", ""), j.at("rentValue"),
                    j.value("recommendedAge", ""), newId
                );
                newProduct = book;

            } else if (type == "Movie") {
                // [ATENÇÃO: Seu Construtor 'Movie.h' está vazio!]
                // Tive que usar os 'setters' manualmente.
                auto movie = make_shared<Movie>(
                    j.at("name"), j.value("description", ""),
                    j.at("director"), j.at("mainActors"),
                    j.value("ownersConsiderations", ""), owner,
                    j.value("genre", ""), j.value("idiom", ""), j.at("rentValue"),
                    j.value("recommendedAge", ""), newId
                );
                // Preenchendo os dados via setters...
                movie->setId(newId);
                movie->setName(j.at("name"));
                movie->setOwner(owner);
                movie->setRentValue(j.at("rentValue"));
                // ... (adicionar outros setters)
                
                newProduct = movie;
            } else {
                throw runtime_error("productType inválido. Deve ser 'Book' ou 'Movie'.");
            }

            // [TODO: Salvar 'newProduct' no Banco de Dados SQL]
            mockProductDatabase.push_back(newProduct);

            res.status = 201; // 201 Created
            if (type == "Book") res.set_content(json(*dynamic_pointer_cast<Book>(newProduct)).dump(4), "application/json");
            if (type == "Movie") res.set_content(json(*dynamic_pointer_cast<Movie>(newProduct)).dump(4), "application/json");

        } catch (json::parse_error& e) {
            res.status = 400; res.set_content(json{{"error", "JSON inválido"}}.dump(), "application/json");
        } catch (json::type_error& e) {
            res.status = 400; res.set_content(json{{"error", "Campos ausentes no JSON"}}.dump(), "application/json");
        } catch (exception& e) {
            res.status = 400; res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });

    /**
     * @route GET /api/my-products
     * @desc "Exibir Meus Produtos Cadastrados"
     */
    svr.Get("/api/my-products", [&](const httplib::Request& req, httplib::Response& res) {
        // [!!! ROTA PROTEGIDA - PRECISA DE AUTENTICAÇÃO !!!]
        
        // [TODO: 1. Pegar o cabeçalho 'Authorization: Bearer <token>']
        // string token = req.get_header_value("Authorization");
        
        // [TODO: 2. Validar o token e extrair o ID/Username do usuário]
        // string userId = validarToken(token);
        // if (userId.empty()) {
        //    res.status = 401; // Unauthorized
        //    res.set_content(json{{"error", "Token inválido ou expirado"}}.dump(), "application/json");
        //    return;
        // }
        
        // [TODO: 3. Filtrar a busca no DB pelo 'userId']
        // ex: "SELECT * FROM products WHERE owner_id = ?"
        
        // Como não temos token, vou retornar 401
        res.status = 401;
        json j_error = {
            {"error", "Acesso não autorizado"},
            {"message", "[TODO: Esta rota precisa de um Token JWT no cabeçalho 'Authorization']"}
        };
        res.set_content(j_error.dump(4), "application/json");
    });


    // =======================================================================
    // === ROTAS DE PERFIL (Salvar Alterações) ===
    // =======================================================================

    /**
     * @route PUT /api/profile
     * @desc "Salvar alterações (profile)"
     * @body { "username": "...", "fullname": "...", "new_password": "..." }
     */
    svr.Put("/api/profile", [&](const httplib::Request& req, httplib::Response& res) {
        // [!!! ROTA PROTEGIDA - PRECISA DE AUTENTICAÇÃO !!!]

        // [TODO: 1. Pegar o token, validar e pegar o 'userId']
        // string userId = validarToken(req.get_header_value("Authorization"));
        // if (userId.empty()) { ... return 401 ... }

        // [TODO: 2. Buscar o 'Account' e 'User' no DB usando o 'userId']
        // Account& account = accountService.getById(userId);
        // User& user = account.getUser();

        // [TODO: 3. Parsear o JSON e atualizar os campos]
        // json j = json::parse(req.body);
        // if (j.contains("username")) account.change_username(j["username"]);
        // if (j.contains("fullname")) user.update_fullname(j["fullname"]);
        
        // [TODO: 4. Salvar as mudanças no DB]
        // accountService.update(account);

        res.status = 401; // Unauthorized
        json j_error = {
            {"error", "Acesso não autorizado"},
            {"message", "[TODO: Esta rota precisa de um Token JWT para saber QUAL perfil atualizar]"}
        };
        res.set_content(j_error.dump(4), "application/json");
    });


    // --- Iniciar Servidor ---
    cout << "Servidor rodando em http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);

    return 0;
}