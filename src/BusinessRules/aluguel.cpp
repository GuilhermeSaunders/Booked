#include "aluguel.h"

// Methods
    // Constructor
    Rental::Rental(int id, string lenderName, string borrowerName, int duration_days, string start_date, float dailyRateValue)
    : transactionId(id), lender(lenderName), borrower(borrowerName), duration(duration_days), startDate(start_date), dailyRate(dailyRateValue) {};

    // Set the rental status (Rented, Available, Out of Stock...)
    void Rental::setStatus(const string& newStatus) {
        status = newStatus;
    }

    // Getters
    int Rental::getTransactionId() const { return transactionId; }
    int Rental::getDuration() const {
        return duration < 0 ? 0 : duration;
    }
    string Rental::getStatus() const { return status; }
    string Rental::getLender() const { return lender; }
    string Rental::getBorrower() const { return borrower; }
    float Rental::getDailyRate() const { return dailyRate; }
    string Rental::getStartDate() const { return startDate; }

    // Method to calculate the end date and check if it is overdue
    string Rental::calculateEndDate() {
        // Simple example: if startDate = "2025-10-08" and duration = 7 days
        // Here you need to convert the string into year, month, and day
        int year = stoi(startDate.substr(0,4));
        int month = stoi(startDate.substr(5,2));
        int day = stoi(startDate.substr(8,2));

        day += duration; // adding days

        // Simple month adjustment (does not cover all cases, just an example)
        int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        while (day > daysInMonth[month - 1]) {
            day -= daysInMonth[month - 1];
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }

        char buffer[11];
        sprintf(buffer, "%04d-%02d-%02d", year, month, day);
        return string(buffer);
    }

    // Functionalities
    bool Rental::isOverdue(const string& currentDate) {
        string endDate = calculateEndDate();

        if (currentDate > endDate) {
            return true;
        }
        return false;
    }

    void Rental::completeRental() {
        status = "completed";
    }

    double Rental::finalPrice() const {
        return dailyRate * duration;
    }