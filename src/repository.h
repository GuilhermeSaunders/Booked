#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>  // For future "get" functions
#include <utility> // For std::pair

// Data Models
#include "product.h"
#include "book.h"
#include "movie.h"
#include "video_game.h"
#include "board_game.h"
#include "user.h"
#include "account.h"
#include "rental.h"

/**
 * @class Repository
 * @brief Manages all communication between the application and the SQLite database.
 *
 * This class encapsulates all SQL logic, acting as a Data Access Layer (DAL).
 * It handles opening/closing the connection, initializing the schema,
 * and performing all CRUD (Create, Read, Update, Delete) operations.
 */
class Repository {
private:
    sqlite3* db; ///< The handle for the active SQLite database connection.

    /**
     * @brief Internal helper function to execute SQL that does not return data.
     * @details Used for CREATE, INSERT, UPDATE, DELETE.
     * @param sql The SQL statement to execute.
     * @return true if execution was successful, false otherwise.
     */
    bool executeSQL(const std::string& sql);

public:
    /**
     * @brief Constructor. Opens the database connection.
     * @param db_path The file path to the .db file.
     */
    Repository(const std::string& db_path);

    /**
     * @brief Destructor. Closes the database connection.
     */
    ~Repository();

    /**
     * @brief Initializes the database schema.
     * @details Executes all "CREATE TABLE IF NOT EXISTS" statements from DBSchema.h.
     */
    void tablesStart();

    // --- Customer/Account Functions ---

    /**
     * @brief Registers a new User in the CUSTOMER table.
     * @param user A pointer to the User object. The object's ID will be set upon success.
     * @return true on successful insertion, false otherwise.
     */
    bool registerUser(User* user);

    /**
     * @brief Registers a new Account in the ACCOUNT table.
     * @param account The Account object to save.
     * @return true on successful insertion, false otherwise.
     */
    bool registerAccount(const Account& account);

    /**
     * @brief Retrieves an Account and its associated User data by username.
     * @param username The username to search for.
     * @return A pointer to a new, heap-allocated Account object if found; nullptr otherwise.
     * @warning The caller is responsible for deleting the returned pointer.
     */
    Account* getAccountByUsername(const std::string& username);

    // --- Product (CREATE) Functions ---

    /**
     * @brief Registers a new Book (in ITEM and BOOK tables).
     * @param book A pointer to the Book object. The object's ID will be set upon success.
     * @return true on successful transaction, false otherwise.
     */
    bool registerBook(Book* book);

    /**
     * @brief Registers a new Movie (in ITEM and MOVIE tables).
     * @param movie A pointer to the Movie object. The object's ID will be set upon success.
     * @return true on successful transaction, false otherwise.
     */
    bool registerMovie(Movie* movie);

    /**
     * @brief Registers a new Video Game (in ITEM and VIDEOGAME tables).
     * @param videogame A pointer to the Video_Game object. The object's ID will be set upon success.
     * @return true on successful transaction, false otherwise.
     */
    bool registerVideoGame(Video_Game* videogame);

    /**
     * @brief Registers a new Board Game (in ITEM and BOARDGAME tables).
     * @param boardgame A pointer to the Board_Game object. The object's ID will be set upon success.
     * @return true on successful transaction, false otherwise.
     */
    bool registerBoardGame(Board_Game* boardgame);

    // --- Product (READ) Functions ---
    // These methods print directly to std::cout using a callback.

    /**
     * @brief Lists all products in the ITEM table to the console.
     */
    void listAllProducts();

    /**
     * @brief Lists all products of a specific type (e.g., "BOOK") to the console.
     * @param type The type string to filter by.
     */
    void listProductsByType(const std::string& type);

    /**
     * @brief Lists all products owned by a specific user to the console.
     * @param ownerCpf The owner's CPF string.
     */
    void listProductsByOwner(const std::string& ownerCpf);

    /**
     * @brief Lists all rental records for a specific user to the console.
     * @param customerId The user's (customer) ID.
     */
    void listRentalsByCustomer(int customerId);

    // --- Update Functions ---

    /**
     * @brief Updates a user's full name in the CUSTOMER table.
     * @param customerId The user's ID.
     * @param newName The new full name.
     * @return true on success, false otherwise.
     */
    bool updateUserFullName(int customerId, const std::string& newName);

    /**
     * @brief Updates an account's username in the ACCOUNT table.
     * @param accountId The account's unique ID.
     * @param newUsername The new username.
     * @return true on success, false otherwise.
     */
    bool updateAccountUsername(const std::string& accountId, const std::string& newUsername);

    /**
     * @brief Updates an account's password hash in the ACCOUNT table.
     * @param accountId The account's unique ID.
     * @param newHash The new password hash.
     * @return true on success, false otherwise.
     */
    bool updateAccountPassword(const std::string& accountId, size_t newHash);

    // --- Delete Functions ---

    /**
     * @brief Checks constraints before allowing an account deletion.
     * @details Fails if the user has active rentals or owns items that are rented out.
     * @param customerId The user's ID.
     * @param customerCpf The user's CPF.
     * @return true if deletion is safe, false otherwise.
     */
    bool checkUserDeleteConstraints(int customerId, const std::string& customerCpf);

    /**
     * @brief Deletes a user account and all associated data (items, rentals).
     * @details Runs as a transaction.
     * @param customerId The user's ID.
     * @param accountId The account's ID.
     * @param customerCpf The user's CPF.
     * @return true on successful transaction, false otherwise.
     */
    bool deleteUserAccount(int customerId, const std::string& accountId, const std::string& customerCpf);

    /**
     * @brief Deletes a single product from the ITEM table.
     * @details Fails if the user is not the owner or if the item is "Alugado" (Rented).
     * @param itemId The ID of the item to delete.
     * @param ownerCpf The CPF of the user attempting the deletion.
     * @return true on success, false otherwise.
     */
    bool deleteProduct(int itemId, const std::string& ownerCpf);

    // --- Rental Functions ---

    /**
     * @brief Registers a new rental in the RENTALS table.
     * @param rental A pointer to the Rental object. The object's ID will be set upon success.
     * @return true on successful insertion, false otherwise.
     */
    bool registerRental(Rental* rental);

    /**
     * @brief Processes a rental return.
     * @details Runs as a transaction: Sets RENTALS status to "Devolvido" (Returned)
     * and ITEM status to "Disponivel" (Available).
     * @param rentalId The ID of the rental transaction.
     * @return true on successful transaction, false otherwise.
     */
    bool returnRental(int rentalId);

    /**
     * @brief Gets an item's current status and price.
     * @details Used to check if an item is available for rent.
     * @param itemId The ID of the item to check.
     * @return A pair of (Status, Price). Status will be "NaoEncontrado" (Not Found) on failure.
     */
    std::pair<std::string, float> getItemStatusAndPrice(int itemId);

    /**
     * @brief Updates the STATUS field of a specific item.
     * @param itemId The ID of the item to update.
     * @param newStatus The new status string (e.g., "Alugado").
     * @return true on success, false otherwise.
     */
    bool updateItemStatus(int itemId, const std::string& newStatus);
};