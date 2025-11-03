#ifndef RENTAL_H
#define RENTAL_H
#include <string>
using namespace std;

class Rental {
private:
    int transactionId;
    int duration;
    int borrowerId; // MUDANÇA: de string borrowerCpf para int borrowerId
    float dailyRate;
    string startDate;
    string status;
    int productId;

public:
    // Construtor atualizado
    Rental(int productId, int borrowerId, int duration_days, string start_date, float dailyRate);

    // Methods
    void setStatus(const string& newStatus);
    void setTransactionId(int id);

    // Getters atualizados
    int getTransactionId() const;
    int getDuration() const;
    string getStatus() const;
    int getBorrowerId() const; // MUDANÇA
    string getStartDate() const;
    float getDailyRate() const;
    int getProductId() const;

    // Functionalities
    bool isOverdue(const string& currentDate);
    void completeRental();
    string calculateEndDate();
    double finalPrice() const;
};

#endif