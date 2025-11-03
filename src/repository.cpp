#include "repository.h"    
#include "DBSchema.h"       
#include <iostream>         
#include <string>   
#include "sqlite3.h" 
#include <sstream> // Para construir SQL

// --- Funções de Callback (para listagem) ---

// Este é um callback do SQLite. Ele é chamado para CADA linha de resultado.
static int listProductsCallback(void* data, int argc, char** argv, char** azColName) {
    // argc = número de colunas
    // argv = array de valores como strings
    // azColName = array de nomes das colunas
    
    std::cout << "------------------------------------" << std::endl;
    // Imprime "NOME_COLUNA = VALOR"
    for(int i = 0; i < argc; i++){
        std::cout << "  " << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0; // 0 = sucesso, continue para a próxima linha
}


// --- Construtor / Destrutor ---

Repositorio::Repositorio(const std::string& db_path) : db(nullptr) {
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Could not open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    } else {
        std::cout << "Database '" << db_path << "' opened successfully." << std::endl;
        // Habilita suporte a Chaves Estrangeiras (MUITO IMPORTANTE)
        executeSQL("PRAGMA foreign_keys = ON;");
    }
}

Repositorio::~Repositorio() {
    if (db) {
        sqlite3_close(db);
        std::cout << "Database closed." << std::endl;
    }
}

// --- Funções Internas ---

// Executa SQL que NÃO retorna dados (CREATE, INSERT, UPDATE, DELETE)
bool Repositorio::executeSQL(const std::string& sql) {
    if (!db) return false;
    
    char* errMsg = 0;
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Error executing SQL: " << errMsg << std::endl;
        std::cerr << "Failed SQL: " << sql << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// --- Inicialização ---

void Repositorio::tablesStart() {
    if (!db) return;
    std::cout << "Initializing tables..." << std::endl;
    executeSQL(DBSchema::CUSTOMER);
    executeSQL(DBSchema::ACCOUNT);
    executeSQL(DBSchema::ITEM);
    executeSQL(DBSchema::BOOKS);
    executeSQL(DBSchema::MOVIES);
    executeSQL(DBSchema::VIDEOGAMES);
    executeSQL(DBSchema::BOARDGAMES);
    executeSQL(DBSchema::RENTALS);
    std::cout << "Tables initialized." << std::endl;
}

// --- Funções de Customer/Account ---

bool Repositorio::registerUser(User* user) {
    if (!db || !user) return false;

    std::stringstream ss;
    ss << "INSERT INTO CUSTOMER (CPF, NAME, EMAIL, BIRTHDAY) VALUES ('"
       << user->getCpf() << "', '"
       << user->getFullname() << "', '"
       << user->getEmail() << "', '"
       << user->getBirthday() + "');";
    
    if (executeSQL(ss.str())) {
        // Pega o ID que o banco acabou de gerar...
        long long id = sqlite3_last_insert_rowid(db);
        // ...e atualiza o objeto C++
        user->setId(static_cast<int>(id));
        return true;
    }
    return false;
}

bool Repositorio::registerAccount(const Account& account) {
    if (!db) return false;

    int customerId = account.getUser().getId(); 
    std::stringstream ss;
    ss << "INSERT INTO ACCOUNT (ID, CUSTOMER_ID, USERNAME, PASSWORD_HASH) VALUES ('"
       << account.getId() << "', "
       << customerId << ", '"
       << account.getUsername() << "', "
       << account.getHash() << ");";
    
    return executeSQL(ss.str());
}

Account* Repositorio::getAccountByUsername(const std::string& username) {
    if (!db) return nullptr;
    
    sqlite3_stmt* stmt;
    Account* foundAccount = nullptr;

    const char* sql = "SELECT A.ID, A.PASSWORD_HASH, A.USERNAME, "
                    "C.ID, C.CPF, C.NAME, C.EMAIL, C.BIRTHDAY "
                    "FROM ACCOUNT A "
                    "JOIN CUSTOMER C ON A.CUSTOMER_ID = C.ID "
                    "WHERE A.USERNAME = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Erro ao preparar statement: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Dados do User (Tabela CUSTOMER)
        int userId = sqlite3_column_int(stmt, 3);
        const char* cpf = (const char*)sqlite3_column_text(stmt, 4);
        const char* name = (const char*)sqlite3_column_text(stmt, 5);
        const char* email = (const char*)sqlite3_column_text(stmt, 6);
        const char* birthday = (const char*)sqlite3_column_text(stmt, 7);

        User user(email, cpf, name, birthday);
        user.setId(userId);

        // Dados da Account (Tabela ACCOUNT)
        const char* accId = (const char*)sqlite3_column_text(stmt, 0);
        size_t hash = (size_t)sqlite3_column_int64(stmt, 1);
        const char* uname = (const char*)sqlite3_column_text(stmt, 2);

        foundAccount = new Account(accId, hash, uname, user);
    }

    sqlite3_finalize(stmt);
    return foundAccount;
}

// --- Funções de Produto (CREATE) ---

// Função base para inserir em ITEM (usada pelas outras)
// Retorna o ID do item inserido, ou -1 em caso de falha.
long long registerBaseItem(sqlite3* db, Product* product) {
    std::stringstream ssItem;
    ssItem << "INSERT INTO ITEM (NAME, DESCRIPTION, OWNER_CPF, GENRE, IDIOM, RENT_VALUE, RECOMMENDED_AGE, TYPE, STATUS) VALUES ("
           << "'" << product->getName() << "', "
           << "'" << product->getDescription() << "', "
           << "'" << product->getOwner() << "', "
           << "'" << product->getGenre() << "', "
           << "'" << product->getIdiom() << "', "
           << product->getRentValue() << ", "
           << "'" << product->getRecommendedAge() << "', "
           << "'" << product->getType() << "', 'Disponivel');";
    
    char* errMsg = 0;
    if (sqlite3_exec(db, ssItem.str().c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Error executing SQL: " << errMsg << std::endl;
        std::cerr << "Failed SQL: " << ssItem.str() << std::endl;
        sqlite3_free(errMsg);
        return -1;
    }
    
    return sqlite3_last_insert_rowid(db);
}


bool Repositorio::registerBook(Book* book) {
    if (!db) return false;
    
    executeSQL("BEGIN TRANSACTION;"); // Inicia transação

    long long itemId = registerBaseItem(db, book); // Insere em ITEM
    if (itemId == -1) {
        executeSQL("ROLLBACK;");
        return false;
    }
    book->setId(static_cast<int>(itemId)); // Atualiza o ID no objeto

    // Insere em BOOK
    std::stringstream ssBook;
    ssBook << "INSERT INTO BOOK (ITEM_ID, AUTHOR, PAGE_NUM) VALUES ("
           << itemId << ", "
           << "'" << book->getAuthor() << "', "
           << book->getNumPages() << ");";

    if (!executeSQL(ssBook.str())) {
        executeSQL("ROLLBACK;"); // Desfaz
        return false;
    }

    return executeSQL("COMMIT;"); // Confirma
}

bool Repositorio::registerMovie(Movie* movie) {
    if (!db) return false;
    
    executeSQL("BEGIN TRANSACTION;");
    long long itemId = registerBaseItem(db, movie);
    if (itemId == -1) {
        executeSQL("ROLLBACK;");
        return false;
    }
    movie->setId(static_cast<int>(itemId));

    std::stringstream ss;
    ss << "INSERT INTO MOVIE (ITEM_ID, DIRECTOR, MAIN_ACTORS, DURATION) VALUES ("
       << itemId << ", "
       << "'" << movie->getDirector() << "', "
       << "'" << movie->getMainActors() << "', "
       << movie->getDuration() << ");";

    if (!executeSQL(ss.str())) {
        executeSQL("ROLLBACK;");
        return false;
    }
    return executeSQL("COMMIT;");
}

bool Repositorio::registerVideoGame(Video_Game* videogame) {
    if (!db) return false;

    executeSQL("BEGIN TRANSACTION;");
    long long itemId = registerBaseItem(db, videogame);
    if (itemId == -1) {
        executeSQL("ROLLBACK;");
        return false;
    }
    videogame->setId(static_cast<int>(itemId));

    std::stringstream ss;
    ss << "INSERT INTO VIDEOGAME (ITEM_ID, STYLE, PLATAFORM, DURATION) VALUES ("
       << itemId << ", "
       << "'" << videogame->getStyle() << "', "
       << "'" << videogame->getPlataform() << "', "
       << videogame->getDuration() << ");";

    if (!executeSQL(ss.str())) {
        executeSQL("ROLLBACK;");
        return false;
    }
    return executeSQL("COMMIT;");
}

bool Repositorio::registerBoardGame(Board_Game* boardgame) {
    if (!db) return false;

    executeSQL("BEGIN TRANSACTION;");
    long long itemId = registerBaseItem(db, boardgame);
    if (itemId == -1) {
        executeSQL("ROLLBACK;");
        return false;
    }
    boardgame->setId(static_cast<int>(itemId));

    std::stringstream ss;
    ss << "INSERT INTO BOARDGAME (ITEM_ID, STYLE, PLAYERS_NUM, DURATION) VALUES ("
       << itemId << ", "
       << "'" << boardgame->getStyle() << "', "
       << boardgame->getNumPlayers() << ", "
       << boardgame->getDuration() << ");";

    if (!executeSQL(ss.str())) {
        executeSQL("ROLLBACK;");
        return false;
    }
    return executeSQL("COMMIT;");
}

// --- Funções de Produto (READ) ---

void Repositorio::listAllProducts() {
    if (!db) return;
    
    const char* sql = "SELECT I.NAME AS 'Nome', I.TYPE AS 'Tipo', I.GENRE AS 'Gênero', I.DESCRIPTION AS 'Descrição', "
                      "I.RENT_VALUE AS 'Valor diário de aluguel', I.STATUS AS 'Status', I.ID AS 'Id do Produto', "
                      "C.NAME AS 'Dono(a)', C.EMAIL AS 'Email do Dono' "
                      "FROM ITEM I "
                      "JOIN CUSTOMER C ON I.OWNER_CPF = C.CPF;";
    
    char* errMsg = 0;

    std::cout << "\n--- Catálogo de Produtos  ---" << std::endl; 

    if (sqlite3_exec(db, sql, listProductsCallback, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Erro ao listar produtos: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    std::cout << "------------------------------------" << std::endl;
}

void Repositorio::listProductsByType(const std::string& type) {
    if (!db) return;

    std::string sql = "SELECT I.NAME AS 'Nome', I.TYPE AS 'Tipo', I.GENRE AS 'Gênero', I.DESCRIPTION AS 'Descrição', "
                      "I.RENT_VALUE AS 'Valor diário de aluguel', I.STATUS AS 'Status', I.ID AS 'Id do Produto', "
                      "C.NAME AS 'Dono(a)', C.EMAIL AS 'Email do Dono' "
                      "FROM ITEM I "
                      "JOIN CUSTOMER C ON I.OWNER_CPF = C.CPF "
                      "WHERE I.TYPE = '" + type + "';"; // Onde o tipo ainda é filtrado
    
    char* errMsg = 0;

    std::cout << "\n--- Catálogo de " << type << "s ---" << std::endl;
    
    if (sqlite3_exec(db, sql.c_str(), listProductsCallback, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Erro ao listar produtos: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    std::cout << "------------------------------------" << std::endl;
}


// --- Funções de Rental ---

bool Repositorio::registerRental(Rental* rental) {
    if (!db || !rental) return false;

    // SQL ATUALIZADO para CUSTOMER_ID (int)
    std::stringstream ss;
    ss << "INSERT INTO RENTALS (CUSTOMER_ID, ITEM_ID, START_DATE, DURATION, DAILY_RATE, STATUS) VALUES ("
       << rental->getBorrowerId() << ", "
       << rental->getProductId() << ", '"
       << rental->getStartDate() << "', "
       << rental->getDuration() << ", "
       << rental->getDailyRate() << ", '"
       << rental->getStatus() << "');";
    
    if (executeSQL(ss.str())) {
        long long id = sqlite3_last_insert_rowid(db);
        rental->setTransactionId(static_cast<int>(id));
        return true;
    }
    return false;
}
bool Repositorio::updateItemStatus(int itemId, const std::string& newStatus) {
    if (!db) return false;

    std::stringstream ss;
    ss << "UPDATE ITEM SET STATUS = '" << newStatus 
       << "' WHERE ID = " << itemId << ";";
    
    return executeSQL(ss.str());
}
void Repositorio::listProductsByOwner(const std::string& ownerCpf) {
    if (!db) return;
    std::string sql = "SELECT NAME AS 'Nome', TYPE AS 'Tipo', RENT_VALUE AS 'Valor diário de aluguel', STATUS AS 'Status', ID AS 'Id do produto'"
                      "FROM ITEM "
                      "WHERE OWNER_CPF = '" + ownerCpf + "';";
    
    char* errMsg = 0;

    std::cout << "\n--- Meus Produtos Cadastrados ---" << std::endl;
    
    if (sqlite3_exec(db, sql.c_str(), listProductsCallback, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Erro ao listar meus produtos: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    std::cout << "------------------------------------" << std::endl;
}
void Repositorio::listRentalsByCustomer(int customerId) {
    if (!db) return;

    // SQL: Seleciona as colunas mais importantes do aluguel
    // e usa um JOIN para buscar o nome do produto na tabela ITEM.
    // Filtra tudo pelo ID do cliente que está logado.
    std::string sql = "SELECT "
                      "  R.ID AS 'ID_do_Aluguel', "
                      "  I.NAME AS 'Produto', "
                      "  R.START_DATE AS 'Data_Inicio', "
                      "  R.DURATION AS 'Dias', "
                      "  R.DAILY_RATE AS 'Diaria_R$', "
                      "  R.STATUS AS 'Status_Aluguel' "
                      "FROM RENTALS R "
                      "JOIN ITEM I ON R.ITEM_ID = I.ID "
                      "WHERE R.CUSTOMER_ID = " + std::to_string(customerId) + ";";
    
    char* errMsg = 0;

    std::cout << "\n--- Meus Aluguéis (Ativos e Passados) ---" << std::endl;
    
    // Reutilizamos o mesmo callback de listagem.
    // Ele vai imprimir as colunas que definimos (ex: 'ID_do_Aluguel', 'Produto')
    if (sqlite3_exec(db, sql.c_str(), listProductsCallback, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Erro ao listar meus aluguéis: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    std::cout << "------------------------------------" << std::endl;
}
std::pair<std::string, float> Repositorio::getItemStatusAndPrice(int itemId) {
    if (!db) return {"ERRO", 0.0f};

    sqlite3_stmt* stmt;
    std::string status = "NaoEncontrado";
    float price = 0.0f;

    const char* sql = "SELECT STATUS, RENT_VALUE FROM ITEM WHERE ID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Erro ao preparar statement: " << sqlite3_errmsg(db) << std::endl;
        return {"ERRO", 0.0f};
    }

    sqlite3_bind_int(stmt, 1, itemId);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Se encontramos o item, pegamos os dados
        status = (const char*)sqlite3_column_text(stmt, 0);
        price = (float)sqlite3_column_double(stmt, 1);
    }

    sqlite3_finalize(stmt);
    return {status, price};
}

int getItemIdFromRental(sqlite3* db, int rentalId) {
    sqlite3_stmt* stmt;
    int itemId = -1;
    const char* sql = "SELECT ITEM_ID FROM RENTALS WHERE ID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        return -1;
    }
    sqlite3_bind_int(stmt, 1, rentalId);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        itemId = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return itemId; 
}

bool Repositorio::returnRental(int rentalId) {
    if (!db) return false;

    if (!executeSQL("BEGIN TRANSACTION;")) return false;

    int itemId = getItemIdFromRental(db, rentalId);
    
    if (itemId == -1) {
        cout << "Erro: ID de Aluguel não encontrado." << endl;
        executeSQL("ROLLBACK;");
        return false;
    }

    std::string sqlUpdateRental = "UPDATE RENTALS SET STATUS = 'Devolvido' WHERE ID = " + std::to_string(rentalId) + ";";
    if (!executeSQL(sqlUpdateRental)) {
        std::cerr << "Erro ao atualizar o status do aluguel." << std::endl;
        executeSQL("ROLLBACK;");
        return false;
    }

    std::string sqlUpdateItem = "UPDATE ITEM SET STATUS = 'Disponivel' WHERE ID = " + std::to_string(itemId) + ";";
    if (!executeSQL(sqlUpdateItem)) {
        std::cerr << "Erro ao atualizar o status do item." << std::endl;
        executeSQL("ROLLBACK;");
        return false;
    }

    return executeSQL("COMMIT;");
}