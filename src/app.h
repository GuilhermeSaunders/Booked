#ifndef APP_H
#define APP_H

#include <string>
#include "login.h"
#include "account.h"
#include "repository.h"        // Inclui o Repositório
#include "validate.h" // Inclui o Validador

// Inclui todos os modelos de produto para o cadastro
#include "Book.h"
#include "Movie.h"
#include "video_game.h"
#include "board_game.h"

using namespace std;

class App {
private:
    Repositorio& db; // Referência para o banco de dados (Injeção de Dependência)
    Login loginSession;
    Validate validator;
    Account* currentUser; // Ponteiro para o usuário logado

    // --- Métodos Privados (as "Rotas" internas) ---

    // Utilitário
    void clearInputBuffer();
    void clearScreen();
    string getStringLine(const string& prompt);
    int getInt(const string& prompt);
    float getFloat(const string& prompt);
    void collectCommonProductData(std::string& name, std::string& desc, std::string& genre, std::string& idiom, float& rentValue, std::string& recAge);

    // Rota de Autenticação
    void showAuthMenu();
    void handleLogin();
    void handleRegister();

    // Rota Principal (Pós-Login)
    void showMainMenu();
    void handleCatalog();
    void handleAddProduct();
        // Sub-rotas de Adicionar Produto
        void handleAddBook();
        void handleAddMovie();
        void handleAddVideoGame();
        void handleAddBoardGame();
    void handleProfile();
    void handleMyProducts();
    void handleRentedProducts();
    void handleLogout();
    void handleRentItem();
    void handleReturnItem();

public:
    // Construtor: recebe o banco de dados
    App(Repositorio& database);

    // Destrutor: para limpar o currentUser
    ~App();

    // Ponto de entrada principal que o main() chamará
    void run(); 
};

#endif