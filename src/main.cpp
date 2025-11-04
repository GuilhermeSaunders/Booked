#include <iostream>

// --- Model Headers ---
#include "user.h"
#include "account.h"
#include "rental.h"

// --- Product Model Headers ---
// Note: The compiler warning suggests the file might be "product.h" (lowercase)
// If you get "non-portable path" warnings, change "Product.h" to "product.h"
#include "product.h"
#include "book.h"
#include "movie.h"
#include "board_game.h"
#include "video_game.h"

// --- Service/Application Headers ---
#include "login.h"
#include "validate.h"
#include "app.h"
#include "repository.h"

/**
 * @mainpage Booked Rental System
 *
 * @section intro_sec Introduction
 * This is the main entry point for the Booked application, a C++ console-based
 * item rental system.
 *
 * @section setup_sec Setup
 * The main function initializes:
 * 1. The database connection (Repository).
 * 2. The database schema (tablesStart).
 * 3. The main application controller (App).
 *
 * @section running_sec Running
 * It then calls `application.run()` to start the user interface loop.
 */

/**
 * @brief Main function and entry point for the application.
 * @return 0 on successful execution.
 */
int main() {

    // 1. Initialize the Repository, connecting to the SQLite database file.
    // Note: The class name "Repositorio" is used here.
    Repository db("aluguel.db");

    // 2. Initialize the database schema (Create tables if they don't exist).
    db.tablesStart();

    // 3. Initialize the main application, injecting the database dependency.
    App application(db);

    // 4. Start the application's main loop (shows auth menu, etc.).
    application.run();

    // 5. When application.run() finishes, the program exits.
    std::cout << "Saindo do sistema. Até logo!" << std::endl;
    return 0;
}