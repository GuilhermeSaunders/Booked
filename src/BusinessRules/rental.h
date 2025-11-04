#ifndef RENTAL_H
#define RENTAL_H

#include <string>
// using namespace std; // Avoid 'using namespace std' in header files

/**
 * @class Rental
 * @brief Represents a single rental transaction.
 *
 * This class holds the data for a rental, which is created by App.cpp
 * and passed to the Repository to be saved in the database.
 */
class Rental {
private:
    int transactionId; // The unique ID from the database
    int duration;      // Duration of the rental in days
    int borrowerId;  // The CUSTOMER ID of the person renting
    float dailyRate;   // The price per day
    std::string startDate; // Start date (e.g., "YYYY-MM-DD")
    std::string status;    // "Pendente", "Ativo", "Completo", etc.
    int productId;     // The ITEM ID of the product being rented

public:
    /**
     * @brief Constructor for a new rental.
     * @param productId ID of the item.
     * @param borrowerId ID of the customer.
     * @param duration_days Duration in days.
     * @param start_date Start date.
     * @param dailyRate Price per day.
     */
    Rental(int productId, int borrowerId, int duration_days, std::string start_date, float dailyRate);

    // --- Methods used by App.cpp ---

    /**
     * @brief Sets the status of the rental (e.g., "Ativo").
     * @param newStatus The new status string.
     */
    void setStatus(const std::string& newStatus);

    /**
     * @brief Sets the database transaction ID.
     * @param id The ID assigned by the database.
     */
    void setTransactionId(int id);

    /**
     * @brief Gets the database transaction ID.
     * @return The transaction ID.
     */
    int getTransactionId() const;

    // --- Getters (Primarily used by Repository to save data) ---

    int getDuration() const;
    std::string getStatus() const;
    int getBorrowerId() const;
    std::string getStartDate() const;
    float getDailyRate() const;
    int getProductId() const;

};

#endif