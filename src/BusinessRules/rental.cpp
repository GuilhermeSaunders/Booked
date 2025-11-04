#include "rental.h"
#include <string>
#include <stdio.h>  // For snprintf
#include <stdlib.h> // For stoi (included by <string> in C++11)

/**
 * @brief Constructor for Rental.
 * Initializes a new rental with a default ID of -1 and "Pendente" status.
 */
Rental::Rental(int productId,
               int borrowerId,
               int duration_days,
               std::string start_date,
               float dailyRate)
    : transactionId(-1),          // Default ID until set by database
      duration(duration_days),
      borrowerId(borrowerId),
      dailyRate(dailyRate),
      startDate(start_date),
      status("Pendente"),         // Default status
      productId(productId) {
}

/**
 * @brief Updates the rental's status.
 */
void Rental::setStatus(const std::string& newStatus) {
    status = newStatus;
}

/**
 * @brief Sets the rental's unique ID (after database insertion).
 */
void Rental::setTransactionId(int id) {
    transactionId = id;
}

/**
 * @brief Gets the rental's unique ID.
 */
int Rental::getTransactionId() const {
    return transactionId;
}

/**
 * @brief Gets the rental duration in days.
 */
int Rental::getDuration() const {
    // Ensures duration is never negative
    return duration < 0 ? 0 : duration;
}

/**
 * @brief Gets the item's ID.
 */
int Rental::getProductId() const {
    return productId;
}

/**
 * @brief Gets the current status string.
 */
std::string Rental::getStatus() const {
    return status;
}

/**
 * @brief Gets the borrower's (customer) ID.
 */
int Rental::getBorrowerId() const {
    return borrowerId;
}

/**
 * @brief Gets the daily rental rate.
 */
float Rental::getDailyRate() const {
    return dailyRate;
}

/**
 * @brief Gets the start date string.
 */
std::string Rental::getStartDate() const {
    return startDate;
}