#ifndef APP_H
#define APP_H

#include <string>
#include "login.h"
#include "account.h"
#include "repository.h" // Includes the Repository
#include "validate.h"   // Includes the Validator

// Include all product models for creation
#include "book.h"
#include "movie.h"
#include "video_game.h"
#include "board_game.h"

// Note: 'using namespace std;' in a header file is
// generally discouraged as it pollutes the global namespace.
using namespace std;

/**
 * @class App
 * @brief Main application class that controls the user interface and flow.
 *
 * This class acts as the "controller" for the entire application.
 * It manages user sessions, navigation between menus (routes),
 * input collection, and orchestrates calls to the Repository
 * and other services.
 */
class App {
private:
    Repository& db;       ///< A reference to the database (Dependency Injection).
    Login loginSession;   ///< Manages the current user's login state (session).
    Validate validator;   ///< Utility service for validating input.
    Account* currentUser; ///< Pointer to the logged-in user's Account. Null if logged out.

    // --- Private Methods (Internal "Routes" & Utilities) ---

    // --- Utility Helpers ---
    /** @brief Clears the standard input buffer (cin). */
    void clearInputBuffer();
    /** @brief Clears the console/terminal screen. */
    void clearScreen();
    /**
     * @brief Gets a full line of string input (using getline).
     * @param prompt The message to display to the user.
     * @return The user's input string.
     */
    string getStringLine(const string& prompt);
    /**
     * @brief Gets a validated integer input.
     * @param prompt The message to display to the user.
     * @return The user's input integer.
     */
    int getInt(const string& prompt);
    /**
     * @brief Gets a validated float input.
     * @param prompt The message to display to the user.
     * @return The user's input float.
     */
    float getFloat(const string& prompt);
    /**
     * @brief Helper to collect common data fields for all product types.
     * @param name [out] The product's name.
     * @param desc [out] The product's description.
     * @param genre [out] The product's genre.
     * @param idiom [out] The product's idiom.
     * @param rentValue [out] The product's daily rent value.
     * @param recAge [out] The product's recommended age.
     */
    void collectCommonProductData(std::string& name, std::string& desc, std::string& genre, std::string& idiom, float& rentValue, std::string& recAge);

    // --- Authentication Routes ---
    /** @brief Displays the main authentication menu (Login/Register/Exit). */
    void showAuthMenu();
    /** @brief Handles the user login workflow. */
    void handleLogin();
    /** @brief Handles the new user registration workflow. */
    void handleRegister();

    // --- Main Application Routes (Post-Login) ---
    /** @brief Displays the main menu for logged-in users. */
    void showMainMenu();
    /** @brief Handles viewing the product catalog and renting items. */
    void handleCatalog();
    /** @brief Shows the menu for adding a new product. */
    void handleAddProduct();
        // --- Add Product Sub-Routes ---
        /** @brief Handles adding a new Book. */
        void handleAddBook();
        /** @brief Handles adding a new Movie. */
        void handleAddMovie();
        /** @brief Handles adding a new Video Game. */
        void handleAddVideoGame();
        /** @brief Handles adding a new Board Game. */
        void handleAddBoardGame();
    /** * @brief Handles the user profile menu (View/Update/Delete).
     * @return true if the user deleted their account, false otherwise.
     */
    bool handleProfile();
    /** @brief Handles viewing and managing the user's own listed products. */
    void handleMyProducts();
    /** @brief Handles viewing the user's rental history and returning items. */
    void handleRentedProducts();
    /** @brief Handles the user logout process. */
    void handleLogout();
    /** @brief Handles the workflow for renting an item from the catalog. */
    void handleRentItem();
    /** @brief Handles the workflow for returning a currently rented item. */
    void handleReturnItem();
    /** @brief Handles deleting one of the user's own products. */
    void handleDeleteProduct();

    // --- Profile Sub-Routes ---
    /** @brief Shows the menu for updating profile information. */
    bool handleUpdateProfile();
    /** @brief Handles changing the user's full name. */
    void handleUpdateFullName();
    /** @brief Handles changing the user's login username. */
    void handleUpdateUsername();
    /** @brief Handles changing the user's password. */
    void handleUpdatePassword();
    /** * @brief Handles the permanent deletion of the user's account.
     * @return true if deletion was successful, false otherwise.
     */
    bool handleDeleteAccount();

public:
    /**
     * @brief Constructor for the App.
     * @param database A reference to the initialized Repository.
     */
    App(Repository& database);

    /**
     * @brief Destructor.
     * @details Cleans up the dynamically allocated currentUser pointer.
     */
    ~App();

    /**
     * @brief The main entry point for the application.
     * @details This is the primary method called by main() to start the app.
     */
    void run();
};

#endif