#include "app.h"
#include <iostream>
#include <limits> // For numeric_limits
#include <string>
#include <cstdlib> // For system("clear")

// --- Constructor & Destructor ---

/**
 * @brief App constructor.
 * @param database A reference to the Repository (Dependency Injection).
 */
App::App(Repository& database)
    : db(database), currentUser(nullptr) {
    // 'loginSession' and 'validator' are initialized via their default constructors.
}

/**
 * @brief App destructor.
 * @details Cleans up the dynamically allocated currentUser Account object.
 */
App::~App() {
    delete currentUser;
}

// --- Main Entry Point ---

/**
 * @brief Starts the application's main loop.
 * @details This is the primary method called by main().
 */
void App::run() {
    showAuthMenu(); // Start at the authentication menu
}

// --- Utility Methods ---

/**
 * @brief Clears the 'cin' error state and ignores the rest of the input buffer.
 * @details Prevents failed 'cin' from causing infinite loops.
 */
void App::clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/**
 * @brief Prompts the user and reads a full line of text.
 * @param prompt The message to display to the user.
 * @return The user's input string.
 */
string App::getStringLine(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

/**
 * @brief Prompts the user and safely reads an integer, looping on invalid input.
 * @param prompt The message to display to the user.
 * @return The user's validated input integer.
 */
int App::getInt(const string& prompt) {
    int input;
    while (true) {
        cout << prompt;
        cin >> input;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a number." << endl;
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return input;
        }
    }
}

/**
 * @brief Prompts the user and safely reads a float, looping on invalid input.
 * @param prompt The message to display to the user.
 * @return The user's validated input float.
 */
float App::getFloat(const string& prompt) {
    float input;
    while (true) {
        cout << prompt;
        cin >> input;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a number (e.g., 12.50)." << endl;
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return input;
        }
    }
}


// --- Route: Authentication ---

/**
 * @brief Displays the main authentication menu (Login/Register/Exit).
 * @details This is the first screen the user sees. It loops until the
 * user successfully logs in (breaking the loop) or exits.
 */
void App::showAuthMenu() {
    clearScreen();
    string option;
    while (true) {
        cout << "\n===== Bem-vindo ao Booked! =====" << endl;
        cout << "1. Cadastrar conta" << endl;
        cout << "2. Login" << endl;
        cout << "3. Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> option;
        clearInputBuffer();

        if (option == "1") {
            handleRegister();
        } else if (option == "2") {
            handleLogin();
            if (loginSession.isLoggedIn()) {
                break; // Successful login, break loop
            }
        } else if (option == "3") {
            return; // Exit the application
        } else {
            cout << "Opção inválida. Tente novamente." << endl;
        }
    }
    showMainMenu(); // Move to the main app menu
}

// --- Route: Registration ---

/**
 * @brief Handles the new user registration workflow.
 * @details Collects and validates all data for a new User and a new Account.
 */
void App::handleRegister() {
    clearScreen();
    cout << "\n===== Cadastro de Novo Usuário =====" << endl;

    string email, cpf, fullname, birthday, username, password;

    // --- Collect User data ---
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
    // (Birthday validation is pending)

    // --- Create User (CUSTOMER table) ---
    User newUser = validator.createUser(email, cpf, fullname, birthday);
    if (!db.registerUser(&newUser)) {
        cout << "Erro: Não foi possível cadastrar o usuário (CPF ou Email podem já existir)." << endl;
        return; // Failed to create user
    }

    // --- Collect Account data ---
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

    // --- Create Account (ACCOUNT table) ---
    Account newAccount = validator.createAccount(password, username, newUser);
    if (!db.registerAccount(newAccount)) {
        cout << "Erro: Não foi possível criar a conta (Username pode já existir)." << endl;
        // Ideally: delete the User that was just created (rollback)
        return;
    }
    cout << "Cadastro realizado com sucesso!" << endl;
}

// --- Route: Login ---

/**
 * @brief Handles the user login workflow.
 */
void App::handleLogin() {
    clearScreen();
    cout << "\n===== Login =====" << endl;
    string username = getStringLine("Usuário: ");
    string password = getStringLine("Senha: ");

    // Get account credentials from the database
    Account* accountFromDb = db.getAccountByUsername(username);
    if (accountFromDb == nullptr) {
        cout << "Usuário não encontrado." << endl;
        return;
    }

    // Attempt to login using the session manager
    if (loginSession.login(*accountFromDb, username, password)) {
        cout << "\nLogin realizado com sucesso!" << endl;
        cout << "Bem-vindo, " << accountFromDb->getUser().getFullname() << "!" << endl;
        currentUser = accountFromDb; // Login success! Store the account pointer
    } else {
        cout << "Senha incorreta." << endl;
        delete accountFromDb; // Login failed, clean up the pointer
    }
}

// --- Route: Main Menu (Post-Login) ---

/**
 * @brief Displays the main menu for an authenticated user.
 * @details This is the main application loop.
 */
void App::showMainMenu() {
    string option;
    bool loggedIn = true;

    while (loggedIn) {
        clearScreen();
        cout << "\n===== Menu Principal =====" << endl;

        // Security check: If currentUser was deleted (e.g., by handleDeleteAccount), force logout.
        if (!currentUser) {
            loggedIn = false;
            break; // User was deleted, force logout
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
        cin >> option;
        clearInputBuffer();

        if (option == "1") {
            handleCatalog();
        } else if (option == "2") {
            handleAddProduct();
        } else if (option == "3") {
            // handleProfile() returns true if the user deleted their account.
            if (handleProfile()) {
                loggedIn = false; // Signal to exit main loop
            }
        } else if (option == "4") {
            handleMyProducts();
        } else if (option == "5") {
            handleRentedProducts();
        } else if (option == "6") {
            handleLogout();
            loggedIn = false; // User chose to log out
        } else {
            cout << "Opção inválida." << endl;
        }
    }

    showAuthMenu(); // Loop ends (logout/deletion), return to the auth menu
}

/**
 * @brief Displays the menu for choosing a product type to add.
 */
void App::handleAddProduct() {
    clearScreen();
    string option;
    cout << "\n===== Adicionar Novo Produto =====" << endl;
    cout << "Que tipo de produto deseja adicionar?" << endl;
    cout << "1. Livro" << endl;
    cout << "2. Filme" << endl;
    cout << "3. Jogo de Tabuleiro" << endl;
    cout << "4. Videogame" << endl;
    cout << "0. Voltar" << endl;
    cout << "Escolha uma opcao: ";
    cin >> option;
    clearInputBuffer();

    if (option == "1") {
        handleAddBook();
    } else if (option == "2") {
        handleAddMovie();
    } else if (option == "3") {
        handleAddBoardGame();
    } else if (option == "4") {
        handleAddVideoGame();
    } else if (option == "0") {
        return; // Go back
    } else {
        cout << "Opção inválida." << endl;
    }
}

// --- Route: Catalog ---

/**
 * @brief Displays the product catalog and filtering options.
 */
void App::handleCatalog() {
    clearScreen();
    string option;

    while (true) {
        cout << "\n===== Catálogo de Produtos =====" << endl;
        cout << "1. Ver Todos os Produtos" << endl;
        cout << "2. Ver Livros (BOOK)" << endl;
        cout << "3. Ver Filmes (MOVIE)" << endl;
        cout << "4. Ver Videogames (VIDEOGAME)" << endl;
        cout << "5. Ver Jogos de Tabuleiro (BOARDGAME)" << endl;
        cout << "--------------------------------" << endl;
        cout << "6. Alugar um item (por ID)" << endl; // <-- NEW
        cout << "0. Voltar ao Menu Principal" << endl;
        cout << "Escolha uma opcao: ";
        cin >> option;
        clearInputBuffer();

        if (option == "1") {
            db.listAllProducts();
        } else if (option == "2") {
            db.listProductsByType("BOOK");
        } else if (option == "3") {
            db.listProductsByType("MOVIE");
        } else if (option == "4") {
            db.listProductsByType("VIDEOGAME");
        } else if (option == "5") {
            db.listProductsByType("BOARDGAME");
        } else if (option == "6") {
            handleRentItem(); // <-- NEW
        } else if (option == "0") {
            break; // Exit catalog loop
        } else {
            cout << "Opção inválida." << endl;
        }
    }
}

/**
 * @brief A helper that prompts the user for all common Product fields.
 * @details This populates the parameters passed by reference.
 */
void App::collectCommonProductData(string& name, string& desc, string& genre, string& idiom, float& rentValue, string& recAge) {
    name = getStringLine("Nome: ");
    desc = getStringLine("Descrição: ");
    genre = getStringLine("Gênero: ");
    idiom = getStringLine("Idioma: ");
    rentValue = getFloat("Valor do Aluguel (diária): ");
    recAge = getStringLine("Idade Recomendada: ");
}

/**
 * @brief Handles the workflow for adding a new Book.
 */
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

/**
 * @brief Handles the workflow for adding a new Movie.
 */
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

/**
 * @brief Handles the workflow for adding a new Video Game.
 */
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

/**
 * @brief Handles the workflow for adding a new Board Game.
 */
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

/**
 * @brief Handles the workflow for renting an item by its ID.
 */
void App::handleRentItem() {
    clearScreen();
    cout << "\n--- Alugar um Item ---" << endl;

    int itemId = getInt("Digite o ID do item que deseja alugar: ");

    // Get the item's current status and price from the DB
    std::pair<std::string, float> itemInfo = db.getItemStatusAndPrice(itemId);
    std::string status = itemInfo.first;
    float dailyRate = itemInfo.second;

    // Check if item is available
    if (status == "Alugado") {
        cout << "ERRO: Este item já está alugado. Não pode ser alugado novamente." << endl;
        return;
    }
    if (status == "NaoEncontrado" || status == "ERRO") {
        cout << "ERRO: Item com ID " << itemId << " não encontrado." << endl;
        return;
    }

    // Get user input for rental details
    cout << "Item: (ID " << itemId << ") - Diária: R$ " << dailyRate << endl;
    int duration = getInt("Por quantos dias você deseja alugar? ");
    std::string startDate = getStringLine("Data de início (YYYY-MM-DD): ");

    // Get the ID of the currently logged-in user
    int borrowerId = currentUser->getUser().getId();

    // Create the Rental object
    Rental newRental(itemId, borrowerId, duration, startDate, dailyRate);
    newRental.setStatus("Ativo"); // Set status to active

    // Attempt to register the rental in the RENTALS table
    if (db.registerRental(&newRental)) {

        // If rental is registered, update the item's status in the ITEM table
        if (db.updateItemStatus(itemId, "Alugado")) {
            cout << "Item alugado com sucesso!" << endl;
            cout << "ID da Transação: " << newRental.getTransactionId() << endl;
        } else {
            // This is a critical error state
            cout << "ERRO FATAL: O aluguel foi registrado, mas não foi possível atualizar o status do item." << endl;
        }
    } else {
        cout << "Erro: Não foi possível registrar o aluguel." << endl;
    }
}

// --- Other Routes ---

/**
 * @brief Displays the user's profile and management options (Update, Delete).
 * @return true if the user deleted their account, false otherwise.
 */
bool App::handleProfile() {
    clearScreen();
    std::string option;

    while (true) {
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
        cin >> option;
        clearInputBuffer();

        if (option == "1") {
            handleUpdateProfile();
        } else if (option == "2") {
            bool accountDeleted = handleDeleteAccount();
            if (accountDeleted) {
                return true; // Signal to logout
            }
        } else if (option == "0") {
            break; // Return to main menu
        } else {
            cout << "Opção inválida." << endl;
        }
    }
    return false; // Did not delete account
}

/**
 * @brief Displays the user's own listed products and provides deletion options.
 */
void App::handleMyProducts() {
    clearScreen();
    

    std::string ownerCpf = currentUser->getUser().getCpf();
    db.listProductsByOwner(ownerCpf);

    while (true) {
        cout << "\n--- Opções de Produtos ---" << endl;
        cout << "1. Remover um produto (pelo ID)" << endl;
        cout << "0. Voltar ao Menu Principal" << endl;
        cout << "Escolha uma opcao: ";

        std::string option;
        cin >> option;
        clearInputBuffer();

        if (option == "1") {
            handleDeleteProduct();

            // Refresh the list after deletion
            clearScreen();
            cout << "\n--- Lista de Produtos Atualizada ---" << endl;
            db.listProductsByOwner(ownerCpf);
        } else if (option == "0") {
            break; // Return to main menu
        } else {
            cout << "Opção inválida." << endl;
        }
    }
}

/**
 * @brief Displays the user's rental history (active and past) and return options.
 */
void App::handleRentedProducts() {
    clearScreen();
    int customerId = currentUser->getUser().getId();
    db.listRentalsByCustomer(customerId);

    while (true) {
        cout << "\n--- Opções de Aluguel ---" << endl;
        cout << "1. Devolver um item (pelo ID do Aluguel)" << endl;
        cout << "0. Voltar ao Menu Principal" << endl;
        cout << "Escolha uma opcao: ";

        std::string option;
        cin >> option;
        clearInputBuffer();

        if (option == "1") {
            handleReturnItem();

            // Refresh the list after returning an item
            clearScreen();
            cout << "\n--- Lista de Aluguéis Atualizada ---" << endl;
            db.listRentalsByCustomer(customerId);
        } else if (option == "0") {
            break; // Return to main menu
        } else {
            cout << "Opção inválida." << endl;
        }
    }
}

/**
 * @brief Handles deleting one of the user's own products by its ID.
 */
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

    // Pause for user to read the message
    cout << "Pressione Enter para continuar...";
    getStringLine("");
}

/**
 * @brief Handles returning a currently rented item by its Rental ID.
 */
void App::handleReturnItem() {
    clearScreen();
    cout << "\n--- Devolver um Item ---" << endl;

    int rentalId = getInt("Digite o ID do Aluguel (o 'ID_do_Aluguel' da lista) que deseja devolver: ");

    if (db.returnRental(rentalId)) {
        cout << "Item devolvido com sucesso!" << endl;
    } else {
        cout << "Erro: Não foi possível processar a devolução." << endl;
    }

    // Pause for user to read the message
    cout << "Pressione Enter para continuar...";
    getStringLine(""); // Just waits for user to press Enter
}

// --- Route: Logout ---

/**
 * @brief Logs the user out and cleans up the session data.
 */
void App::handleLogout() {
    loginSession.logout();
    delete currentUser; // Delete the heap-allocated Account object
    currentUser = nullptr; // Set to null to prevent dangling pointer
    cout << "\nVocê saiu com sucesso." << endl;
}

// --- Clear Terminal ---

/**
 * @brief Clears the console screen.
 * @details Uses platform-dependent system calls.
 */
void App::clearScreen() {
    system("clear"); // For macOS/Linux
    // system("cls"); // For Windows (alternative)
}

/**
 * @brief Displays the menu for updating profile details (Name, Username, Password).
 */
bool App::handleUpdateProfile() {
    clearScreen();
    std::string option;

    while (true) {
        cout << "\n--- Atualizar Perfil ---" << endl;
        cout << "1. Mudar Nome Completo" << endl;
        cout << "2. Mudar Nome de Usuário (Username)" << endl;
        cout << "3. Mudar Senha" << endl;
        cout << "0. Voltar ao Perfil" << endl;
        cout << "Escolha uma opcao: ";
        cin >> option;
        clearInputBuffer();

        if (option == "1") {
            handleUpdateFullName();
        } else if (option == "2") {
            handleUpdateUsername();
        } else if (option == "3") {
            handleUpdatePassword();
        } else if (option == "0") {
            break; // Return to previous menu (handleProfile)
        } else {
            cout << "Opção inválida." << endl;
        }
    }
    // Return 'false' to indicate the account was NOT deleted
    return false;
}

/**
 * @brief Handles changing the user's full name in the database and in memory.
 */
void App::handleUpdateFullName() {
    clearScreen();
    cout << "\n--- Mudar Nome Completo ---" << endl;
    std::string newName = getStringLine("Digite seu novo nome completo: ");

    if (!validator.validateFullname(newName)) {
        cout << "Erro: Nome inválido." << endl;
        return;
    }

    if (db.updateUserFullName(currentUser->getUser().getId(), newName)) {
        // Also update the object in memory!
        currentUser->getUser().update_fullname(newName);
        cout << "Nome completo atualizado com sucesso!" << endl;
    } else {
        cout << "Erro: Não foi possível atualizar o nome." << endl;
    }
    cout << "Pressione Enter para continuar...";
    getStringLine(""); // Pause
}

/**
 * @brief Handles changing the user's login username in the database and in memory.
 */
void App::handleUpdateUsername() {
    clearScreen();
    cout << "\n--- Mudar Nome de Usuário ---" << endl;
    std::string newUsername = getStringLine("Digite o novo nome de usuário: ");

    if (!validator.validateUsername(newUsername)) {
        cout << "Erro: Nome de usuário inválido (3-15 caracteres)." << endl;
        return;
    }

    if (db.updateAccountUsername(currentUser->getId(), newUsername)) {
        // Also update the object in memory!
        currentUser->change_username(newUsername);
        cout << "Nome de usuário atualizado com sucesso!" << endl;
    } else {
        cout << "Erro: Não foi possível atualizar. Esse nome de usuário já pode estar em uso." << endl;
    }
    cout << "Pressione Enter para continuar...";
    getStringLine(""); // Pause
}

/**
 * @brief Handles changing the user's password in the database and in memory.
 */
void App::handleUpdatePassword() {
    clearScreen();
    cout << "\n--- Mudar Senha ---" << endl;
    std::string currentPass = getStringLine("Digite sua senha ATUAL: ");

    // 1. Verify current password
    if (validator.hashPassword(currentPass) != currentUser->getHash()) {
        cout << "Erro: Senha atual incorreta." << endl;
        return;
    }

    // 2. Get and validate new password
    std::string newPass = getStringLine("Digite a NOVA senha: ");
    if (!validator.validateStringPassword(newPass)) {
        cout << "Senha fraca (min 6 chars, 1 maiúsc, 1 minúsc, 1 num, 1 especial)." << endl;
        return;
    }

    // 3. Save new password
    size_t newHash = validator.hashPassword(newPass);
    if (db.updateAccountPassword(currentUser->getId(), newHash)) {
        // Also update the object in memory!
        currentUser->change_password(currentUser->getHash(), newHash);
        cout << "Senha atualizada com sucesso!" << endl;
    } else {
        cout << "Erro: Não foi possível atualizar a senha." << endl;
    }
    cout << "Pressione Enter para continuar...";
    getStringLine(""); // Pause
}

/**
 * @brief Handles the permanent deletion of the user's account.
 * @details This is an irreversible action.
 * @return true if deletion was successful, false otherwise.
 */
bool App::handleDeleteAccount() {
    clearScreen();
    cout << "\n--- Deletar Conta (PERMANENTE) ---" << endl;
    cout << "Esta ação é irreversível e irá deletar todos os seus dados e produtos." << endl;
    std::string pass = getStringLine("Para confirmar, digite sua senha: ");

    // 1. Verify password
    if (validator.hashPassword(pass) != currentUser->getHash()) {
        cout << "Erro: Senha atual incorreta. Exclusão cancelada." << endl;
        cout << "Pressione Enter para continuar...";
        getStringLine("");
        return false; // Did not delete
    }

    // 2. Try to delete (Repository will do constraint checking, e.g., active rentals)
    int customerId = currentUser->getUser().getId();
    std::string accountId = currentUser->getId();
    std::string customerCpf = currentUser->getUser().getCpf();

    if (db.deleteUserAccount(customerId, accountId, customerCpf)) {
        cout << "Sua conta foi deletada com sucesso. Você será desconectado." << endl;

        // 3. Logout and clear session data
        loginSession.logout();
        delete currentUser;
        currentUser = nullptr;

        cout << "Pressione Enter para sair...";
        getStringLine("");
        return true; // SIGNALS THAT THE ACCOUNT WAS DELETED
    } else {
        cout << "Erro: Não foi possível deletar a conta." << endl;
        cout << "(Verifique se você possui itens alugados ou emprestados)." << endl;
        cout << "Pressione Enter para continuar...";
        getStringLine("");
        return false; // Did not delete
    }
}