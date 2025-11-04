#include "app.h"
#include <iostream>
#include <limits> 
#include <string> 
#include <cstdlib> 

// --- Construtor e Destrutor ---

App::App(Repository& database) 
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
        cout << "\n===== Bem-vindo ao Booked! =====" << endl;
        cout << "1. Cadastrar conta" << endl;
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
    string opcao;
    bool loggedIn = true; 

    while (loggedIn) { 
        clearScreen();
        cout << "\n===== Menu Principal =====" << endl;
        
        // Checagem de segurança (caso o usuário tenha sido deletado)
        if (!currentUser) {
            loggedIn = false;
            break;
        }

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

            if (handleProfile()) { 
                loggedIn = false; 
            }
        } else if (opcao == "4") {
            handleMyProducts();
        } else if (opcao == "5") {
            handleRentedProducts();
        } else if (opcao == "6") {
            handleLogout();
            loggedIn = false;
        } else {
            cout << "Opção inválida." << endl;
        }
    }

    showAuthMenu();
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

    std::pair<std::string, float> itemInfo = db.getItemStatusAndPrice(itemId);
    std::string status = itemInfo.first;
    float dailyRate = itemInfo.second;

    if (status == "Alugado") {
        cout << "ERRO: Este item já está alugado. Não pode ser alugado novamente." << endl;
        return;
    }
    if (status == "NaoEncontrado" || status == "ERRO") {
        cout << "ERRO: Item com ID " << itemId << " não encontrado." << endl;
        return;
    }

    cout << "Item: (ID " << itemId << ") - Diária: R$ " << dailyRate << endl;
    int duration = getInt("Por quantos dias você deseja alugar? ");
    std::string startDate = getStringLine("Data de início (YYYY-MM-DD): ");

    int borrowerId = currentUser->getUser().getId();

    Rental newRental(itemId, borrowerId, duration, startDate, dailyRate);
    newRental.setStatus("Ativo");

    if (db.registerRental(&newRental)) {

        if (db.updateItemStatus(itemId, "Alugado")) {
            cout << "Item alugado com sucesso!" << endl;
            cout << "ID da Transação: " << newRental.getTransactionId() << endl;
        } else {
            cout << "ERRO FATAL: O aluguel foi registrado, mas não foi possível atualizar o status do item." << endl;

        }
    } else {
        cout << "Erro: Não foi possível registrar o aluguel." << endl;
    }
}

// --- Outras Rotas (Stubs) ---
// Em src/App.cpp

bool App::handleProfile() {
    clearScreen();
    std::string opcao;

    while(true) {
        cout << "\n===== Perfil do Usuário =====" << endl;
        cout << "Usuário: " << currentUser->getUsername() << endl;
        cout << "Nome: " << currentUser->getUser().getFullname() << endl;
        cout << "Email: " << currentUser->getUser().getEmail() << endl;
        cout << "CPF: " << currentUser->getUser().getCpf() << endl;
        cout << "-------------------------------" << endl;
        cout << "1. Atualizar Perfil" << endl;
        cout << "2. Deletar Conta (PERMANENTE)" << endl;
        cout << "0. Voltar ao Menu Principal" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        clearInputBuffer();

        if (opcao == "1") {
            handleUpdateProfile();
        } else if (opcao == "2") {
            bool contaDeletada = handleDeleteAccount();
            if (contaDeletada) {
                return true;
            }
        } else if (opcao == "0") {
            break; // Volta ao menu principal
        } else {
            cout << "Opção inválida." << endl;
        }
    }
    return false;
}
void App::handleMyProducts() {
    clearScreen();
    cout << "\n--- Meus Produtos Cadastrados ---" << endl;

    std::string cpfDoDono = currentUser->getUser().getCpf();
    db.listProductsByOwner(cpfDoDono);

    while(true) {
        cout << "\n--- Opções de Produtos ---" << endl;
        cout << "1. Remover um produto (pelo ID)" << endl;
        cout << "0. Voltar ao Menu Principal" << endl;
        cout << "Escolha uma opcao: ";
        
        std::string opcao;
        cin >> opcao;
        clearInputBuffer();

        if (opcao == "1") {
            handleDeleteProduct();

            clearScreen();
            cout << "\n--- Lista de Produtos Atualizada ---" << endl;
            db.listProductsByOwner(cpfDoDono);
        } else if (opcao == "0") {
            break; // Volta ao menu principal
        } else {
            cout << "Opção inválida." << endl;
        }
    }
}
void App::handleRentedProducts() {
    clearScreen();
    cout << "\n--- Meus Aluguéis (Ativos e Passados) ---" << endl;

    int customerId = currentUser->getUser().getId();
    db.listRentalsByCustomer(customerId);

    while(true) {
        cout << "\n--- Opções de Aluguel ---" << endl;
        cout << "1. Devolver um item (pelo ID do Aluguel)" << endl;
        cout << "0. Voltar ao Menu Principal" << endl;
        cout << "Escolha uma opcao: ";
        
        std::string opcao;
        cin >> opcao;
        clearInputBuffer();

        if (opcao == "1") {
            handleReturnItem();

            clearScreen();
            cout << "\n--- Lista de Aluguéis Atualizada ---" << endl;
            db.listRentalsByCustomer(customerId);
        } else if (opcao == "0") {
            break; 
        } else {
            cout << "Opção inválida." << endl;
        }
    }
}

void App::handleDeleteProduct() {
    clearScreen();
    cout << "\n--- Remover Produto ---" << endl;
    
    int itemId = getInt("Digite o ID do produto que deseja remover: ");
    std::string ownerCpf = currentUser->getUser().getCpf();

    if (db.deleteProduct(itemId, ownerCpf)) {
        cout << "Produto ID " << itemId << " removido com sucesso!" << endl;
    } else {
        cout << "Erro: Produto não encontrado, não pertence a você, ou está alugado." << endl;
    }

    cout << "Pressione Enter para continuar...";
    getStringLine("");
}

void App::handleReturnItem() {
    clearScreen();
    cout << "\n--- Devolver um Item ---" << endl;
    
    int rentalId = getInt("Digite o ID do Aluguel (o 'ID_do_Aluguel' da lista) que deseja devolver: ");

    if (db.returnRental(rentalId)) {
        cout << "Item devolvido com sucesso!" << endl;
    } else {
        cout << "Erro: Não foi possível processar a devolução." << endl;
    }
    
    // Pausa para o usuário ler a mensagem
    cout << "Pressione Enter para continuar...";
    getStringLine(""); // Apenas espera o usuário apertar Enter
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

bool App::handleUpdateProfile() {
    clearScreen();
    std::string opcao;

    while(true) {
        cout << "\n--- Atualizar Perfil ---" << endl;
        cout << "1. Mudar Nome Completo" << endl;
        cout << "2. Mudar Nome de Usuário (Username)" << endl;
        cout << "3. Mudar Senha" << endl;
        cout << "0. Voltar ao Perfil" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        clearInputBuffer();

        if (opcao == "1") {
            handleUpdateFullName();
        } else if (opcao == "2") {
            handleUpdateUsername();
        } else if (opcao == "3") {
            handleUpdatePassword();
        } else if (opcao == "0") {
            break; // Volta ao menu anterior (handleProfile)
        } else {
            cout << "Opção inválida." << endl;
        }
    }
    // Retorna 'false' para indicar que a conta NÃO foi deletada
    return false;
}

void App::handleUpdateFullName() {
    clearScreen();
    cout << "\n--- Mudar Nome Completo ---" << endl;
    std::string newName = getStringLine("Digite seu novo nome completo: ");

    if (!validator.validateFullname(newName)) {
        cout << "Erro: Nome inválido." << endl;
        return;
    }

    if (db.updateUserFullName(currentUser->getUser().getId(), newName)) {
        // Atualiza também o objeto em memória!
        currentUser->getUser().update_fullname(newName); 
        cout << "Nome completo atualizado com sucesso!" << endl;
    } else {
        cout << "Erro: Não foi possível atualizar o nome." << endl;
    }
    cout << "Pressione Enter para continuar...";
    getStringLine("");
}

void App::handleUpdateUsername() {
    clearScreen();
    cout << "\n--- Mudar Nome de Usuário ---" << endl;
    std::string newUsername = getStringLine("Digite o novo nome de usuário: ");

    if (!validator.validateUsername(newUsername)) {
        cout << "Erro: Nome de usuário inválido (3-15 caracteres)." << endl;
        return;
    }

    if (db.updateAccountUsername(currentUser->getId(), newUsername)) {
        // Atualiza também o objeto em memória!
        currentUser->change_username(newUsername);
        cout << "Nome de usuário atualizado com sucesso!" << endl;
    } else {
        cout << "Erro: Não foi possível atualizar. Esse nome de usuário já pode estar em uso." << endl;
    }
    cout << "Pressione Enter para continuar...";
    getStringLine("");
}

void App::handleUpdatePassword() {
    clearScreen();
    cout << "\n--- Mudar Senha ---" << endl;
    std::string currentPass = getStringLine("Digite sua senha ATUAL: ");
    
    // 1. Verificar senha atual
    if (validator.hashPassword(currentPass) != currentUser->getHash()) {
        cout << "Erro: Senha atual incorreta." << endl;
        return;
    }

    // 2. Pedir e validar nova senha
    std::string newPass = getStringLine("Digite a NOVA senha: ");
    if (!validator.validateStringPassword(newPass)) {
        cout << "Senha fraca (min 6 chars, 1 maiúsc, 1 minúsc, 1 num, 1 especial)." << endl;
        return;
    }
    
    // 3. Salvar nova senha
    size_t newHash = validator.hashPassword(newPass);
    if (db.updateAccountPassword(currentUser->getId(), newHash)) {
        // Atualiza também o objeto em memória!
        currentUser->change_password(currentUser->getHash(), newHash);
        cout << "Senha atualizada com sucesso!" << endl;
    } else {
        cout << "Erro: Não foi possível atualizar a senha." << endl;
    }
    cout << "Pressione Enter para continuar...";
    getStringLine("");
}

bool App::handleDeleteAccount() {
    clearScreen();
    cout << "\n--- Deletar Conta (PERMANENTE) ---" << endl;
    cout << "Esta ação é irreversível e irá deletar todos os seus dados e produtos." << endl;
    std::string pass = getStringLine("Para confirmar, digite sua senha: ");

    // 1. Verificar senha
    if (validator.hashPassword(pass) != currentUser->getHash()) {
        cout << "Erro: Senha atual incorreta. Exclusão cancelada." << endl;
        cout << "Pressione Enter para continuar...";
        getStringLine("");
        return false; // Não deletou
    }

    // 2. Tentar deletar (o Repository fará as verificações de aluguel)
    int customerId = currentUser->getUser().getId();
    std::string accountId = currentUser->getId();
    std::string customerCpf = currentUser->getUser().getCpf();

    if (db.deleteUserAccount(customerId, accountId, customerCpf)) {
        cout << "Sua conta foi deletada com sucesso. Você será desconectado." << endl;
        
        // 3. Fazer logout e limpar dados da sessão
        loginSession.logout();
        delete currentUser;
        currentUser = nullptr;
        
        cout << "Pressione Enter para sair...";
        getStringLine("");
        return true; // SINALIZA QUE DELETOU A CONTA
    } else {
        cout << "Erro: Não foi possível deletar a conta." << endl;
        cout << "(Verifique se você possui itens alugados ou emprestados)." << endl;
        cout << "Pressione Enter para continuar...";
        getStringLine("");
        return false; // Não deletou
    }
}