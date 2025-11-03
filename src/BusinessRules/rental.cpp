// Rental.cpp (A IMPLEMENTAÇÃO CORRETA)
#include "Rental.h"
#include <utility> 

// Construtor
Rental::Rental(int productId, int customerId, int duration_days, string start_date, float dailyRate)
    : duration(duration_days),
      customerId(customerId), // <-- A MUDANÇA
      productId(productId),
      dailyRate(dailyRate),
      startDate(std::move(start_date)),
      id(-1),            // ID inicial = -1 (não salvo)
      status("Ativo")  
{
    // Corpo vazio
}

// Métodos de ID
void Rental::setId(int id) { this->id = id; }
int Rental::getId() const { return this->id; }

// Métodos de Status
void Rental::setStatus(const string& newStatus) { this->status = newStatus; }
string Rental::getStatus() const { return this->status; }

// --- OS GETTERS CORRETOS ---
int Rental::getDuration() const { return this->duration; }
int Rental::getCustomerId() const { return this->customerId; } // <-- A MUDANÇA
int Rental::getProductId() const { return this->productId; }
string Rental::getStartDate() const { return this->startDate; }
float Rental::getDailyRate() const { return this->dailyRate; }

// --- Lógica (a implementar) ---
bool Rental::isOverdue(const string& currentDate) { return false; }
void Rental::completeRental() { this->status = "Concluido"; }
string Rental::calculateEndDate() { return ""; }
double Rental::finalPrice() const { return this->dailyRate * this->duration; }