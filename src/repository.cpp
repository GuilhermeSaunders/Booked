#include "repository.h"
#include "DBSchema.h"
#include <iostream>
#include <string>
#include "sqlite3.h"
#include <sstream> // For building SQL strings

using namespace std;

// --- Callback Functions (for listing) ---

/**
 * @brief A C-style callback function for sqlite3_exec.
 * @details This function is called by SQLite for EACH row returned by a SELECT query.
 * It's used here to print the results of list queries directly to the console.
 *
 * @param data    (Not used) A pointer to custom data (unused in this case).
 * @param argc    The number of columns in the result row.
 * @param argv    An array of strings representing the row's data.
 * @param azColName An array of strings representing the column names.
 * @return 0 on success to continue to the next row.
 */
static int listProductsCallback(void* data, int argc, char** argv, char** azColName) {
    // argc = number of columns
    // argv = array of values as strings
    // azColName = array of column names

    std::cout << "------------------------------------" << std::endl;
    // Print "COLUMN_NAME = VALUE" for each column in the row
    for (int i = 0; i < argc; i++) {
        std::cout << "  " << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0; // 0 = success, continue to the next row
}


// --- Constructor / Destructor ---

/**
 * @brief Constructs the Repository and opens the SQLite database.
 * @param db_path The file path to the .db database file.
 */
Repository::Repository(const std::string& db_path) : db(nullptr) {
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        // Failed to open database
        std::cerr << "Could not open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    } else {
        std::cout << "Database '" << db_path << "' opened successfully." << std::endl;
        // VERY IMPORTANT: Enable foreign key support (it's OFF by default in SQLite)
        executeSQL("PRAGMA foreign_keys = ON;");
    }
}

/**
 * @brief Destructs the Repository and closes the database connection.
 */
Repository::~Repository() {
    if (db) {
        sqlite3_close(db);
        std::cout << "Database closed." << std::endl;
    }
}

// --- Internal Utility Functions ---

/**
 * @brief Executes an SQL statement that does NOT return data (e.g., CREATE, INSERT, UPDATE, DELETE).
 * @param sql The SQL string to execute.
 * @return true on success, false on failure.
 */
bool Repository::executeSQL(const std::string& sql) {
    if (!db) return false;

    char* errMsg = 0;
    // sqlite3_exec: The 3rd and 4th params are (callback, callback_data),
    // which we set to 0 (null) because we don't expect rows back.
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Error executing SQL: " << errMsg << std::endl;
        std::cerr << "Failed SQL: " << sql << std::endl;
        sqlite3_free(errMsg); // Free the error message
        return false;
    }
    return true;
}

// --- Initialization ---

/**
 * @brief Initializes the database schema by executing all CREATE TABLE statements.
 */
void Repository::tablesStart() {
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

// --- Customer/Account Functions ---

/**
 * @brief Inserts a new user into the CUSTOMER table.
 * @param user A pointer to the User object containing the data.
 * @return true on success, false on failure.
 */
bool Repository::registerUser(User* user) {
    if (!db || !user) return false;

    // Use a stringstream to build the SQL query
    std::stringstream ss;
    ss << "INSERT INTO CUSTOMER (CPF, NAME, EMAIL, BIRTHDAY) VALUES ('"
       << user->getCpf() << "', '"
       << user->getFullname() << "', '"
       << user->getEmail() << "', '"
       << user->getBirthday() + "');";

    if (executeSQL(ss.str())) {
        // If successful, get the ID the database just generated...
        long long id = sqlite3_last_insert_rowid(db);
        // ...and update the C++ object with this ID.
        user->setId(static_cast<int>(id));
        return true;
    }
    return false;
}

/**
 * @brief Inserts a new account into the ACCOUNT table.
 * @param account The Account object to save (contains the User).
 * @return true on success, false on failure.
 */
bool Repository::registerAccount(const Account& account) {
    if (!db) return false;

    int customerId = account.getUser().getId(); // Get the User's ID
    std::stringstream ss;
    ss << "INSERT INTO ACCOUNT (ID, CUSTOMER_ID, USERNAME, PASSWORD_HASH) VALUES ('"
       << account.getId() << "', "
       << customerId << ", '"
       << account.getUsername() << "', "
       << account.getHash() << ");";

    return executeSQL(ss.str());
}

/**
 * @brief Retrieves a user's Account and User data by their username.
 * @details Uses a prepared statement to safely query the database and prevent SQL injection.
 * @param username The username to search for.
 * @return A dynamically allocated (new) Account pointer if found, otherwise nullptr.
 * The caller is responsible for 'delete'ing this pointer.
 */
Account* Repository::getAccountByUsername(const std::string& username) {
    if (!db) return nullptr;

    sqlite3_stmt* stmt; // This is the "prepared statement"
    Account* foundAccount = nullptr;

    // SQL query with a JOIN to get both Account and Customer data
    const char* sql = "SELECT A.ID, A.PASSWORD_HASH, A.USERNAME, "
                      "C.ID, C.CPF, C.NAME, C.EMAIL, C.BIRTHDAY "
                      "FROM ACCOUNT A "
                      "JOIN CUSTOMER C ON A.CUSTOMER_ID = C.ID "
                      "WHERE A.USERNAME = ?;"; // '?' is a placeholder

    // 1. Prepare the statement (compile the SQL)
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }

    // 2. Bind the username to the '?' placeholder (index 1)
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    // 3. Execute the statement and check if we got a row
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // A row was found, now extract the data
        // Get User data (CUSTOMER table, columns 3-7)
        int userId = sqlite3_column_int(stmt, 3);
        const char* cpf = (const char*)sqlite3_column_text(stmt, 4);
        const char* name = (const char*)sqlite3_column_text(stmt, 5);
        const char* email = (const char*)sqlite3_column_text(stmt, 6);
        const char* birthday = (const char*)sqlite3_column_text(stmt, 7);

        // Reconstruct the User object
        User user(email, cpf, name, birthday);
        user.setId(userId);

        // Get Account data (ACCOUNT table, columns 0-2)
        const char* accId = (const char*)sqlite3_column_text(stmt, 0);
        size_t hash = (size_t)sqlite3_column_int64(stmt, 1);
        const char* uname = (const char*)sqlite3_column_text(stmt, 2);

        // Reconstruct the Account object
        foundAccount = new Account(accId, hash, uname, user);
    }

    // 4. Finalize the statement (clean up)
    sqlite3_finalize(stmt);
    return foundAccount;
}

// --- Product Functions (CREATE) ---

/**
 * @brief (Internal Helper) Inserts the common product data into the ITEM table.
 * @details This is intended to be called by the specific register...() methods.
 * @param db The sqlite3 database handle.
 * @param product A pointer to the Product object (Book, Movie, etc.).
 * @return The newly generated ITEM ID, or -1 on failure.
 */
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
           << "'" << product->getType() << "', 'Disponivel');"; // 'Available'

    char* errMsg = 0;
    if (sqlite3_exec(db, ssItem.str().c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Error executing SQL: " << errMsg << std::endl;
        std::cerr << "Failed SQL: " << ssItem.str() << std::endl;
        sqlite3_free(errMsg);
        return -1; // Return -1 on failure
    }

    // Return the ID of the row that was just inserted
    return sqlite3_last_insert_rowid(db);
}


/**
 * @brief Registers a new Book. Uses a transaction.
 * @details 1. Inserts into ITEM table. 2. Inserts into BOOK table.
 * @param book A pointer to the Book object.
 * @return true on success, false on failure (rolls back).
 */
bool Repository::registerBook(Book* book) {
    if (!db) return false;

    executeSQL("BEGIN TRANSACTION;"); // Start transaction

    long long itemId = registerBaseItem(db, book); // 1. Insert into ITEM
    if (itemId == -1) {
        executeSQL("ROLLBACK;"); // Abort
        return false;
    }
    book->setId(static_cast<int>(itemId)); // Update object's ID

    // 2. Insert into BOOK
    std::stringstream ssBook;
    ssBook << "INSERT INTO BOOK (ITEM_ID, AUTHOR, PAGE_NUM) VALUES ("
           << itemId << ", "
           << "'" << book->getAuthor() << "', "
           << book->getNumPages() << ");";

    if (!executeSQL(ssBook.str())) {
        executeSQL("ROLLBACK;"); // Abort
        return false;
    }

    return executeSQL("COMMIT;"); // Confirm transaction
}

/**
 * @brief Registers a new Movie. Uses a transaction.
 * @param movie A pointer to the Movie object.
 * @return true on success, false on failure (rolls back).
 */
bool Repository::registerMovie(Movie* movie) {
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

/**
 * @brief Registers a new Video Game. Uses a transaction.
 * @param videogame A pointer to the Video_Game object.
 * @return true on success, false on failure (rolls back).
 */
bool Repository::registerVideoGame(Video_Game* videogame) {
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

/**
 * @brief Registers a new Board Game. Uses a transaction.
 * @param boardgame A pointer to the Board_Game object.
 * @return true on success, false on failure (rolls back).
 */
bool Repository::registerBoardGame(Board_Game* boardgame) {
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

// --- Product Functions (READ) ---

/**
 * @brief Lists all products in the catalog to the console.
 */
void Repository::listAllProducts() {
    if (!db) return;

    // SQL query with aliases for nice column names in the callback
    const char* sql = "SELECT I.NAME AS 'Nome', I.TYPE AS 'Tipo', I.GENRE AS 'Gênero', I.DESCRIPTION AS 'Descrição', "
                      "I.RENT_VALUE AS 'Valor diário de aluguel', I.STATUS AS 'Status', I.ID AS 'Id do Produto', "
                      "C.NAME AS 'Dono(a)', C.EMAIL AS 'Email do Dono' "
                      "FROM ITEM I "
                      "JOIN CUSTOMER C ON I.OWNER_CPF = C.CPF;";

    char* errMsg = 0;

    std::cout << "\n--- Catálogo de Produtos  ---" << std::endl;

    // Execute the query, passing 'listProductsCallback' to print each row
    if (sqlite3_exec(db, sql, listProductsCallback, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Erro ao listar produtos: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    std::cout << "------------------------------------" << std::endl;
}

/**
 * @brief Lists all products of a specific type (e.g., "BOOK") to the console.
 * @param type The product type string to filter by.
 */
void Repository::listProductsByType(const std::string& type) {
    if (!db) return;

    // SQL query with a WHERE clause for the type
    std::string sql = "SELECT I.NAME AS 'Nome', I.TYPE AS 'Tipo', I.GENRE AS 'Gênero', I.DESCRIPTION AS 'Descrição', "
                      "I.RENT_VALUE AS 'Valor diário de aluguel', I.STATUS AS 'Status', I.ID AS 'Id do Produto', "
                      "C.NAME AS 'Dono(a)', C.EMAIL AS 'Email do Dono' "
                      "FROM ITEM I "
                      "JOIN CUSTOMER C ON I.OWNER_CPF = C.CPF "
                      "WHERE I.TYPE = '" + type + "';"; // Filtering by type

    char* errMsg = 0;

    std::cout << "\n--- Catálogo de " << type << "s ---" << std::endl;

    if (sqlite3_exec(db, sql.c_str(), listProductsCallback, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Erro ao listar produtos: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    std::cout << "------------------------------------" << std::endl;
}

/**
 * @brief Lists all products owned by a specific user (by CPF).
 * @param ownerCpf The CPF of the owner.
 */
void Repository::listProductsByOwner(const std::string& ownerCpf) {
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

/**
 * @brief Lists all rentals (past and present) for a specific customer.
 * @param customerId The ID of the customer.
 */
void Repository::listRentalsByCustomer(int customerId) {
    if (!db) return;

    // SQL: Selects rental columns and JOINs ITEM to get the product name.
    std::string sql = "SELECT "
                      "  R.ID AS 'ID_do_Aluguel', "
                      "  I.NAME AS 'Produto', "
                      "  R.START_DATE AS 'Data_Inicio', "
                      "  R.DURATION AS 'Dias', "
                      "  R.DAILY_RATE AS 'Diaria_R$', "
                      "  R.STATUS AS 'Status_Aluguel' "
                      "FROM RENTALS R "
                      "JOIN ITEM I ON R.ITEM_ID = I.ID " // Get product name
                      "WHERE R.CUSTOMER_ID = " + std::to_string(customerId) + ";";

    char* errMsg = 0;

    std::cout << "\n--- Meus Aluguéis (Ativos e Passados) ---" << std::endl;

    // We reuse the same listProductsCallback to print the results
    if (sqlite3_exec(db, sql.c_str(), listProductsCallback, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Erro ao listar meus aluguéis: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    std::cout << "------------------------------------" << std::endl;
}


// --- Rental Functions ---

/**
 * @brief Registers a new rental transaction in the RENTALS table.
 * @param rental A pointer to the Rental object.
 * @return true on success, false on failure.
 */
bool Repository::registerRental(Rental* rental) {
    if (!db || !rental) return false;

    // SQL using CUSTOMER_ID (int)
    std::stringstream ss;
    ss << "INSERT INTO RENTALS (CUSTOMER_ID, ITEM_ID, START_DATE, DURATION, DAILY_RATE, STATUS) VALUES ("
       << rental->getBorrowerId() << ", "
       << rental->getProductId() << ", '"
       << rental->getStartDate() << "', "
       << rental->getDuration() << ", "
       << rental->getDailyRate() << ", '"
       << rental->getStatus() << "');";

    if (executeSQL(ss.str())) {
        // Get the generated transaction ID and update the object
        long long id = sqlite3_last_insert_rowid(db);
        rental->setTransactionId(static_cast<int>(id));
        return true;
    }
    return false;
}

/**
 * @brief Updates the STATUS column of an item in the ITEM table.
 * @param itemId The ID of the item to update.
 * @param newStatus The new status string (e.g., "Alugado", "Disponivel").
 * @return true on success, false on failure.
 */
bool Repository::updateItemStatus(int itemId, const std::string& newStatus) {
    if (!db) return false;

    std::stringstream ss;
    ss << "UPDATE ITEM SET STATUS = '" << newStatus
       << "' WHERE ID = " << itemId << ";";

    return executeSQL(ss.str());
}

/**
 * @brief Gets the current status and rent price of a single item.
 * @details Uses a prepared statement to fetch data.
 * @param itemId The ID of the item to check.
 * @return A std::pair containing the item's (status, price).
 */
std::pair<std::string, float> Repository::getItemStatusAndPrice(int itemId) {
    if (!db) return {"ERRO", 0.0f};

    sqlite3_stmt* stmt;
    std::string status = "NaoEncontrado"; // Default if not found
    float price = 0.0f;

    const char* sql = "SELECT STATUS, RENT_VALUE FROM ITEM WHERE ID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return {"ERRO", 0.0f};
    }

    sqlite3_bind_int(stmt, 1, itemId); // Bind the item ID

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // If we found the item, get its data
        status = (const char*)sqlite3_column_text(stmt, 0);
        price = (float)sqlite3_column_double(stmt, 1);
    }

    sqlite3_finalize(stmt);
    return {status, price};
}

/**
 * @brief (Internal Helper) Finds the ITEM_ID associated with a RENTAL_ID.
 * @param db The sqlite3 database handle.
 * @param rentalId The ID of the rental transaction.
 * @return The ITEM_ID on success, or -1 if not found.
 */
int getItemIdFromRental(sqlite3* db, int rentalId) {
    sqlite3_stmt* stmt;
    int itemId = -1; // Default -1 (not found)
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

/**
 * @brief Processes the return of a rental. Uses a transaction.
 * @details 1. Updates RENTALS status to "Devolvido". 2. Updates ITEM status to "Disponivel".
 * @param rentalId The ID of the rental transaction to return.
 * @return true on success, false on failure (rolls back).
 */
bool Repository::returnRental(int rentalId) {
    if (!db) return false;

    if (!executeSQL("BEGIN TRANSACTION;")) return false; // Start transaction

    // 1. Find the Item ID associated with this rental
    int itemId = getItemIdFromRental(db, rentalId);

    if (itemId == -1) {
        cout << "Erro: ID de Aluguel não encontrado." << endl;
        executeSQL("ROLLBACK;");
        return false;
    }

    // 2. Update the RENTALS table
    std::string sqlUpdateRental = "UPDATE RENTALS SET STATUS = 'Devolvido' WHERE ID = " + std::to_string(rentalId) + ";";
    if (!executeSQL(sqlUpdateRental)) {
        std::cerr << "Erro ao atualizar o status do aluguel." << std::endl;
        executeSQL("ROLLBACK;");
        return false;
    }

    // 3. Update the ITEM table (make it available again)
    std::string sqlUpdateItem = "UPDATE ITEM SET STATUS = 'Disponivel' WHERE ID = " + std::to_string(itemId) + ";";
    if (!executeSQL(sqlUpdateItem)) {
        std::cerr << "Erro ao atualizar o status do item." << std::endl;
        executeSQL("ROLLBACK;");
        return false;
    }

    return executeSQL("COMMIT;"); // All good, commit changes
}

// --- Update Functions ---

/**
 * @brief Updates the full name of a user in the CUSTOMER table.
 * @param customerId The ID of the user to update.
 * @param newName The new full name.
 * @return true on success, false on failure.
 */
bool Repository::updateUserFullName(int customerId, const std::string& newName) {
    if (!db) return false;
    std::stringstream ss;
    ss << "UPDATE CUSTOMER SET NAME = '" << newName << "' WHERE ID = " << customerId << ";";
    return executeSQL(ss.str());
}

/**
 * @brief Updates the username of an account in the ACCOUNT table.
 * @param accountId The ID of the account to update.
 * @param newUsername The new username.
 * @return true on success, false on failure.
 */
bool Repository::updateAccountUsername(const std::string& accountId, const std::string& newUsername) {
    if (!db) return false;
    std::stringstream ss;
    ss << "UPDATE ACCOUNT SET USERNAME = '" << newUsername << "' WHERE ID = '" << accountId << "';";
    return executeSQL(ss.str());
}

/**
 * @brief Updates the password hash of an account in the ACCOUNT table.
 * @param accountId The ID of the account to update.
 * @param newHash The new password hash.
 * @return true on success, false on failure.
 */
bool Repository::updateAccountPassword(const std::string& accountId, size_t newHash) {
    if (!db) return false;
    std::stringstream ss;
    ss << "UPDATE ACCOUNT SET PASSWORD_HASH = " << newHash << " WHERE ID = '" << accountId << "';";
    return executeSQL(ss.str());
}


// --- Delete Functions ---

/**
 * @brief Deletes a product from the ITEM table.
 * @details Will only delete if the user is the owner AND the item is not 'Alugado' (Rented).
 * @param itemId The ID of the item to delete.
 * @param ownerCpf The CPF of the user attempting to delete (must match).
 * @return true on success, false if delete fails or conditions aren't met.
 */
bool Repository::deleteProduct(int itemId, const std::string& ownerCpf) {
    if (!db) return false;

    std::stringstream ss;
    // SQL: Delete from ITEM where ID matches, CPF matches, AND status is not 'Rented'.
    ss << "DELETE FROM ITEM WHERE ID = " << itemId
       << " AND OWNER_CPF = '" << ownerCpf
       << "' AND STATUS != 'Alugado';";

    if (!executeSQL(ss.str())) {
        return false; // SQL execution failed
    }

    // Check if any rows were actually changed (deleted)
    if (sqlite3_changes(db) == 0) {
        // 0 changes means the item was not found, or user was not the owner,
        // or the item was 'Alugado' (Rented).
        return false;
    }

    return true; // Deletion was successful
}

/**
 * @brief (Internal Helper) Checks if a user is allowed to delete their account.
 * @details Checks two conditions:
 * 1. User must not have any 'Ativo' (Active) rentals.
 * 2. User must not own any items that are currently 'Alugado' (Rented) by others.
 * @param customerId The user's ID.
 * @param customerCpf The user's CPF.
 * @return true if deletion is allowed, false otherwise.
 */
bool Repository::checkUserDeleteConstraints(int customerId, const std::string& customerCpf) {
    if (!db) return false;

    sqlite3_stmt* stmt;
    int count = 0;

    // 1. Check if the user is actively renting any items
    const char* sqlRentals = "SELECT COUNT(*) FROM RENTALS WHERE CUSTOMER_ID = ? AND STATUS = 'Ativo';";
    if (sqlite3_prepare_v2(db, sqlRentals, -1, &stmt, 0) != SQLITE_OK) return false;
    sqlite3_bind_int(stmt, 1, customerId);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    if (count > 0) {
        std::cerr << "Erro: Você precisa devolver todos os itens que está alugando (" << count << ") antes de deletar sua conta." << std::endl;
        return false; // Fails constraint
    }

    // 2. Check if the user owns any items that are currently rented out
    const char* sqlItems = "SELECT COUNT(*) FROM ITEM WHERE OWNER_CPF = ? AND STATUS = 'Alugado';";
    if (sqlite3_prepare_v2(db, sqlItems, -1, &stmt, 0) != SQLITE_OK) return false;
    sqlite3_bind_text(stmt, 1, customerCpf.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    if (count > 0) {
        std::cerr << "Erro: Você precisa esperar que todos os seus itens (" << count << ") sejam devolvidos antes de deletar sua conta." << std::endl;
        return false; // Fails constraint
    }

    return true; // OK to delete
}

/**
 * @brief Permanently deletes a user's account and all associated data.
 * @details Uses a transaction. Deletes from RENTALS, ITEM, ACCOUNT, and CUSTOMER tables.
 * @param customerId The user's ID.
 * @param accountId The account's ID.
 * @param customerCpf The user's CPF.
 * @return true on successful deletion, false otherwise (rolls back).
 */
bool Repository::deleteUserAccount(int customerId, const std::string& accountId, const std::string& customerCpf) {
    if (!db) return false;

    // First, check if deletion is allowed
    if (!checkUserDeleteConstraints(customerId, customerCpf)) {
        return false;
    }

    if (!executeSQL("BEGIN TRANSACTION;")) return false; // Start transaction

    bool success = true;

    // 1. Delete all their rental records
    if (success) success = executeSQL("DELETE FROM RENTALS WHERE CUSTOMER_ID = " + std::to_string(customerId) + ";");
    // 2. Delete all items they own
    if (success) success = executeSQL("DELETE FROM ITEM WHERE OWNER_CPF = '" + customerCpf + "';");
    // 3. Delete their account entry
    if (success) success = executeSQL("DELETE FROM ACCOUNT WHERE ID = '" + accountId + "';");
    // 4. Delete their customer profile (FOREIGN KEY cascade should handle this, but explicit is safer)
    if (success) success = executeSQL("DELETE FROM CUSTOMER WHERE ID = " + std::to_string(customerId) + ";");

    if (success) {
        return executeSQL("COMMIT;"); // All deletions succeeded
    } else {
        std::cerr << "Erro durante a transação de exclusão. Desfazendo." << std::endl;
        executeSQL("ROLLBACK;"); // One of the steps failed
        return false;
    }
}