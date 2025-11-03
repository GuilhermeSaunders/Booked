// Rental.h (O CONTRATO CORRETO)
#ifndef RENTAL_H
#define RENTAL_H
#include <string>
using namespace std;

class Rental {
private:
    int id;         // O ID da transação (gerado pelo banco)
    int duration;
    int customerId; // <-- A MUDANÇA: O ID do cliente (era string borrower)
    int productId;  // O ID do produto
    float dailyRate;
    string startDate;
    string status;

public:
    // Construtor (sem 'id')
    Rental(int productId, int customerId, int duration_days, string start_date, float dailyRate);

    // Getters/Setters de Status
    void setStatus(const string& newStatus);
    string getStatus() const;
    
    // Getters/Setters de ID (para o Repositorio)
    void setId(int id);
    int getId() const;

    // --- OS GETTERS CORRETOS ---
    int getDuration() const;
    int getCustomerId() const; // <-- A MUDANÇA: O método que o Repositorio procura
    int getProductId() const; 
    string getStartDate() const;
    float getDailyRate() const;

    // Funções de Lógica
    bool isOverdue(const string& currentDate);
    void completeRental();
    string calculateEndDate();
    double finalPrice() const;
};

#endif