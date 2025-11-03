// Repository.cpp
#include "Repositorio.h"    
#include "DBSchema.h"       
#include <iostream>         
#include <string>           

#include "Product\Book.h"
#include "Product\Movie.h"
#include "Product\video_game.h"
#include "Product\board_game.h"

static int callback_print_lines(void* data, int argc, char** argv, char** azColName){
    // 'argc' = number of columns
    // 'argv' = array of values (e.g., "The Lord of the Rings")
    // 'azColName' = array of column names (e.g., "NAME")

    std::cout << "------------------------------------\n";
    for(int i = 0; i < argc; i++){
        // Prints: COLUMN_NAME = VALUE
        std::cout << "  " << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0; // 0 = Success, continue to the next row
}


// Opens the database connection
Repositorio::Repositorio(const std::string& db_path) : db(nullptr) {
    // Try to open the database
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        // If it fails, print the error and ensure 'db' is null
        std::cerr << "Could not open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    } else {
        std::cout << "Database '" << db_path << "' opened successfully." << std::endl;
        // IMPORTANT: Enable Foreign Key support
        executeSQL("PRAGMA foreign_keys = ON;");
    }
}

// --- Destructor ---
// Closes the database connection
Repositorio::~Repositorio() {
    if (db) {
        sqlite3_close(db);
        std::cout << "Database closed." << std::endl;
    }
}

// --- Private Helper Function ---
// Executes SQL that does NOT return data (CREATE, INSERT, UPDATE, DELETE)
bool Repositorio::executeSQL(const std::string& sql) {
    if (!db) return false; // Do nothing if the database isn't open
    
    char* errMsg = 0;
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        // If it errors, print the error and the SQL that failed
        std::cerr << "Error executing SQL: " << errMsg << std::endl;
        std::cerr << "Failed SQL: " << sql << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// --- Initialization Function ---
// Executes all creation strings from DBSchema.h
void Repositorio::tablesStart() {
    if (!db) return;
    std::cout << "Initializing tables (if they don't exist)..." << std::endl;
    executeSQL(DBSchema::CUSTOMER);
    executeSQL(DBSchema::ITEM);
    executeSQL(DBSchema::BOOKS);
    executeSQL(DBSchema::MOVIES);
    executeSQL(DBSchema::VIDEOGAMES);
    executeSQL(DBSchema::BOARDGAMES);
    executeSQL(DBSchema::RENTALS);
    std::cout << "Tables initialized." << std::endl;
}

bool Repositorio::registerUser(User* user) {
    if (!db || !user) return false;

    // Build the SQL using getters from User.h
    std::string sql = "INSERT INTO CUSTOMER (CPF, NAME, EMAIL, BIRTHDAY) VALUES ('"
                    + user->getCpf() + "', '"
                    + user->getFullname() + "', '"
                    + user->getEmail() + "', '"
                    + user->getBirthday() + "');";
    
    if (executeSQL(sql)) {
        // "Option A" Magic: Get the ID the database just generated...
        long long id = sqlite3_last_insert_rowid(db);
        // ...and update the C++ object in memory
        user->setId(static_cast<int>(id));
        return true;
    }
    return false;
}

bool Repositorio::registerRental(Rental* rental) {
    if (!db || !rental) return false;

    // Build the SQL using getters from Rental.h
    std::string sql = "INSERT INTO RENTALS (CUSTOMER_ID, ITEM_ID, START_DATE, DURATION, DAILY_RATE, STATUS) VALUES ("
                    + std::to_string(rental->getCustomerId()) + ", "
                    + std::to_string(rental->getProductId()) + ", '"
                    + rental->getStartDate() + "', "
                    + std::to_string(rental->getDuration()) + ", "
                    + std::to_string(rental->getDailyRate()) + ", '"
                    + rental->getStatus() + "');";
    
    if (executeSQL(sql)) {
        // "Option A" Magic
        long long id = sqlite3_last_insert_rowid(db);
        rental->setId(static_cast<int>(id)); // Update the object
        return true;
    }
    return false;
}

bool Repositorio::registerProduct(Product* product) {
    if (!db || !product) return false;

    // 1. Discover the TYPE (using the pure virtual function)
    std::string type = product->getType(); // "BOOK", "MOVIE", etc.

    // 2. Save to the PARENT table (ITEM)
    // Build the SQL using getters from Product.h
    std::string sql_pai = "INSERT INTO ITEM (NAME, DESCRIPTION, OWNER, GENRE, IDIOM, RENT_VALUE, RECOMMENDED_AGE, TYPE, STATUS) VALUES ('"
                        + product->getName() + "', '"
                        + product->getDescription() + "', '"
                        + product->getOwner() + "', '"
                        + product->getGenre() + "', '"
                        + product->getIdiom() + "', "
                        + std::to_string(product->getRentValue()) + ", '"
                        + product->getRecommendedAge() + "', '"
                        + type + "', 'Disponivel');";
    
    if (!executeSQL(sql_pai)) {
        std::cerr << "Failed to save to PARENT table (ITEM)." << std::endl;
        return false;
    }

    // 3. "Option A" Magic: Get the ID generated by the database
    long long id = sqlite3_last_insert_rowid(db);
    
    // Update the C++ object (in memory) with the real ID from the database
    product->setId(static_cast<int>(id));

    // 4. Save to the specific CHILD table
    std::string sql_filha;
    
    if (type == "BOOK") {
        // If it's "BOOK", we can safely cast the pointer
        Book* livro = static_cast<Book*>(product);
        sql_filha = "INSERT INTO BOOK (ITEM_ID, AUTHOR, PAGE_NUM) VALUES ("
                  + std::to_string(id) + ", '"
                  + livro->getAuthor() + "', "
                  + std::to_string(livro->getNumPages()) + ");";
    
    } else if (type == "MOVIE") {
        Movie* filme = static_cast<Movie*>(product);
        sql_filha = "INSERT INTO MOVIE (ITEM_ID, DIRECTOR, MAIN_ACTORS, DURATION) VALUES ("
                  + std::to_string(id) + ", '"
                  + filme->getDirector() + "', '"
                  + filme->getMainActors() + "', "
                  + std::to_string(filme->getDuration()) + ");";
    
    } else if (type == "VIDEOGAME") {
        Video_Game* jogo = static_cast<Video_Game*>(product);
        sql_filha = "INSERT INTO VIDEOGAME (ITEM_ID, STYLE, PLATAFORM, DURATION, PLAYERS_NUM) VALUES ("
                  + std::to_string(id) + ", '"
                  + jogo->getStyle() + "', '"
                  + jogo->getPlataform() + "', "
                  + std::to_string(jogo->getDuration()) + ", "
                  + std::to_string(jogo->getNumPlayers()) + ");";
    
    } else if (type == "BOARDGAME") {
        Board_Game* jogo = static_cast<Board_Game*>(product);
        sql_filha = "INSERT INTO BOARDGAME (ITEM_ID, STYLE, PLAYERS_NUM, DURATION) VALUES ("
                  + std::to_string(id) + ", '"
                  + jogo->getStyle() + "', "
                  + std::to_string(jogo->getNumPlayers()) + ", "
                  + std::to_string(jogo->getDuration()) + ");";
    } else {
         std::cerr << "Unknown type, nothing saved to child table." << std::endl;
         return true; // Not an error, just no child table
    }

    // 5. Execute the child table SQL
    return executeSQL(sql_filha);
}


void Repositorio::listUsers() {
    if (!db) return;
    std::cout << "\n--- Listing All Customers ---" << std::endl;
    std::string sql_select = "SELECT * FROM CUSTOMER;";
    
    char* errMsg = 0;
    sqlite3_exec(db, sql_select.c_str(), callback_print_lines, 0, &errMsg);
    // (callback_print_lines does the std::cout for us)
}

void Repositorio::listRentals() {
    if (!db) return;
    std::cout << "\n--- Listing All Rentals ---" << std::endl;
    // We use JOINs to show names, not just IDs
    std::string sql_select = 
        "SELECT T1.ID, T2.NAME AS CUSTOMER_NAME, T3.NAME AS ITEM_NAME, T1.START_DATE, T1.STATUS "
        "FROM RENTALS AS T1 "
        "JOIN CUSTOMER AS T2 ON T1.CUSTOMER_ID = T2.ID "
        "JOIN ITEM AS T3 ON T1.ITEM_ID = T3.ID;";

    char* errMsg = 0;
    sqlite3_exec(db, sql_select.c_str(), callback_print_lines, 0, &errMsg);
}

void Repositorio::listProducts(const std::string& categoria) {
    if (!db) return;

    std::string sql_select;
    std::cout << "\n--- Listing Products in Category: " << categoria << " ---" << std::endl;

    // We use JOINs to get data from the PARENT table (ITEM)
    // and the specific CHILD table (e.g., BOOK)
    
    if (categoria == "BOOK") {
        sql_select = "SELECT T1.ID, T1.NAME, T1.STATUS, T2.AUTHOR, T2.PAGE_NUM "
                     "FROM ITEM AS T1 "
                     "JOIN BOOK AS T2 ON T1.ID = T2.ITEM_ID;";
    } else if (categoria == "MOVIE") {
        sql_select = "SELECT T1.ID, T1.NAME, T1.STATUS, T2.DIRECTOR, T2.MAIN_ACTORS, T2.DURATION "
                     "FROM ITEM AS T1 "
                     "JOIN MOVIE AS T2 ON T1.ID = T2.ITEM_ID;";
    } else if (categoria == "VIDEOGAME") {
        sql_select = "SELECT T1.ID, T1.NAME, T1.STATUS, T2.STYLE, T2.PLATAFORM, T2.PLAYERS_NUM "
                     "FROM ITEM AS T1 "
                     "JOIN VIDEOGAME AS T2 ON T1.ID = T2.ITEM_ID;";
    } else if (categoria == "BOARDGAME") {
        sql_select = "SELECT T1.ID, T1.NAME, T1.STATUS, T2.STYLE, T2.PLAYERS_NUM, T2.DURATION "
                     "FROM ITEM AS T1 "
                     "JOIN BOARDGAME AS T2 ON T1.ID = T2.ITEM_ID;";
    } else {
        std::cout << "Unknown or empty category. Listing all base items..." << std::endl;
        sql_select = "SELECT ID, NAME, TYPE, STATUS FROM ITEM;";
    }

    char* errMsg = 0;
    sqlite3_exec(db, sql_select.c_str(), callback_print_lines, 0, &errMsg);
    if (errMsg) sqlite3_free(errMsg); // Clear the error if one exists
}