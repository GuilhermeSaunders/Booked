#ifndef RENTAL_H
#define RENTAL_H
#include <string>
using namespace std;

class Rental {
private:
    int transactionId;
    int duration;
    string lender;
    string borrower;
    double dailyRate;
    string startDate;
    string status;

public:
    // Constructor
    Rental(int id, string lender, string borrower, int duration_days, string start_date, double dailyRate);

    // Methods
    void setStatus(const string& newStatus);
    int getTransactionId() const;
    int getDuration() const;
    string getStatus() const;
    string getLender() const;
    string getBorrower() const;
    string getStartDate() const;
    double getDailyRate() const;

    // Functionalities
    bool isOverdue(const string& currentDate);
    void completeRental();
    string calculateEndDate();
    double finalPrice() const;
};

#endif