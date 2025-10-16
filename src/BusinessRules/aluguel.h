#ifndef RENTAL_H
#define RENTAL_H
#include <string>
using namespace std;
// nome certo dos arquivos #justiçapordanielmolestador 

class Rental {
private:
    int transactionId;
    int duration;
    string lender;
    string borrower;
    float dailyRate;
    string startDate;
    string status;
    string productType;
    int productId; // precisa implementar o get e set e implementar no construtor 

public:
    // Constructor
    Rental(int id, string lender, string borrower, int duration_days, string start_date, float dailyRate);

    // Methods
    void setStatus(const string& newStatus);
    int getTransactionId() const;
    int getDuration() const;
    string getStatus() const;
    string getLender() const;
    string getBorrower() const;
    string getStartDate() const;
    float getDailyRate() const;
    string getProductType() const {return productType;}

    // Functionalities
    bool isOverdue(const string& currentDate);
    void completeRental();
    string calculateEndDate();
    double finalPrice() const;
};

#endif