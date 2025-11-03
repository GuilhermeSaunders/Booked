#include "rental.h"
#include <utility> 
#include <string>
#include <stdio.h> 
#include <stdlib.h> 

// CORREÇÃO: Ordem dos inicializadores batendo com o rental.h
Rental::Rental(int productId, 
                int borrowerId, 
                int duration_days, 
                std::string start_date, 
                float dailyRate)
:   transactionId(-1),
    duration(duration_days),
    borrowerId(borrowerId),
    dailyRate(dailyRate),
    startDate(start_date),
    status("Pendente"),
    productId(productId)
{}

// --- AS FUNÇÕES FALTANTES ESTÃO AQUI ---

void Rental::setStatus(const std::string& newStatus) {
    status = newStatus;
}
void Rental::setTransactionId(int id) {
    transactionId = id;
}
int Rental::getTransactionId() const { return transactionId; }
int Rental::getDuration() const {
    return duration < 0 ? 0 : duration;
}

// --- FIM DAS FUNÇÕES FALTANTES ---

int Rental::getProductId() const { return productId; }
std::string Rental::getStatus() const { return status; }
int Rental::getBorrowerId() const { return borrowerId; }
float Rental::getDailyRate() const { return dailyRate; }
std::string Rental::getStartDate() const { return startDate; }

std::string Rental::calculateEndDate() {
    int year = stoi(startDate.substr(0,4));
    int month = stoi(startDate.substr(5,2));
    int day = stoi(startDate.substr(8,2));
    day += duration;
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
    while (day > daysInMonth[month]) {
        day -= daysInMonth[month];
        month++;
        if (month > 12) { month = 1; year++; }
    }
    char buffer[11];
    // CORREÇÃO: Trocado sprintf por snprintf (mais seguro)
    snprintf(buffer, 11, "%04d-%02d-%02d", year, month, day); 
    return std::string(buffer);
}
bool Rental::isOverdue(const std::string& currentDate) {
    std::string endDate = calculateEndDate();
    return currentDate > endDate;
}
void Rental::completeRental() {
    status = "completed";
}
double Rental::finalPrice() const {
    return dailyRate * duration;
}