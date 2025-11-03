#include "app.h"
#include <iostream>
#include <limits> 
#include <string> 
#include <cstdlib> 

// --- Construtor e Destrutor ---

App::App(Repositorio& database) 
    : db(database), currentUser(nullptr) {}

App::~App() {
    delete currentUser;
}

// --- Ponto de Entrada Principal ---

void App::run() {
    showAuthMenu();
}

// --- Métodos Utilitários ---

void App::clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string App::getStringLine(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

int App::getInt(const string& prompt) {
    int input;
    while (true) {
        cout << prompt;
        cin >> input;
        if (cin.fail()) {
            cout << "Entrada inválida. Por favor, digite um número." << endl;
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return input;
        }
    }
}

float App::getFloat(const string& prompt) {
    float input;
    while (true) {
        cout << prompt;
        cin >> input;
        if (cin.fail()) {
            cout << "Entrada inválida. Por favor, digite um número (ex: 12.50)." << endl;
            clearInputBuffer();
        } else {
            clearInputBuffer(); 
            return input;
        }
    }
}


// --- Rota: Autenticação ---

void App::showAuthMenu() {
    clearScreen();
    string opcao;
    while (true) { 
        cout << "\n===== Sistema de Aluguel =====" << endl;
        cout << "1. Cadastrar" << endl;
        cout << "2. Login" << endl;
        cout << "3. Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        clearInputBuffer(); 

        if (opcao == "1") {
            handleRegister();
        } else if (opcao == "2") {
            handleLogin();
            if (loginSession.isLoggedIn()) {
                break;
            }
        } else if (opcao == "3") {
            return;
        } else {
            cout << "Opção inválida. Tente novamente." << endl;
        }
    }
    showMainMenu();
}

// --- Rota: Cadastro ---

void App::handleRegister() {
    clearScreen();
    cout << "\n===== Cadastro de Novo Usuário =====" << endl;
    
    string email, cpf, fullname, birthday, username, password;
    
    while (true) {
        email = getStringLine("Email: ");
        if (validator.validateEmail(email)) break;
        cout << "Email inválido. Tente novamente." << endl;
    }
    while (true) {
        cpf = getStringLine("CPF (apenas 11 números): ");
        if (validator.validateCpf(cpf)) break;
        cout << "CPF inválido. Deve conter 11 dígitos." << endl;
    }
    while (true) {
        fullname = getStringLine("Nome Completo: ");
        if (validator.validateFullname(fullname)) break;
        cout << "Nome inválido. Tente novamente." << endl;
    }
    birthday = getStringLine("Data de Nascimento (DD/MM/AAAA): ");
    // (Validação de data pendente)

    User newUser = validator.createUser(email, cpf, fullname, birthday);
    if (!db.registerUser(&newUser)) {
        cout << "Erro: Não foi possível cadastrar o usuário (CPF ou Email podem já existir)." << endl;
        return;
    }
    

    while (true) {
        username = getStringLine("Nome de usuário (para login): ");
        if (validator.validateUsername(username)) break;
        cout << "Nome de usuário inválido (3-15 caracteres)." << endl;
    }
    while (true) {
        password = getStringLine("Senha: ");
        if (validator.validateStringPassword(password)) break;
        cout << "Senha fraca (min 6 chars, 1 maiúsc, 1 minúsc, 1 num, 1 especial)." << endl;
    }

    Account newAccount = validator.createAccount(password, username, newUser);
    if (!db.registerAccount(newAccount)) {
        cout << "Erro: Não foi possível criar a conta (Username pode já existir)." << endl;
        // Idealmente: deletar o User que acabou de ser criado (rollback)
        return;
    }
    cout << "Cadastro realizado com sucesso!" << endl;
}

// --- Rota: Login ---

void App::handleLogin() {
    clearScreen();
    cout << "\n===== Login =====" << endl;
    string username = getStringLine("Usuário: ");
    string password = getStringLine("Senha: ");

    Account* accountFromDb = db.getAccountByUsername(username);
    if (accountFromDb == nullptr) {
        cout << "Usuário não encontrado." << endl;
        return;
    }
    if (loginSession.login(*accountFromDb, username, password)) {
        cout << "\nLogin realizado com sucesso!" << endl;
        cout << "Bem-vindo, " << accountFromDb->getUser().getFullname() << "!" << endl;
        currentUser = accountFromDb; 
    } else {
        cout << "Senha incorreta." << endl;
        delete accountFromDb;
    }
}

// --- Rota: Menu Principal (Pós-Login) ---

void App::showMainMenu() {
    clearScreen();
    string opcao;
    while (true) {
        cout << "\n===== Menu Principal =====" << endl;
        cout << "Usuário: " << currentUser->getUsername() << endl;
        cout << "--------------------------" << endl;
        cout << "1. Ver Catálogo" << endl;
        cout << "2. Adicionar Produto para Aluguel" << endl;
        cout << "3. Ver Perfil" << endl;
        cout << "4. Meus Produtos" << endl;
        cout << "5. Meus Aluguéis" << endl;
        cout << "6. Logout" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        clearInputBuffer();

        if (opcao == "1") {
            handleCatalog();
        } else if (opcao == "2") {
            handleAddProduct();
        } else if (opcao == "3") {
            handleProfile();
        } else if (opcao == "4") {
            handleMyProducts();
        } else if (opcao == "5") {
            handleRentedProducts();
        } else if (opcao == "6") {
            handleLogout();
            break;
        } else {
            cout << "Opção inválida." << endl;
        }
    }
    showAuthMenu(); // Volta para o menu de login após o logout
}

void App::handleAddProduct() {
    clearScreen();
    string opcao;
    cout << "\n===== Adicionar Novo Produto =====" << endl;
    cout << "Que tipo de produto deseja adicionar?" << endl;
    cout << "1. Livro" << endl;
    cout << "2. Filme" << endl;
    cout << "3. Jogo de Tabuleiro" << endl;
    cout << "4. Videogame" << endl;
    cout << "0. Voltar" << endl;
    cout << "Escolha uma opcao: ";
    cin >> opcao;
    clearInputBuffer();

    if (opcao == "1") {
        handleAddBook();
    } else if (opcao == "2") {
        handleAddMovie();
    } else if (opcao == "3") {
        handleAddBoardGame();
    } else if (opcao == "4") {
        handleAddVideoGame();
    } else if (opcao == "0") {
        return;
    } else {
        cout << "Opção inválida." << endl;
    }
}

// --- Rota: Catálogo (IMPLEMENTADA) ---
void App::handleCatalog() {
    clearScreen();
    string opcao;
    
    while(true) {
        cout << "\n===== Catálogo de Produtos =====" << endl;
        cout << "1. Ver Todos os Produtos" << endl;
        cout << "2. Ver Livros (BOOK)" << endl;
        cout << "3. Ver Filmes (MOVIE)" << endl;
        cout << "4. Ver Videogames (VIDEOGAME)" << endl;
        cout << "5. Ver Jogos de Tabuleiro (BOARDGAME)" << endl;
        cout << "--------------------------------" << endl;
        cout << "6. Alugar um item (por ID)" << endl; // <-- NOVO
        cout << "0. Voltar ao Menu Principal" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        clearInputBuffer();

        if (opcao == "1") {
            db.listAllProducts();
        } else if (opcao == "2") {
            db.listProductsByType("BOOK");
        } else if (opcao == "3") {
            db.listProductsByType("MOVIE");
        } else if (opcao == "4") {
            db.listProductsByType("VIDEOGAME");
        } else if (opcao == "5") {
            db.listProductsByType("BOARDGAME");
        } else if (opcao == "6") {
            handleRentItem(); // <-- NOVO
        } else if (opcao == "0") {
            break; // Sai do loop do catálogo
        } else {
            cout << "Opção inválida." << endl;
        }
    }
}

// Coleta dados comuns a todos os produtos
void App::collectCommonProductData(string& name, string& desc, string& genre, string& idiom, float& rentValue, string& recAge) {
    name = getStringLine("Nome: ");
    desc = getStringLine("Descrição: ");
    genre = getStringLine("Gênero: ");
    idiom = getStringLine("Idioma: ");
    rentValue = getFloat("Valor do Aluguel (diária): ");
    recAge = getStringLine("Idade Recomendada: ");
}

void App::handleAddBook() {
    clearScreen();
    cout << "\n--- Adicionar Livro ---" << endl;
    string name, desc, genre, idiom, recAge, author;
    float rentValue;
    int numPages;
    
    collectCommonProductData(name, desc, genre, idiom, rentValue, recAge);
    string owner = currentUser->getUser().getCpf();
    
    author = getStringLine("Autor: ");
    numPages = getInt("Número de Páginas: ");
    
    Book book(rentValue, owner, name, desc, idiom, recAge, genre, numPages, author);
    if (db.registerBook(&book)) {
        cout << "Livro cadastrado com sucesso com o ID: " << book.getId() << endl;
    } else {
        cout << "Erro ao cadastrar o livro." << endl;
    }
}

void App::handleAddMovie() {
    clearScreen();
    cout << "\n--- Adicionar Filme ---" << endl;
    string name, desc, genre, idiom, recAge, director, mainActors;
    float rentValue;
    int duration;

    collectCommonProductData(name, desc, genre, idiom, rentValue, recAge);
    string owner = currentUser->getUser().getCpf();

    director = getStringLine("Diretor: ");
    mainActors = getStringLine("Atores Principais: ");
    duration = getInt("Duração (minutos): ");
    
    Movie movie(rentValue, owner, name, desc, idiom, recAge, genre, director, mainActors, duration);
    if (db.registerMovie(&movie)) {
        cout << "Filme cadastrado com sucesso com o ID: " << movie.getId() << endl;
    } else {
        cout << "Erro ao cadastrar o filme." << endl;
    }
}

void App::handleAddVideoGame() {
    clearScreen();
    cout << "\n--- Adicionar Videogame ---" << endl;
    string name, desc, genre, idiom, recAge, style, platform;
    float rentValue;
    int duration, numPlayers;

    collectCommonProductData(name, desc, genre, idiom, rentValue, recAge);
    string owner = currentUser->getUser().getCpf();

    style = getStringLine("Estilo (ex: FPS, RPG): ");
    platform = getStringLine("Plataforma (ex: PC, PS5): ");
    duration = getInt("Duração média (horas): ");
    numPlayers = getInt("Número de jogadores: ");
    
    Video_Game videogame(rentValue, owner, name, desc, idiom, recAge, genre, style, platform, duration, numPlayers);
    if (db.registerVideoGame(&videogame)) {
        cout << "Videogame cadastrado com sucesso com o ID: " << videogame.getId() << endl;
    } else {
        cout << "Erro ao cadastrar o videogame." << endl;
    }
}

void App::handleAddBoardGame() {
    clearScreen();
    cout << "\n--- Adicionar Jogo de Tabuleiro ---" << endl;
    string name, desc, genre, idiom, recAge, style;
    float rentValue;
    int duration, numPlayers;

    collectCommonProductData(name, desc, genre, idiom, rentValue, recAge);
    string owner = currentUser->getUser().getCpf();

    style = getStringLine("Estilo (ex: Estratégia, Cooperativo): ");
    numPlayers = getInt("Número de jogadores: ");
    duration = getInt("Duração média (minutos): ");
    
    Board_Game boardgame(rentValue, owner, name, desc, idiom, recAge, genre, style, numPlayers, duration);
    if (db.registerBoardGame(&boardgame)) {
        cout << "Jogo de Tabuleiro cadastrado com sucesso com o ID: " << boardgame.getId() << endl;
    } else {
        cout << "Erro ao cadastrar o Jogo de Tabuleiro." << endl;
    }
}

void App::handleRentItem() {
    clearScreen();
    cout << "\n--- Alugar um Item ---" << endl;

    int itemId = getInt("Digite o ID do item que deseja alugar: ");
    int duration = getInt("Por quantos dias você deseja alugar? ");
    std::string startDate = getStringLine("Data de início (YYYY-MM-DD): ");

    int borrowerId = currentUser->getUser().getId();
    float dailyRate = 5.50; 

    Rental newRental(itemId, borrowerId, duration, startDate, dailyRate);
    newRental.setStatus("Ativo");
    if (db.registerRental(&newRental)) {
        // Se deu certo, atualizamos o status do item
        if (db.updateItemStatus(itemId, "Alugado")) {
            cout << "Item alugado com sucesso!" << endl;
            cout << "ID da Transação: " << newRental.getTransactionId() << endl;
        } else {
            cout << "Erro: O aluguel foi registrado, mas não foi possível atualizar o status do item." << endl;

        }
    } else {
        cout << "Erro: Não foi possível registrar o aluguel." << endl;
    }
}

// --- Outras Rotas (Stubs) ---
void App::handleProfile() {
    clearScreen();
    cout << "===== Perfil do Usuário =====" << endl;
    cout << "Nome: " << currentUser->getUser().getFullname() << endl;
    cout << "Email: " << currentUser->getUser().getEmail() << endl;
    cout << "CPF: " << currentUser->getUser().getCpf() << endl;
    cout << "Username: " << currentUser->getUsername() << endl;
}
void App::handleMyProducts() {
    clearScreen();
    // 1. Obter o CPF do usuário que está logado na sessão
    std::string cpfDoDono = currentUser->getUser().getCpf();
    // 2. Chamar a nova função do repositório
    db.listProductsByOwner(cpfDoDono);
}
void App::handleRentedProducts() {
    clearScreen();
    // 1. Obter o ID do usuário que está logado na sessão
    int customerId = currentUser->getUser().getId();

    // 2. Chamar a nova função do repositório
    db.listRentalsByCustomer(customerId);
    
}

// --- Rota: Logout ---

void App::handleLogout() {
    loginSession.logout();
    delete currentUser; 
    currentUser = nullptr;
    cout << "\nVocê saiu com sucesso." << endl;
}

// Limpar o Terminal
void App::clearScreen() {
    system("clear");
}